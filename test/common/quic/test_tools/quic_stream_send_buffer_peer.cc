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

#include "test/common/quic/test_tools/quic_stream_send_buffer_peer.h"

namespace gfe_quic {

namespace test {

// static
void QuicStreamSendBufferPeer::SetStreamOffset(
    QuicStreamSendBuffer* send_buffer,
    QuicStreamOffset stream_offset) {
  send_buffer->stream_offset_ = stream_offset;
}

// static
const BufferedSlice* QuicStreamSendBufferPeer::CurrentWriteSlice(
    QuicStreamSendBuffer* send_buffer) {
  if (send_buffer->write_index_ == -1) {
    return nullptr;
  }
  return &send_buffer->buffered_slices_[send_buffer->write_index_];
}

// static
QuicByteCount QuicStreamSendBufferPeer::TotalLength(
    QuicStreamSendBuffer* send_buffer) {
  QuicByteCount length = 0;
  for (const auto& slice : send_buffer->buffered_slices_) {
    length += slice.slice.length();
  }
  return length;
}

}  // namespace test

}  // namespace gfe_quic
