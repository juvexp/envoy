package gfe.quic.tools.cronetspeedtest.android;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.RadioButton;
import android.widget.TextView;
import android.widget.Toast;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.nio.ByteBuffer;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import javax.annotation.Nullable;
import org.chromium.net.CronetEngine;
import org.chromium.net.CronetException;
import org.chromium.net.ExperimentalCronetEngine;
import org.chromium.net.UploadDataSink;
import org.chromium.net.UrlRequest;
import org.chromium.net.UrlResponseInfo;

/** Tests uploads and downloads. */
public class SpeedTestActivity extends Activity {

  // Wrapper for Future<Void> to allow a Future<Void> array to be created. Doing Future<Void>[]
  // directly instead of FutureWrapper[] results in generic array creation error.
  private static class FutureWrapper {
    FutureWrapper(Future<Void> future) {
      this.future = future;
    }
    public Future<Void> future;
  }

  // Provides dummy upload data if doing an upload using Cronet.
  private class DummyUploadDataProvider extends org.chromium.net.UploadDataProvider {
    private final Random random;
    private final byte[] randomBuffer;
    private final long bytesTotal;
    private long bytesRemaining;

    DummyUploadDataProvider(long bytes) {
      this.bytesTotal = bytes;
      this.bytesRemaining = bytes;
      this.random = new Random();
      randomBuffer = new byte[1024 * 1024];
      random.nextBytes(randomBuffer);
    }

    @Override
    public long getLength() throws IOException {
      return bytesRemaining;
    }

    @Override
    public void read(UploadDataSink uploadDataSink, ByteBuffer byteBuffer) throws IOException {
      long bytesToWrite = Math.min(bytesRemaining, (long) byteBuffer.remaining());
      bytesToWrite = Math.min(bytesToWrite, (long) randomBuffer.length);
      byteBuffer.put(randomBuffer, 0, (int) bytesToWrite);
      bytesRemaining -= bytesToWrite;
      uploadDataSink.onReadSucceeded(false);

      onBytesCompleted(bytesToWrite);
    }

    @Override
    public void rewind(UploadDataSink uploadDataSink) throws IOException {
      bytesRemaining = bytesTotal;
      uploadDataSink.onRewindSucceeded();
    }
  }

  private static final String TAG = "CronetSpeedTest";
  private static final String HOST_KEY = "host";
  private static final String NUM_STREAMS_KEY = "num_streams";
  private static final String UPLOAD_BYTES_KEY = "upload_bytes";
  private static final String STARTUP_PROTOCOL_KEY = "startup_protocol";
  private static final String STARTUP_MODE_KEY = "startup_mode";
  private static final String QUIC_CONNECTION_OPTIONS_KEY = "quic_connection_options";
  private static final String QUIC_CLIENT_CONNETION_OPTIONS_KEY = "quic_client_connection_options";

  private static final long PROGRESS_THRESHOLD_MILLIS = 200;  // 0.2 second
  private static final int PORT = 8443;
  private static final long UPLOAD_BYTES = 500 * 1000000; // 500 MBytes

  @Nullable private String quicConnectionOptions;
  @Nullable private String quicClientConnectionOptions;
  // For QUIC (and to a lesser degree HTTP/2), gfe single silo performance can be bottlneck.  Work
  // around this by running multiple gfe's listening to distict ports.  Run concurrent requests to
  // boost total throughput.
  private int numStreams;
  private long uploadBytes;

  private EditText hostEditText;
  private CheckBox cronetEnabledCheckbox;
  private RadioButton cronetProtocolH11;
  private RadioButton cronetProtocolH2;
  private RadioButton cronetProtocolQUIC;
  private Button uploadButton;
  private Button downloadButton;
  private Button cancelButton;
  private ProgressBar progressBar;
  private TextView bitrateIndication;
  private TextView transferDetails;

  private ExecutorService executor;
  private UrlRequest[] cronetRequests;     // Cronet requests
  private FutureWrapper[] httpFutures;     // HttpRequest futures

