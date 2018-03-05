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

#ifndef GFE_QUIC_TEST_TOOLS_QUIC_STREAM_SEQUENCER_BUFFER_PEER_H_
#define GFE_QUIC_TEST_TOOLS_QUIC_STREAM_SEQUENCER_BUFFER_PEER_H_

#include "common/quic/core/quic_stream_sequencer_buffer.h"

namespace gfe_quic {

namespace test {

class QuicStreamSequencerBufferPeer {
 public:
  explicit QuicStreamSequencerBufferPeer(QuicStreamSequencerBuffer* buffer);

  // Read from this buffer_ into the given destination buffer_ up to the
  // size of the destination. Returns the number of bytes read. Reading from
  // an empty buffer_->returns 0.
  size_t Read(char* dest_buffer, size_t size);

  // If buffer is empty, the blocks_ array must be empty, which means all
  // blocks are deallocated.
  bool CheckEmptyInvariants();

  bool IsBlockArrayEmpty();

  bool CheckInitialState();

  bool CheckBufferInvariants();

  size_t GetInBlockOffset(QuicStreamOffset offset);

  QuicStreamSequencerBuffer::BufferBlock* GetBlock(size_t index);

  int IntervalSize();

  size_t max_buffer_capacity();

  size_t ReadableBytes();

  std::map<QuicStreamOffset, QuicStreamSequencerBuffer::FrameInfo>*
  frame_arrival_time_map();

  void set_total_bytes_read(QuicStreamOffset total_bytes_read);

  void AddBytesReceived(QuicStreamOffset offset, QuicByteCount length);

  bool IsBufferAllocated();

  size_t block_count();

  const QuicIntervalSet<QuicStreamOffset>& bytes_received();

 private:
  QuicStreamSequencerBuffer* buffer_;
  DISALLOW_COPY_AND_ASSIGN(QuicStreamSequencerBufferPeer);
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_QUIC_STREAM_SEQUENCER_BUFFER_PEER_H_
