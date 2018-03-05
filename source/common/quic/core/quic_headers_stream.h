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

#ifndef GFE_QUIC_CORE_QUIC_HEADERS_STREAM_H_
#define GFE_QUIC_CORE_QUIC_HEADERS_STREAM_H_

#include <cstddef>
#include <memory>

#include "base/macros.h"
#include "common/quic/core/quic_header_list.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_stream.h"
#include "common/quic/platform/api/quic_containers.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/spdy_utils/core/spdy_framer.h"

namespace gfe_quic {

class QuicSpdySession;

namespace test {
class QuicHeadersStreamPeer;
}  // namespace test

// Headers in QUIC are sent as HTTP/2 HEADERS or PUSH_PROMISE frames over a
// reserved stream with the id 3.  Each endpoint (client and server) will
// allocate an instance of QuicHeadersStream to send and receive headers.
class QUIC_EXPORT_PRIVATE QuicHeadersStream : public QuicStream {
 public:
  explicit QuicHeadersStream(QuicSpdySession* session);
  ~QuicHeadersStream() override;

  // QuicStream implementation
  void OnDataAvailable() override;

  // Release underlying buffer if allowed.
  void MaybeReleaseSequencerBuffer();

  bool OnStreamFrameAcked(QuicStreamOffset offset,
                          QuicByteCount data_length,
                          bool fin_acked,
                          QuicTime::Delta ack_delay_time) override;

  void OnStreamFrameRetransmitted(QuicStreamOffset offset,
                                  QuicByteCount data_length,
                                  bool fin_retransmitted) override;

 private:
  friend class test::QuicHeadersStreamPeer;

  // CompressedHeaderInfo includes simple information of a header, including
  // offset in headers stream, unacked length and ack listener of this header.
  struct CompressedHeaderInfo {
    CompressedHeaderInfo(
        QuicStreamOffset headers_stream_offset,
        QuicStreamOffset full_length,
        QuicReferenceCountedPointer<QuicAckListenerInterface> ack_listener);
    CompressedHeaderInfo(const CompressedHeaderInfo& other);
    ~CompressedHeaderInfo();

    // Offset the header was sent on the headers stream.
    QuicStreamOffset headers_stream_offset;
    // The full length of the header.
    QuicByteCount full_length;
    // The remaining bytes to be acked.
    QuicByteCount unacked_length;
    // Ack listener of this header, and it is notified once any of the bytes has
    // been acked or retransmitted.
    QuicReferenceCountedPointer<QuicAckListenerInterface> ack_listener;
  };

  // Returns true if the session is still connected.
  bool IsConnected();

  // Override to store mapping from offset, length to ack_listener. This
  // ack_listener is notified once data within [offset, offset + length] is
  // acked or retransmitted.
  void OnDataBuffered(
      QuicStreamOffset offset,
      QuicByteCount data_length,
      const QuicReferenceCountedPointer<QuicAckListenerInterface>& ack_listener)
      override;

  QuicSpdySession* spdy_session_;

  // Headers that have not been fully acked.
  QuicDeque<CompressedHeaderInfo> unacked_headers_;

  DISALLOW_COPY_AND_ASSIGN(QuicHeadersStream);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_HEADERS_STREAM_H_