  private final Object lock = new Object();
  private long bytesTotal;      // Total bytes being uploaded/downloaded across all requests
  private long bytesCompleted;  // total bytes uploaded/downloaded so far
  private long startTimeMillis;
  private long prevUiUpdateTimeMillis;

  enum CronetProtocol {
    CRONET_H11,
    CRONET_H2,
    CRONET_QUIC
  }

  // Generates a string indicating the bitrate for the UI.
  private static String bitrateIndicationString(long bytesRead, long timeElapsedMillis) {
    double totalTimeS = timeElapsedMillis / 1000.0;
    double sizeMB = bytesRead / (1000000.0);
    double transferRateMBitS = sizeMB * 8.0 / totalTimeS;
    String bitrateString = String.format("%.2f", transferRateMBitS) + " MBit/s";
    String bytesString = String.format("%.2f", sizeMB) + " MB";
    return bytesString + " at " + bitrateString;
  }

  private void log(String msg, boolean append) {
    Log.i(TAG, msg, null);
    if (append) {
      runOnUiThread(() -> transferDetails.append(msg + "\n"));
    } else {
      runOnUiThread(() -> transferDetails.setText(msg + "\n"));
    }
  }

  private void resetBeforeTransfer() {
    synchronized (lock) {
      bytesTotal = 0;
      bytesCompleted = 0;
      startTimeMillis = System.currentTimeMillis();
      prevUiUpdateTimeMillis = startTimeMillis;
    }
  }

  // Should be called for each request after the size of the upload/download becomes known.
  private void onRequestLengthKnown(long bytes) {
    synchronized (lock) {
      if (bytesTotal >= 0) {
        if (bytes >= 0) {
          bytesTotal += bytes;
        } else {
          bytesTotal = -1;  // Set to invalid if bytesExpected is invalid
        }
      }
    }
  }

  // Should be called after bytes are uploaded/downloaded.
  private void onBytesCompleted(long bytes) {
    synchronized (lock) {
      bytesCompleted += bytes;
    }
    maybeUpdateProgressBar();
  }

  // Will update the progress bar and bitrate string if it's been long enough since the last time
  // this function was called.
  private void maybeUpdateProgressBar() {
    long currentTimeMillis = System.currentTimeMillis();
    boolean shouldUpdate = false;
    synchronized (lock) {
      if (currentTimeMillis >= prevUiUpdateTimeMillis + PROGRESS_THRESHOLD_MILLIS) {
        prevUiUpdateTimeMillis = currentTimeMillis;
        shouldUpdate = true;
      }
    }
    if (shouldUpdate) {
      runOnUiThread(
          () -> {
            bitrateIndication.setText(
                bitrateIndicationString(bytesCompleted, currentTimeMillis - startTimeMillis));
            // Do not update progressBar if bytesTotal is invalid.
            if (bytesTotal >= 0) {
              progressBar.setProgress(
                  (int) (1000L * bytesCompleted / (double) bytesTotal));
            }
          });
    }
  }

  private void onMalformedUrlException(MalformedURLException e) {
    Toast.makeText(SpeedTestActivity.this, "Malformed URL", Toast.LENGTH_LONG)
        .show();
    transferDetails.append("Invalid URL\n");
    transferDetails.append(e.getMessage() + "\n");
    resetAfterTransfer();
  }

  private void onException(Exception exception) {
    Log.e(TAG, exception.toString(), exception);
    StringWriter sw = new StringWriter();
    PrintWriter pw = new PrintWriter(sw);
    exception.printStackTrace(pw);
    runOnUiThread(
        () -> {
          transferDetails.append("Exception:\n");
          transferDetails.append(sw.toString());
          Toast.makeText(SpeedTestActivity.this, "Exception occurred", Toast.LENGTH_LONG).show();
        });
    resetAfterTransfer();
  }

