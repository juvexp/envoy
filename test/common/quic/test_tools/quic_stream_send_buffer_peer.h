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

#ifndef GFE_QUIC_TEST_TOOLS_QUIC_STREAM_SEND_BUFFER_PEER_H_
#define GFE_QUIC_TEST_TOOLS_QUIC_STREAM_SEND_BUFFER_PEER_H_

#include "common/quic/core/quic_stream_send_buffer.h"

namespace gfe_quic {

namespace test {

class QuicStreamSendBufferPeer {
 public:
  static void SetStreamOffset(QuicStreamSendBuffer* send_buffer,
                              QuicStreamOffset stream_offset);

  static const BufferedSlice* CurrentWriteSlice(
      QuicStreamSendBuffer* send_buffer);

  static QuicByteCount TotalLength(QuicStreamSendBuffer* send_buffer);
};

}  // namespace test

}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_QUIC_STREAM_SEND_BUFFER_PEER_H_
