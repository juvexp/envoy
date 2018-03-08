// Copyright 2018 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Based on //depot/google3/uploader/tutorial/simple_agent.cc

#include <string>
#include <vector>  // IWYU pragma: keep

#include "base/callback.h"
#include "base/commandlineflags.h"
#include "base/init_google.h"
#include "base/logging.h"
#include "blobstore2/public/blob_client.h"  // IWYU pragma: keep
#include "file/base/file.h"                 // IWYU pragma: keep
#include "file/base/filesystem.h"           // IWYU pragma: keep
#include "net/base/selectserver.h"
#include "net/http/httpserver.h"
#include "net/rpc2/rpc2.h"
#include "strings/split.h"  // IWYU pragma: keep
#include "third_party/absl/strings/str_cat.h"
#include "uploader/agent/scotty_agent.proto.h"
#include "util/task/status.h"  // IWYU pragma: keep
#include "webutil/http/httpresponse.h"

DEFINE_int32(port, 10001, "The port on which to listen for RPCs.");

namespace uploader_tutorial {

// This using declaration is for the drop target part of the codelab.
using uploader_service::AuthenticationInfo;  // NOLINT
using uploader_service::DropTarget_Blobstore2;  // NOLINT
using uploader_service::HttpRequestInfo;
using uploader_service::HttpRequestInfo_Payload;  // NOLINT
using uploader_service::HttpResponseInfo;
using uploader_service::HttpResponseInfo_CompositePayload;
using uploader_service::HttpResponseInfo_Payload;
using uploader_service::RequestReceivedParameters;
using uploader_service::RequestReceivedResult;
using uploader_service::ResponseSentParameters;
using uploader_service::ResponseSentResult;
using uploader_service::ScottyAgent;


class SimpleAgent : public ScottyAgent {
 public:
  // Implement ScottyAgent functions.
  void OnRequestReceived(
      RPC* rpc,
      const RequestReceivedParameters* params,
      RequestReceivedResult* result,
      Closure* callback) override;

  void OnResponseSent(
      RPC* rpc,
      const ResponseSentParameters* params,
      ResponseSentResult* result,
      Closure* callback) override;
};

static void ProcessFileAndRespond(
    const HttpRequestInfo& request,
    HttpResponseInfo* response) {
  // CODELAB STEP 2: Upload Handler
  // Just using the default...
  for (int i = 0; i < request.payload_size(); i++) {
    const HttpRequestInfo_Payload& payload = request.payload(i);
    if (payload.has_bs2_info()) {
      response->mutable_payload()->set_data(
          absl::StrCat("<html><body><p>Your upload is located at ",
                       payload.bs2_info().blob_id(), "</p></body></html>"));
    }
  }
  response->set_http_status(HTTPResponse::RC_REQUEST_OK);
}

static void HandleDownloadRequest(RequestReceivedResult* result) {
  HttpResponseInfo* response = result->mutable_response();
  response->set_http_status(HTTPResponse::RC_REQUEST_OK);
  uploader_service::KeyValue* header = response->add_header();
  header->set_key("Content-Type");
  header->set_value("text/plain");
  // Use composite payload since a single payload's |arbitrary_bytes| has an
  // upper limit. This results in chunked encoding.
  const int NUM_PAYLOADS = 50;
  const int64_t SIZE_PER_PAYLOAD = 100LL * 1000000LL;  // 100 MB
  HttpResponseInfo_CompositePayload* composite_payload =
      response->mutable_payload()->mutable_composite_payload();
  for (int i = 0; i < NUM_PAYLOADS; ++i) {
    HttpResponseInfo_Payload* payload = composite_payload->add_payload();
    payload->set_arbitrary_bytes(SIZE_PER_PAYLOAD);
    payload->set_size(SIZE_PER_PAYLOAD);
  }
}

void SimpleAgent::OnRequestReceived(
    RPC* rpc,
    const RequestReceivedParameters* params,
    RequestReceivedResult* result,
    Closure* callback) {
  AutoClosureRunner auto_callback_runner(callback);

  LOG(INFO) << "Received notification:\n" << params->DebugString();

  // CODELAB STEP 4: Drop Target Handler

  // The agent can receive multiple calls to this, but for this agent we're
  // only interested when the upload is complete.
  // An agent should always set the response on the final call.
  if (!params->has_final_status()) {
    return;
  }

  // CODELAB DOWNLOAD
  // Handle GET requests.
  if (params->request().method() == "GET") {
    HandleDownloadRequest(result);
    return;
  }

  string text;
  switch (params->final_status()) {
    case RequestReceivedParameters::OK:
      // CODELAB STEP 6: Authentication
      // (none for upload_test)
      ProcessFileAndRespond(params->request(), result->mutable_response());
      // CODELAB STEP 5: Drop Target
      // (none for upload_test)
      return;
    case RequestReceivedParameters::SERVICE_FAILURE:
      result->mutable_response()->set_http_status(
          HTTPResponse::RC_SERVICE_UNAV);
      text = "Service is unavailable.";
      break;

    case RequestReceivedParameters::BROKEN_CONNECTION:
      result->mutable_response()->set_http_status(HTTPResponse::RC_REQUEST_TO);
      text = "Connection was reset. Please try again.";
      break;

    case RequestReceivedParameters::BAD_REQUEST:
      result->mutable_response()->set_http_status(HTTPResponse::RC_BAD_REQUEST);
      text = "Request was malformed.";
      break;

    case RequestReceivedParameters::ENTITY_TOO_BIG:
      result->mutable_response()->set_http_status(
          HTTPResponse::RC_ENTITY_TOO_BIG);
      text = "Upload size is too large. Maximum file size is 5GB.";
      break;

    default:
      result->mutable_response()->set_http_status(HTTPResponse::RC_BAD_REQUEST);
      break;
  }

  result->mutable_response()->mutable_payload()->set_data(
      "<html><body>"
      "<p>" + text + "</p>"
      "</body></html>");
}


void SimpleAgent::OnResponseSent(
    RPC* rpc,
    const ResponseSentParameters* params,
    ResponseSentResult* result,
    Closure* callback) {
  // Nothing for us to do here.
  result->CheckInitialized();
  callback->Run();
}

}  // namespace uploader_tutorial


int main(int argc, char* argv[]) {
  InitGoogle(argv[0], &argc, &argv, true);
  net_base::SelectServer ss;
  HTTPServer http(&ss, FLAGS_port, "SimpleAgent");
  uploader_tutorial::SimpleAgent agent;
  rpc2::EnableRPC2(&agent, rpc2::ServiceParameters());
  agent.ExportService(&http);
  LOG(INFO) << "ScottyAgent is now running on port " << FLAGS_port;
  ss.Loop();
  return 0;
}
