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

#ifndef GFE_QUIC_TEST_TOOLS_QUIC_STREAM_SEQUENCER_PEER_H_
#define GFE_QUIC_TEST_TOOLS_QUIC_STREAM_SEQUENCER_PEER_H_

#include "base/macros.h"
#include "common/quic/core/quic_packets.h"

namespace gfe_quic {

class QuicStreamSequencer;

namespace test {

class QuicStreamSequencerPeer {
 public:
  static size_t GetNumBufferedBytes(QuicStreamSequencer* sequencer);

  static QuicStreamOffset GetCloseOffset(QuicStreamSequencer* sequencer);

  static bool IsUnderlyingBufferAllocated(QuicStreamSequencer* sequencer);

  static void SetFrameBufferTotalBytesRead(QuicStreamSequencer* sequencer,
                                           QuicStreamOffset total_bytes_read);

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicStreamSequencerPeer);
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_QUIC_STREAM_SEQUENCER_PEER_H_
