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

#ifndef GFE_QUIC_TEST_TOOLS_SIMULATOR_TRAFFIC_POLICER_H_
#define GFE_QUIC_TEST_TOOLS_SIMULATOR_TRAFFIC_POLICER_H_

#include "common/quic/platform/api/quic_containers.h"
#include "test/common/quic/test_tools/simulator/packet_filter.h"
#include "test/common/quic/test_tools/simulator/port.h"

namespace gfe_quic {
namespace simulator {

// Traffic policer uses a token bucket to limit the bandwidth of the traffic
// passing through.  It wraps around an input port and exposes an output port.
// Only the traffic from input to the output is policed, so in case when
// bidirectional policing is desired, two policers have to be used.  The flows
// are hashed by the destination only.
class TrafficPolicer : public PacketFilter {
 public:
  TrafficPolicer(Simulator* simulator,
                 string name,
                 QuicByteCount initial_bucket_size,
                 QuicByteCount max_bucket_size,
                 QuicBandwidth target_bandwidth,
                 Endpoint* input);
  ~TrafficPolicer() override;

 protected:
  bool FilterPacket(const Packet& packet) override;

 private:
  // Refill the token buckets with all the tokens that have been granted since
  // |last_refill_time_|.
  void Refill();

  QuicByteCount initial_bucket_size_;
  QuicByteCount max_bucket_size_;
  QuicBandwidth target_bandwidth_;

  // The time at which the token buckets were last refilled.
  QuicTime last_refill_time_;

  // Maps each destination to the number of tokens it has left.
  QuicUnorderedMap<string, QuicByteCount> token_buckets_;

  DISALLOW_COPY_AND_ASSIGN(TrafficPolicer);
};

}  // namespace simulator
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_SIMULATOR_TRAFFIC_POLICER_H_
