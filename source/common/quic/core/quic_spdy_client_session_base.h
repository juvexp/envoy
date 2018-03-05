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

#ifndef GFE_QUIC_CORE_QUIC_SPDY_CLIENT_SESSION_BASE_H_
#define GFE_QUIC_CORE_QUIC_SPDY_CLIENT_SESSION_BASE_H_

#include "base/macros.h"
#include "common/quic/core/quic_crypto_client_stream.h"
#include "common/quic/core/quic_spdy_session.h"
#include "common/quic/platform/api/quic_containers.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {

class QuicClientPromisedInfo;
class QuicClientPushPromiseIndex;
class QuicSpdyClientStream;

// For client/http layer code. Lookup promised streams based on
// matching promised request url. The same map can be shared across
// multiple sessions, since cross-origin pushes are allowed (subject
// to authority constraints).  Clients should use this map to enforce
// session affinity for requests corresponding to cross-origin push
// promised streams.
using QuicPromisedByUrlMap =
    QuicUnorderedMap<QuicString, QuicClientPromisedInfo*>;

// The maximum time a promises stream can be reserved without being
// claimed by a client request.
const int64_t kPushPromiseTimeoutSecs = 60;

// Base class for all client-specific QuicSession subclasses.
class QUIC_EXPORT_PRIVATE QuicSpdyClientSessionBase
    : public QuicSpdySession,
      public QuicCryptoClientStream::ProofHandler {
 public:
  // Takes ownership of |connection|. Caller retains ownership of
  // |promised_by_url|.
  QuicSpdyClientSessionBase(QuicConnection* connection,
                            QuicClientPushPromiseIndex* push_promise_index,
                            const QuicConfig& config);

  ~QuicSpdyClientSessionBase() override;

  void OnConfigNegotiated() override;

  // Override base class to set FEC policy before any data is sent by client.
  void OnCryptoHandshakeEvent(CryptoHandshakeEvent event) override;

  // Called by |headers_stream_| when push promise headers have been
  // completely received.
  void OnPromiseHeaderList(QuicStreamId stream_id,
                           QuicStreamId promised_stream_id,
                           size_t frame_len,
                           const QuicHeaderList& header_list) override;

  // Called by |QuicSpdyClientStream| on receipt of response headers,
  // needed to detect promised server push streams, as part of
  // client-request to push-stream rendezvous.
  void OnInitialHeadersComplete(
      QuicStreamId stream_id,
      const gfe_spdy::SpdyHeaderBlock& response_headers);

  // Called by |QuicSpdyClientStream| on receipt of PUSH_PROMISE, does
  // some session level validation and creates the
  // |QuicClientPromisedInfo| inserting into maps by (promised) id and
  // url. Returns true if a new push promise is accepted. Resets the promised
  // stream and returns false otherwise.
  virtual bool HandlePromised(QuicStreamId associated_id,
                              QuicStreamId promised_id,
                              const gfe_spdy::SpdyHeaderBlock& headers);

  // For cross-origin server push, this should verify the server is
  // authoritative per [RFC2818], Section 3.  Roughly, subjectAltName
  // list in the certificate should contain a matching DNS name, or IP
  // address.  |hostname| is derived from the ":authority" header field of
  // the PUSH_PROMISE frame, port if present there will be dropped.
  virtual bool IsAuthorized(const QuicString& hostname) = 0;

  // Session retains ownership.
  QuicClientPromisedInfo* GetPromisedByUrl(const QuicString& url);
  // Session retains ownership.
  QuicClientPromisedInfo* GetPromisedById(const QuicStreamId id);

  //
  QuicSpdyStream* GetPromisedStream(const QuicStreamId id);

  // Removes |promised| from the maps by url.
  void ErasePromisedByUrl(QuicClientPromisedInfo* promised);

  // Removes |promised| from the maps by url and id and destroys
  // promised.
  virtual void DeletePromised(QuicClientPromisedInfo* promised);

  virtual void OnPushStreamTimedOut(QuicStreamId stream_id);

  // Sends Rst for the stream, and makes sure that future calls to
  // IsClosedStream(id) return true, which ensures that any subsequent
  // frames related to this stream will be ignored (modulo flow
  // control accounting).
  void ResetPromised(QuicStreamId id, QuicRstStreamErrorCode error_code);

  // Release headers stream's sequencer buffer if it's empty.
  void CloseStreamInner(QuicStreamId stream_id, bool locally_reset) override;

  // Returns true if there are no active requests and no promised streams.
  bool ShouldReleaseHeadersStreamSequencerBuffer() override;

  size_t get_max_promises() const {
    return max_open_incoming_streams() * kMaxPromisedStreamsMultiplier;
  }

  QuicClientPushPromiseIndex* push_promise_index() {
    return push_promise_index_;
  }

 private:
  // For QuicSpdyClientStream to detect that a response corresponds to a
  // promise.
  using QuicPromisedByIdMap =
      QuicUnorderedMap<QuicStreamId, std::unique_ptr<QuicClientPromisedInfo>>;

  // As per rfc7540, section 10.5: track promise streams in "reserved
  // (remote)".  The primary key is URL from the promise request
  // headers.  The promised stream id is a secondary key used to get
  // promise info when the response headers of the promised stream
  // arrive.
  QuicClientPushPromiseIndex* push_promise_index_;
  QuicPromisedByIdMap promised_by_id_;
  QuicStreamId largest_promised_stream_id_;

  DISALLOW_COPY_AND_ASSIGN(QuicSpdyClientSessionBase);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_SPDY_CLIENT_SESSION_BASE_H_