  private void onRequestSuccess(int status, Map<String, List<String>> responseHeaders) {
    // Don't do anything until the final request finishes.
    synchronized (lock) {
      if (bytesCompleted < bytesTotal) {
        return;
      }
    }
    runOnUiThread(
          () -> {
            bitrateIndication.setText(bitrateIndicationString(
                bytesCompleted, System.currentTimeMillis() - startTimeMillis));
          });

    String headersMsg = "";
    for (Map.Entry<String, List<String>> entry : responseHeaders.entrySet()) {
      for (String value : entry.getValue()) {
        headersMsg += entry.getKey() + "=" + value + "\n";
      }
    }
    String msg =
        "Total bytes: " + bytesCompleted + "\n"
        + "Response: " + status + "\n"
        + "Headers:\n"
        + headersMsg;
    log(msg, false);
    resetAfterTransfer();
  }

  // Cancels all ongoing requests; resets UI elements to be ready for another run.
  private void resetAfterTransfer() {
    if (cronetRequests != null) {
      for (int i = 0; i < cronetRequests.length; ++i) {
        UrlRequest request = cronetRequests[i];
        if (request != null) {
          executor.execute(() -> request.cancel());
        }
      }
      cronetRequests = null;
    }
    if (httpFutures != null) {
      for (int i = 0; i < httpFutures.length; ++i) {
        if (httpFutures[i] != null) {
          httpFutures[i].future.cancel(true);
        }
      }
      httpFutures = null;
    }
    runOnUiThread(
        () -> {
          cronetEnabledCheckbox.setEnabled(true);
          if (cronetEnabledCheckbox.isChecked()) {
            cronetProtocolH11.setEnabled(true);
            cronetProtocolH2.setEnabled(true);
            cronetProtocolQUIC.setEnabled(true);
          }
          uploadButton.setEnabled(true);
          downloadButton.setEnabled(true);
          cancelButton.setEnabled(false);
          progressBar.setProgress(0);
        });
  }

