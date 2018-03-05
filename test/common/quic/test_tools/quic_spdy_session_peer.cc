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

#include "test/common/quic/test_tools/quic_spdy_session_peer.h"

#include "common/quic/core/quic_spdy_session.h"

namespace gfe_quic {
namespace test {

// static
QuicHeadersStream* QuicSpdySessionPeer::GetHeadersStream(
    QuicSpdySession* session) {
  return session->headers_stream_.get();
}

// static
void QuicSpdySessionPeer::SetHeadersStream(QuicSpdySession* session,
                                           QuicHeadersStream* headers_stream) {
  session->headers_stream_.reset(headers_stream);
  if (headers_stream != nullptr) {
    session->static_streams()[headers_stream->id()] = headers_stream;
  }
}

// static
const gfe_spdy::SpdyFramer& QuicSpdySessionPeer::GetSpdyFramer(
    QuicSpdySession* session) {
  return session->spdy_framer_;
}

void QuicSpdySessionPeer::SetHpackEncoderDebugVisitor(
    QuicSpdySession* session,
    std::unique_ptr<QuicHpackDebugVisitor> visitor) {
  session->SetHpackEncoderDebugVisitor(std::move(visitor));
}

void QuicSpdySessionPeer::SetHpackDecoderDebugVisitor(
    QuicSpdySession* session,
    std::unique_ptr<QuicHpackDebugVisitor> visitor) {
  session->SetHpackDecoderDebugVisitor(std::move(visitor));
}

void QuicSpdySessionPeer::SetMaxUncompressedHeaderBytes(
    QuicSpdySession* session,
    size_t set_max_uncompressed_header_bytes) {
  session->set_max_uncompressed_header_bytes(set_max_uncompressed_header_bytes);
}

// static
size_t QuicSpdySessionPeer::WriteHeadersImpl(
    QuicSpdySession* session,
    QuicStreamId id,
    gfe_spdy::SpdyHeaderBlock headers,
    bool fin,
    int weight,
    QuicStreamId parent_stream_id,
    bool exclusive,
    QuicReferenceCountedPointer<QuicAckListenerInterface> ack_listener) {
  return session->WriteHeadersImpl(id, std::move(headers), fin, weight,
                                   parent_stream_id, exclusive,
                                   std::move(ack_listener));
}

//  static
QuicStreamId QuicSpdySessionPeer::NextStreamId(const QuicSpdySession& session) {
  return 2;
}

//  static
QuicStreamId QuicSpdySessionPeer::GetNthClientInitiatedStreamId(
    const QuicSpdySession& session,
    int n) {
  return 5 + QuicSpdySessionPeer::NextStreamId(session) * n;
}

//  static
QuicStreamId QuicSpdySessionPeer::GetNthServerInitiatedStreamId(
    const QuicSpdySession& session,
    int n) {
  return 2 + QuicSpdySessionPeer::NextStreamId(session) * n;
}

}  // namespace test
}  // namespace gfe_quic
