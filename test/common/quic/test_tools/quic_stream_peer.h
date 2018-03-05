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

#ifndef GFE_QUIC_TEST_TOOLS_QUIC_STREAM_PEER_H_
#define GFE_QUIC_TEST_TOOLS_QUIC_STREAM_PEER_H_

#include <cstdint>

#include "base/macros.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_stream_send_buffer.h"
#include "common/quic/core/quic_stream_sequencer.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {

class QuicStream;
class QuicSession;

namespace test {

class QuicStreamPeer {
 public:
  static void SetWriteSideClosed(bool value, QuicStream* stream);
  static void SetStreamBytesWritten(QuicStreamOffset stream_bytes_written,
                                    QuicStream* stream);
  static bool read_side_closed(QuicStream* stream);
  static void CloseReadSide(QuicStream* stream);

  static bool FinSent(QuicStream* stream);
  static bool RstSent(QuicStream* stream);

  static uint32_t SizeOfQueuedData(QuicStream* stream);

  static bool StreamContributesToConnectionFlowControl(QuicStream* stream);

  static void WriteOrBufferData(
      QuicStream* stream,
      QuicStringPiece data,
      bool fin,
      QuicReferenceCountedPointer<QuicAckListenerInterface> ack_listener);

  static QuicStreamSequencer* sequencer(QuicStream* stream);
  static QuicSession* session(QuicStream* stream);

  static QuicStreamSendBuffer& SendBuffer(QuicStream* stream);

  static void set_ack_listener(
      QuicStream* stream,
      QuicReferenceCountedPointer<QuicAckListenerInterface> ack_listener);

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicStreamPeer);
};

}  // namespace test

}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_QUIC_STREAM_PEER_H_
