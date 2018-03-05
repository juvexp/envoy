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

#include "test/common/quic/test_tools/quic_stream_sequencer_peer.h"

#include "common/quic/core/quic_stream_sequencer.h"
#include "test/common/quic/test_tools/quic_stream_sequencer_buffer_peer.h"

namespace gfe_quic {
namespace test {

// static
size_t QuicStreamSequencerPeer::GetNumBufferedBytes(
    QuicStreamSequencer* sequencer) {
  return sequencer->buffered_frames_.BytesBuffered();
}

// static
QuicStreamOffset QuicStreamSequencerPeer::GetCloseOffset(
    QuicStreamSequencer* sequencer) {
  return sequencer->close_offset_;
}

// static
bool QuicStreamSequencerPeer::IsUnderlyingBufferAllocated(
    QuicStreamSequencer* sequencer) {
  QuicStreamSequencerBufferPeer buffer_peer(&(sequencer->buffered_frames_));
  return buffer_peer.IsBufferAllocated();
}

// static
void QuicStreamSequencerPeer::SetFrameBufferTotalBytesRead(
    QuicStreamSequencer* sequencer,
    QuicStreamOffset total_bytes_read) {
  QuicStreamSequencerBufferPeer buffer_peer(&(sequencer->buffered_frames_));
  buffer_peer.set_total_bytes_read(total_bytes_read);
}
}  // namespace test
}  // namespace gfe_quic
