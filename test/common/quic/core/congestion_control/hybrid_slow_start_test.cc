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

#include "common/quic/core/congestion_control/hybrid_slow_start.h"

#include <memory>

#include "common/quic/platform/api/quic_ptr_util.h"
#include "test/common/quic/platform/api/quic_test.h"

namespace gfe_quic {
namespace test {

class HybridSlowStartTest : public QuicTest {
 protected:
  HybridSlowStartTest()
      : one_ms_(QuicTime::Delta::FromMilliseconds(1)),
        rtt_(QuicTime::Delta::FromMilliseconds(60)) {}
  void SetUp() override { slow_start_ = QuicMakeUnique<HybridSlowStart>(); }
  const QuicTime::Delta one_ms_;
  const QuicTime::Delta rtt_;
  std::unique_ptr<HybridSlowStart> slow_start_;
};

TEST_F(HybridSlowStartTest, Simple) {
  QuicPacketNumber packet_number = 1;
  QuicPacketNumber end_packet_number = 3;
  slow_start_->StartReceiveRound(end_packet_number);

  EXPECT_FALSE(slow_start_->IsEndOfRound(packet_number++));

  // Test duplicates.
  EXPECT_FALSE(slow_start_->IsEndOfRound(packet_number));

  EXPECT_FALSE(slow_start_->IsEndOfRound(packet_number++));
  EXPECT_TRUE(slow_start_->IsEndOfRound(packet_number++));

  // Test without a new registered end_packet_number;
  EXPECT_TRUE(slow_start_->IsEndOfRound(packet_number++));

  end_packet_number = 20;
  slow_start_->StartReceiveRound(end_packet_number);
  while (packet_number < end_packet_number) {
    EXPECT_FALSE(slow_start_->IsEndOfRound(packet_number++));
  }
  EXPECT_TRUE(slow_start_->IsEndOfRound(packet_number++));
}

TEST_F(HybridSlowStartTest, Delay) {
  // We expect to detect the increase at +1/8 of the RTT; hence at a typical
  // RTT of 60ms the detection will happen at 67.5 ms.
  const int kHybridStartMinSamples = 8;  // Number of acks required to trigger.

  QuicPacketNumber end_packet_number = 1;
  slow_start_->StartReceiveRound(end_packet_number++);

  // Will not trigger since our lowest RTT in our burst is the same as the long
  // term RTT provided.
  for (int n = 0; n < kHybridStartMinSamples; ++n) {
    EXPECT_FALSE(slow_start_->ShouldExitSlowStart(
        rtt_ + QuicTime::Delta::FromMilliseconds(n), rtt_, 100));
  }
  slow_start_->StartReceiveRound(end_packet_number++);
  for (int n = 1; n < kHybridStartMinSamples; ++n) {
    EXPECT_FALSE(slow_start_->ShouldExitSlowStart(
        rtt_ + QuicTime::Delta::FromMilliseconds(n + 10), rtt_, 100));
  }
  // Expect to trigger since all packets in this burst was above the long term
  // RTT provided.
  EXPECT_TRUE(slow_start_->ShouldExitSlowStart(
      rtt_ + QuicTime::Delta::FromMilliseconds(10), rtt_, 100));
}

}  // namespace test
}  // namespace gfe_quic