  private void onClickUploadOrDownload(boolean isUpload) {
    transferDetails.setText("");
    cronetEnabledCheckbox.setEnabled(false);
    cronetProtocolH11.setEnabled(false);
    cronetProtocolH2.setEnabled(false);
    cronetProtocolQUIC.setEnabled(false);
    uploadButton.setEnabled(false);
    downloadButton.setEnabled(false);
    cancelButton.setEnabled(true);
    CronetProtocol protocol = CronetProtocol.CRONET_H11;
    if (cronetProtocolH2.isChecked()) {
      protocol = CronetProtocol.CRONET_H2;
    } else if (cronetProtocolQUIC.isChecked()) {
      protocol = CronetProtocol.CRONET_QUIC;
    }

    resetBeforeTransfer();
    executor = Executors.newFixedThreadPool(numStreams);

    if (cronetEnabledCheckbox.isChecked()) {
      // Use Cronet
      CronetEngine engine;
      try {
        engine = createCronetEngine(protocol);
      } catch (MalformedURLException e) {
        onMalformedUrlException(e);
        return;
      }
      cronetRequests = new UrlRequest[numStreams];
      for (int i = 0; i < numStreams; ++i) {
        UrlRequest.Callback requestCallback = new CronetRequestCallback(isUpload);
        // Invoke callbacks on |executor|.
        UrlRequest.Builder requestBuilder =
            engine.newUrlRequestBuilder(makeUrl(i), requestCallback, executor);
        if (isUpload) {
          requestBuilder.setHttpMethod("POST");
          requestBuilder.addHeader("Content-Length", String.valueOf(uploadBytes));
          requestBuilder.setUploadDataProvider(
              new DummyUploadDataProvider(uploadBytes), executor);
          onRequestLengthKnown(uploadBytes);
        } else {
          requestBuilder.setHttpMethod("GET");
        }
        requestBuilder.addHeader("Content-Type", "application/x-www-form-urlencoded");

        UrlRequest request = requestBuilder.build();
        executor.execute(() -> request.start());
        cronetRequests[i] = request;
        log("Request " + i + " started.", true);
      }
    } else {
      // Use okhttp
      httpFutures = new FutureWrapper[numStreams];
      for (int i = 0; i < numStreams; ++i) {
        URL url;
        try {
          url = new URL(makeUrl(i));
        } catch (MalformedURLException e) {
          onMalformedUrlException(e);
          return;
        }
        httpFutures[i] =
            new FutureWrapper(executor.submit(new HttpRequest(url, isUpload, uploadBytes)));
      }
    }
  }

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_speedtest);

    // For scripted benchmarks, get configuration from bundle extras.
    Intent intent = getIntent();
    @Nullable Bundle bundle = intent.getExtras();
    @Nullable String host = null;
    @Nullable String startupProtocol = null;
    @Nullable String startupMode = null;
    numStreams = 1;
    uploadBytes = UPLOAD_BYTES;
    if (bundle != null) {
      quicConnectionOptions = bundle.getString(QUIC_CONNECTION_OPTIONS_KEY);
      quicClientConnectionOptions = bundle.getString(QUIC_CLIENT_CONNETION_OPTIONS_KEY);
      startupProtocol = bundle.getString(STARTUP_PROTOCOL_KEY);
      startupMode = bundle.getString(STARTUP_MODE_KEY);
      numStreams = bundle.getInt(NUM_STREAMS_KEY, /*defaultValue=*/1);
      uploadBytes = bundle.getLong(UPLOAD_BYTES_KEY, /*defaultValue=*/UPLOAD_BYTES);
      host = bundle.getString(HOST_KEY);
    }

    if (quicConnectionOptions != null) {
      Log.i(TAG, QUIC_CONNECTION_OPTIONS_KEY + ": " + quicConnectionOptions);
    }
    if (quicClientConnectionOptions != null) {
      Log.i(TAG, QUIC_CLIENT_CONNETION_OPTIONS_KEY + ": " + quicClientConnectionOptions);
    }
    hostEditText = (EditText) findViewById(R.id.host_field);
    cronetEnabledCheckbox = (CheckBox) findViewById(R.id.cronet_enabled);
    cronetProtocolH11 = (RadioButton) findViewById(R.id.cronet_protocol_h11);
    cronetProtocolH2 = (RadioButton) findViewById(R.id.cronet_protocol_h2);
    cronetProtocolQUIC = (RadioButton) findViewById(R.id.cronet_protocol_quic);
    transferDetails = (TextView) findViewById(R.id.transfer_details);
    uploadButton = (Button) findViewById(R.id.upload_button);
    downloadButton = (Button) findViewById(R.id.download_button);
    progressBar = (ProgressBar) findViewById(R.id.progress_bar);
    progressBar.setIndeterminate(false);
    progressBar.setMax(1000);
    bitrateIndication = (TextView) findViewById(R.id.bitrate_indication);
    cancelButton = (Button) findViewById(R.id.cancel_button);

    cronetEnabledCheckbox.setOnCheckedChangeListener(
        (buttonView, isChecked) -> {
          if (isChecked) {
            cronetProtocolH11.setEnabled(true);
            cronetProtocolH2.setEnabled(true);
            cronetProtocolQUIC.setEnabled(true);
          } else {
            cronetProtocolH11.setEnabled(false);
            cronetProtocolH2.setEnabled(false);
            cronetProtocolQUIC.setEnabled(false);
          }
        });

    uploadButton.setOnClickListener(
        (v) -> {
          onClickUploadOrDownload(true);
        });

    downloadButton.setOnClickListener(
        (v) -> {
          onClickUploadOrDownload(false);
        });

    cancelButton.setOnClickListener(
        (v) -> {
          resetAfterTransfer();
        });

    if (host != null) {
      hostEditText.setText(host, TextView.BufferType.EDITABLE);
    }
    Button buttonToClick = null;
    if (startupMode != null) {
      switch (startupMode) {
        case "upload":
          buttonToClick = uploadButton;
          break;
        case "download":
          buttonToClick = downloadButton;
          break;
        default:
          Log.e(TAG, "Unknown startup mode: " + startupMode);
          break;
      }
    }
    if (startupProtocol != null) {
      switch (startupProtocol) {
        case "okhttp":
          cronetEnabledCheckbox.performClick();
          if (buttonToClick != null) {
            buttonToClick.performClick();
          }
          break;
        case "http1.1":
          cronetProtocolH11.performClick();
          if (buttonToClick != null) {
            buttonToClick.performClick();
          }
          break;
        case "http2":
          cronetProtocolH2.performClick();
          if (buttonToClick != null) {
            buttonToClick.performClick();
          }
          break;
        case "quic":
          cronetProtocolQUIC.performClick();
          if (buttonToClick != null) {
            buttonToClick.performClick();
          }
          break;
        default:
          Log.e(TAG, "Unknown startup protocol: " + startupProtocol);
          break;
      }
    }
  }

  private class CronetRequestCallback extends org.chromium.net.UrlRequest.Callback {
    private final boolean isUpload;
    private final ByteBuffer byteBuffer;

    CronetRequestCallback(boolean isUpload) {
      this.isUpload = isUpload;
      byteBuffer = ByteBuffer.allocateDirect(32 * 1024);
    }

    @Override
    public void onRedirectReceived(
        UrlRequest request, UrlResponseInfo info, String newLocationUrl) throws Exception {
      onException(new Exception("Redirect received"));
    }

    @Override
    public void onResponseStarted(UrlRequest request, UrlResponseInfo info) {
      if (!isUpload) {
        long bytesExpected = -1;
        List<String> lengthHeaderValues = info.getAllHeaders().get("content-length");
        if (lengthHeaderValues != null && !lengthHeaderValues.isEmpty()) {
          bytesExpected = Long.parseLong(lengthHeaderValues.get(0));
        }
        onRequestLengthKnown(bytesExpected);
      }
      request.read(byteBuffer);  // Start read
    }

    @Override
    public void onReadCompleted(UrlRequest request, UrlResponseInfo info, ByteBuffer byteBuffer)
        throws Exception {
      long bytesRead = byteBuffer.position();
      byteBuffer.clear();
      request.read(byteBuffer);  // Start next read.
      if (!isUpload) {
        onBytesCompleted(bytesRead);
      }
    }

    @Override
    public void onSucceeded(UrlRequest request, UrlResponseInfo info) {
      onRequestSuccess(info.getHttpStatusCode(), info.getAllHeaders());
    }

    @Override
    public void onFailed(UrlRequest request, UrlResponseInfo info, CronetException error) {
      onException(error);
    }

    @Override
    public void onCanceled(UrlRequest request, UrlResponseInfo info) {
      onException(new Exception("Transfer cancelled"));
    }
  }

  private class HttpRequest implements Callable<Void> {
    private final boolean isUpload;
    private final URL url;

    private InputStream inputStream;
    long uploadBytes;
    private OutputStream outputStream;

    public HttpRequest(URL url, boolean isUpload, long uploadBytes) {
      this.isUpload = isUpload;
      this.url = url;
      this.uploadBytes = uploadBytes;
    }

    @Override
    public Void call() throws Exception {
      try {
        callImpl();
      } catch (IOException e) {
        onException(e);
      } finally {
        try {
          if (inputStream != null) {
            inputStream.close();
          }
          if (outputStream != null) {
            outputStream.close();
          }
        } catch (IOException e) {
          onException(e);
        }
      }
      return null;
    }

    private void callImpl() throws IOException {
      HttpURLConnection connection;
      connection = (HttpURLConnection) url.openConnection();
      connection.setDoInput(true);
      connection.setDoOutput(isUpload);
      connection.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");

      log("Request for " + url + " started.", true);

      if (isUpload) {
        onRequestLengthKnown(uploadBytes);

        connection.setRequestMethod("POST");
        connection.setFixedLengthStreamingMode(uploadBytes);

        outputStream = connection.getOutputStream();  // Fires off request

        final Random random = new Random();
        final int bufferLength = 1024 * 1024;
        final byte[] randomBuffer = new byte[bufferLength];
        random.nextBytes(randomBuffer);

        long remainingBytes = uploadBytes;
        while (remainingBytes > 0) {
          long bytesToWrite = Math.min(remainingBytes, (long) bufferLength);
          outputStream.write(randomBuffer, 0, (int) bytesToWrite);
          outputStream.flush();
          remainingBytes -= bytesToWrite;

          onBytesCompleted(bytesToWrite);
        }
      } else {
        connection.setRequestMethod("GET");

        inputStream = connection.getInputStream();  // Fires off request

        // Add number of bytes expected from this request to bytesTotal.
        long bytesExpected = -1;
        List<String> lengthHeaderValues = connection.getHeaderFields().get("content-length");
        if (lengthHeaderValues != null && !lengthHeaderValues.isEmpty()) {
          bytesExpected = Long.parseLong(lengthHeaderValues.get(0));
        }
        onRequestLengthKnown(bytesExpected);

        // Read response until end.
        byte[] buffer = new byte[32 * 1024];
        while (true) {
          int bytesRead = inputStream.read(buffer);
          if (bytesRead < 0) {
            break;
          }
          onBytesCompleted(bytesRead);
        }
      }

      int status = connection.getResponseCode();
      onRequestSuccess(status, connection.getHeaderFields());
    }
  }

  private String makeUrl(int streamNumber) {
    String host = hostEditText.getText().toString();
    return "https://" + host + ":" + (PORT + streamNumber) + "/uploads";
  }

  ExperimentalCronetEngine.Builder createCronetEngineBuilder() {
    return new ExperimentalCronetEngine.Builder(getApplicationContext());
  }

  private CronetEngine createCronetEngine(CronetProtocol protocol) throws MalformedURLException {
    ExperimentalCronetEngine.Builder cronetEngineBuilder = createCronetEngineBuilder();

    switch (protocol) {
      case CRONET_H11:
        Log.i(TAG, "Defaulting to http 1.1");
        cronetEngineBuilder.enableHttp2(false);
        cronetEngineBuilder.enableQuic(false);
        break;
      case CRONET_H2:
        Log.i(TAG, "Enabling H2");
        cronetEngineBuilder.enableHttp2(true);
        cronetEngineBuilder.enableQuic(false);
        break;
      case CRONET_QUIC:
        Log.i(TAG, "Enabling QUIC");
        cronetEngineBuilder.enableHttp2(true);
        cronetEngineBuilder.enableQuic(true);
        break;
    }

    // SDCH (Shared Dictionary Compression for HTTP) adds some overhead and likely won't benefit
    // us here.
    cronetEngineBuilder.enableSdch(false);

    // This allows Cronet to assume a host is QUIC-capable without first connecting with TCP and
    // noticing an "Alternate-Protocol: quic:PORT" HTTP header.
    URL hint = new URL(makeUrl(0));
    cronetEngineBuilder.addQuicHint(hint.getHost(), hint.getPort(), hint.getPort());
    cronetEngineBuilder.addQuicHint(hint.getHost(), hint.getPort() + 1, hint.getPort() + 1);
    cronetEngineBuilder.addQuicHint(hint.getHost(), hint.getPort() + 2, hint.getPort() + 2);
    cronetEngineBuilder.addQuicHint(hint.getHost(), hint.getPort() + 3, hint.getPort() + 3);

    String cronetExperimentalOptions = "";
    if (quicConnectionOptions != null || quicClientConnectionOptions != null) {
      cronetExperimentalOptions = "{\"QUIC\": {\"";
      if (quicConnectionOptions != null) {
        cronetExperimentalOptions += "connection_options\": \"";
        cronetExperimentalOptions += quicConnectionOptions;
        if (quicClientConnectionOptions != null) {
          cronetExperimentalOptions += "\", \"";
        }
      }
      if (quicClientConnectionOptions != null) {
        cronetExperimentalOptions += "client_connection_options\": \"";
        cronetExperimentalOptions += quicClientConnectionOptions;
      }
      cronetExperimentalOptions += "\"}}";
      Log.i(TAG, cronetExperimentalOptions);
    }
    cronetEngineBuilder.setExperimentalOptions(cronetExperimentalOptions);
    return cronetEngineBuilder.build();
  }
}
