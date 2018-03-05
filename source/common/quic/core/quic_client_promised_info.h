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

#ifndef GFE_QUIC_CORE_QUIC_CLIENT_PROMISED_INFO_H_
#define GFE_QUIC_CORE_QUIC_CLIENT_PROMISED_INFO_H_

#include <cstddef>

#include "common/quic/core/quic_alarm.h"
#include "common/quic/core/quic_client_push_promise_index.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_spdy_client_session_base.h"
#include "common/quic/core/quic_spdy_stream.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/spdy_utils/core/spdy_framer.h"

namespace gfe_quic {

namespace test {
class QuicClientPromisedInfoPeer;
}  // namespace test

// QuicClientPromisedInfo tracks the client state of a server push
// stream from the time a PUSH_PROMISE is received until rendezvous
// between the promised response and the corresponding client request
// is complete.
class QUIC_EXPORT_PRIVATE QuicClientPromisedInfo
    : public QuicClientPushPromiseIndex::TryHandle {
 public:
  // Interface to QuicSpdyClientStream
  QuicClientPromisedInfo(QuicSpdyClientSessionBase* session,
                         QuicStreamId id,
                         QuicString url);
  virtual ~QuicClientPromisedInfo();

  void Init();

  // Validate promise headers etc. Returns true if headers are valid.
  bool OnPromiseHeaders(const gfe_spdy::SpdyHeaderBlock& headers);

  // Store response, possibly proceed with final validation.
  void OnResponseHeaders(const gfe_spdy::SpdyHeaderBlock& headers);

  // Rendezvous between this promised stream and a client request that
  // has a matching URL.
  virtual QuicAsyncStatus HandleClientRequest(
      const gfe_spdy::SpdyHeaderBlock& headers,
      QuicClientPushPromiseIndex::Delegate* delegate);

  void Cancel() override;

  void Reset(QuicRstStreamErrorCode error_code);

  // Client requests are initially associated to promises by matching
  // URL in the client request against the URL in the promise headers,
  // uing the |promised_by_url| map.  The push can be cross-origin, so
  // the client should validate that the session is authoritative for
  // the promised URL.  If not, it should call |RejectUnauthorized|.
  QuicSpdyClientSessionBase* session() { return session_; }

  // If the promised response contains Vary header, then the fields
  // specified by Vary must match between the client request header
  // and the promise headers (see https://crbug.com//554220).  Vary
  // validation requires the response headers (for the actual Vary
  // field list), the promise headers (taking the role of the "cached"
  // request), and the client request headers.
  gfe_spdy::SpdyHeaderBlock* request_headers() { return &request_headers_; }

  gfe_spdy::SpdyHeaderBlock* response_headers() {
    return response_headers_.get();
  }

  // After validation, client will use this to access the pushed stream.

  QuicStreamId id() const { return id_; }

  const QuicString url() const { return url_; }

  // Return true if there's a request pending matching this push promise.
  bool is_validating() const { return client_request_delegate_ != nullptr; }

 private:
  friend class test::QuicClientPromisedInfoPeer;

  class CleanupAlarm : public QuicAlarm::Delegate {
   public:
    explicit CleanupAlarm(QuicClientPromisedInfo* promised)
        : promised_(promised) {}

    void OnAlarm() override;

    QuicClientPromisedInfo* promised_;
  };

  QuicAsyncStatus FinalValidation();

  QuicSpdyClientSessionBase* session_;
  QuicStreamId id_;
  QuicString url_;
  gfe_spdy::SpdyHeaderBlock request_headers_;
  std::unique_ptr<gfe_spdy::SpdyHeaderBlock> response_headers_;
  gfe_spdy::SpdyHeaderBlock client_request_headers_;
  QuicClientPushPromiseIndex::Delegate* client_request_delegate_;

  // The promise will commit suicide eventually if it is not claimed by a GET
  // first.
  std::unique_ptr<QuicAlarm> cleanup_alarm_;

  DISALLOW_COPY_AND_ASSIGN(QuicClientPromisedInfo);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_CLIENT_PROMISED_INFO_H_
