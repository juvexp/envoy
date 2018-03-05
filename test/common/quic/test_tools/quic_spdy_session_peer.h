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

#ifndef GFE_QUIC_TEST_TOOLS_QUIC_SPDY_SESSION_PEER_H_
#define GFE_QUIC_TEST_TOOLS_QUIC_SPDY_SESSION_PEER_H_

#include "base/macros.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_write_blocked_list.h"
#include "common/quic/spdy_utils/core/spdy_framer.h"

namespace gfe_quic {

class QuicHeadersStream;
class QuicSpdySession;
class QuicHpackDebugVisitor;

namespace test {

class QuicSpdySessionPeer {
 public:
  static QuicHeadersStream* GetHeadersStream(QuicSpdySession* session);
  static void SetHeadersStream(QuicSpdySession* session,
                               QuicHeadersStream* headers_stream);
  static const gfe_spdy::SpdyFramer& GetSpdyFramer(QuicSpdySession* session);
  static void SetHpackEncoderDebugVisitor(
      QuicSpdySession* session,
      std::unique_ptr<QuicHpackDebugVisitor> visitor);
  static void SetHpackDecoderDebugVisitor(
      QuicSpdySession* session,
      std::unique_ptr<QuicHpackDebugVisitor> visitor);
  static void SetMaxUncompressedHeaderBytes(
      QuicSpdySession* session,
      size_t set_max_uncompressed_header_bytes);
  static size_t WriteHeadersImpl(
      QuicSpdySession* session,
      QuicStreamId id,
      gfe_spdy::SpdyHeaderBlock headers,
      bool fin,
      int weight,
      QuicStreamId parent_stream_id,
      bool exclusive,
      QuicReferenceCountedPointer<QuicAckListenerInterface> ack_listener);
  // Helper functions for stream ids, to allow test logic to abstract
  // over the HTTP stream numbering scheme (i.e. whether one or
  // two QUIC streams are used per HTTP transaction).
  static QuicStreamId NextStreamId(const QuicSpdySession& session);
  // n should start at 0.
  static QuicStreamId GetNthClientInitiatedStreamId(
      const QuicSpdySession& session,
      int n);
  // n should start at 0.
  static QuicStreamId GetNthServerInitiatedStreamId(
      const QuicSpdySession& session,
      int n);

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicSpdySessionPeer);
};

}  // namespace test

}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_QUIC_SPDY_SESSION_PEER_H_
