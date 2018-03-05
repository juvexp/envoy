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

#ifndef GFE_QUIC_TEST_TOOLS_SIMPLE_QUIC_FRAMER_H_
#define GFE_QUIC_TEST_TOOLS_SIMPLE_QUIC_FRAMER_H_

#include <memory>
#include <vector>

#include "base/macros.h"
#include "common/quic/core/quic_framer.h"
#include "common/quic/core/quic_packets.h"

namespace gfe_quic {

struct QuicAckFrame;

namespace test {

class SimpleFramerVisitor;

// Peer to make public a number of otherwise private QuicFramer methods.
class SimpleQuicFramer {
 public:
  SimpleQuicFramer();
  explicit SimpleQuicFramer(const ParsedQuicVersionVector& supported_versions);
  SimpleQuicFramer(const ParsedQuicVersionVector& supported_versions,
                   Perspective perspective);
  ~SimpleQuicFramer();

  bool ProcessPacket(const QuicEncryptedPacket& packet);
  void Reset();

  const QuicPacketHeader& header() const;
  size_t num_frames() const;
  const std::vector<QuicAckFrame>& ack_frames() const;
  const std::vector<QuicConnectionCloseFrame>& connection_close_frames() const;
  const std::vector<QuicStopWaitingFrame>& stop_waiting_frames() const;
  const std::vector<QuicPingFrame>& ping_frames() const;
  const std::vector<QuicWindowUpdateFrame>& window_update_frames() const;
  const std::vector<QuicGoAwayFrame>& goaway_frames() const;
  const std::vector<QuicRstStreamFrame>& rst_stream_frames() const;
  const std::vector<std::unique_ptr<QuicStreamFrame>>& stream_frames() const;
  const std::vector<QuicPaddingFrame>& padding_frames() const;
  const QuicVersionNegotiationPacket* version_negotiation_packet() const;

  QuicFramer* framer();

  void SetSupportedVersions(const ParsedQuicVersionVector& versions) {
    framer_.SetSupportedVersions(versions);
  }

 private:
  QuicFramer framer_;
  std::unique_ptr<SimpleFramerVisitor> visitor_;
  DISALLOW_COPY_AND_ASSIGN(SimpleQuicFramer);
};

}  // namespace test

}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_SIMPLE_QUIC_FRAMER_H_
