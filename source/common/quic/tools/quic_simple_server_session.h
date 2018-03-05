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

// Toy server specific QuicSession subclass.

#ifndef GFE_QUIC_TOOLS_QUIC_SIMPLE_SERVER_SESSION_H_
#define GFE_QUIC_TOOLS_QUIC_SIMPLE_SERVER_SESSION_H_

#include <list>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/macros.h"
#include "common/quic/core/quic_crypto_server_stream.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_server_session_base.h"
#include "common/quic/core/quic_spdy_session.h"
#include "common/quic/platform/api/quic_containers.h"
#include "common/quic/tools/quic_http_response_cache.h"
#include "common/quic/tools/quic_simple_server_stream.h"

namespace gfe_quic {

namespace test {
class QuicSimpleServerSessionPeer;
}  // namespace test

class QuicSimpleServerSession : public QuicServerSessionBase {
 public:
  // A PromisedStreamInfo is an element of the queue to store promised
  // stream which hasn't been created yet. It keeps a mapping between promised
  // stream id with its priority and the headers sent out in PUSH_PROMISE.
  struct PromisedStreamInfo {
   public:
    PromisedStreamInfo(gfe_spdy::SpdyHeaderBlock request_headers,
                       QuicStreamId stream_id,
                       gfe_spdy::SpdyPriority priority)
        : request_headers(std::move(request_headers)),
          stream_id(stream_id),
          priority(priority),
          is_cancelled(false) {}
    gfe_spdy::SpdyHeaderBlock request_headers;
    QuicStreamId stream_id;
    gfe_spdy::SpdyPriority priority;
    bool is_cancelled;
  };

  // Takes ownership of |connection|.
  QuicSimpleServerSession(const QuicConfig& config,
                          QuicConnection* connection,
                          QuicSession::Visitor* visitor,
                          QuicCryptoServerStream::Helper* helper,
                          const QuicCryptoServerConfig* crypto_config,
                          QuicCompressedCertsCache* compressed_certs_cache,
                          QuicHttpResponseCache* response_cache);

  ~QuicSimpleServerSession() override;

  // When a stream is marked draining, it will decrease the number of open
  // streams. If it is an outgoing stream, try to open a new stream to send
  // remaing push responses.
  void StreamDraining(QuicStreamId id) override;

  // Override base class to detact client sending data on server push stream.
  void OnStreamFrame(const QuicStreamFrame& frame) override;

  // Send out PUSH_PROMISE for all |resources| promised stream id in each frame
  // will increase by 2 for each item in |resources|.
  // And enqueue HEADERS block in those PUSH_PROMISED for sending push response
  // later.
  virtual void PromisePushResources(
      const string& request_url,
      const std::list<QuicHttpResponseCache::ServerPushInfo>& resources,
      QuicStreamId original_stream_id,
      const gfe_spdy::SpdyHeaderBlock& original_request_headers);

 protected:
  // QuicSession methods:
  QuicSpdyStream* CreateIncomingDynamicStream(QuicStreamId id) override;
  QuicSimpleServerStream* CreateOutgoingDynamicStream() override;
  // Closing an outgoing stream can reduce open outgoing stream count, try
  // to handle queued promised streams right now.
  void CloseStreamInner(QuicStreamId stream_id, bool locally_reset) override;
  // Override to return true for locally preserved server push stream.
  void HandleFrameOnNonexistentOutgoingStream(QuicStreamId stream_id) override;
  // Override to handle reseting locally preserved streams.
  void HandleRstOnValidNonexistentStream(
      const QuicRstStreamFrame& frame) override;

  // QuicServerSessionBaseMethod:
  QuicCryptoServerStreamBase* CreateQuicCryptoServerStream(
      const QuicCryptoServerConfig* crypto_config,
      QuicCompressedCertsCache* compressed_certs_cache) override;

  QuicHttpResponseCache* response_cache() { return response_cache_; }

 private:
  friend class test::QuicSimpleServerSessionPeer;

  // Create a server push headers block by copying request's headers block.
  // But replace or add these pseudo-headers as they are specific to each
  // request:
  // :authority, :path, :method, :scheme, referer.
  // Copying the rest headers ensures they are the same as the original
  // request, especially cookies.
  gfe_spdy::SpdyHeaderBlock SynthesizePushRequestHeaders(
      string request_url,
      QuicHttpResponseCache::ServerPushInfo resource,
      const gfe_spdy::SpdyHeaderBlock& original_request_headers);

  // Send PUSH_PROMISE frame on headers stream.
  void SendPushPromise(QuicStreamId original_stream_id,
                       QuicStreamId promised_stream_id,
                       gfe_spdy::SpdyHeaderBlock headers);

  // Fetch response from cache for request headers enqueued into
  // promised_headers_and_streams_ and send them on dedicated stream until
  // reaches max_open_stream_ limit.
  // Called when return value of GetNumOpenOutgoingStreams() changes:
  //    CloseStreamInner();
  //    StreamDraining();
  // Note that updateFlowControlOnFinalReceivedByteOffset() won't change the
  // return value becasue all push streams are impossible to become locally
  // closed. Since a locally preserved stream becomes remotely closed after
  // HandlePromisedPushRequests() starts to process it, and if it is reset
  // locally afterwards, it will be immediately become closed and never get into
  // locally_closed_stream_highest_offset_. So all the streams in this map
  // are not outgoing streams.
  void HandlePromisedPushRequests();

  // Keep track of the highest stream id which has been sent in PUSH_PROMISE.
  QuicStreamId highest_promised_stream_id_;

  // Promised streams which hasn't been created yet because of max_open_stream_
  // limit. New element is added to the end of the queue.
  // Since outgoing stream is created in sequence, stream_id of each element in
  // the queue also increases by 2 from previous one's. The front element's
  // stream_id is always next_outgoing_stream_id_, and the last one is always
  // highest_promised_stream_id_.
  QuicDeque<PromisedStreamInfo> promised_streams_;

  QuicHttpResponseCache* response_cache_;  // Not owned.

  DISALLOW_COPY_AND_ASSIGN(QuicSimpleServerSession);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_TOOLS_QUIC_SIMPLE_SERVER_SESSION_H_
