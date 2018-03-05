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

#include "test/common/quic/test_tools/simulator/traffic_policer.h"

#include <algorithm>

namespace gfe_quic {
namespace simulator {

TrafficPolicer::TrafficPolicer(Simulator* simulator,
                               string name,
                               QuicByteCount initial_bucket_size,
                               QuicByteCount max_bucket_size,
                               QuicBandwidth target_bandwidth,
                               Endpoint* input)
    : PacketFilter(simulator, name, input),
      initial_bucket_size_(initial_bucket_size),
      max_bucket_size_(max_bucket_size),
      target_bandwidth_(target_bandwidth),
      last_refill_time_(clock_->Now()) {}

TrafficPolicer::~TrafficPolicer() {}

void TrafficPolicer::Refill() {
  QuicTime::Delta time_passed = clock_->Now() - last_refill_time_;
  QuicByteCount refill_size = time_passed * target_bandwidth_;

  for (auto& bucket : token_buckets_) {
    bucket.second = std::min(bucket.second + refill_size, max_bucket_size_);
  }

  last_refill_time_ = clock_->Now();
}

bool TrafficPolicer::FilterPacket(const Packet& packet) {
  // Refill existing buckets.
  Refill();

  // Create a new bucket if one does not exist.
  if (token_buckets_.count(packet.destination) == 0) {
    token_buckets_.insert(
        std::make_pair(packet.destination, initial_bucket_size_));
  }

  auto bucket = token_buckets_.find(packet.destination);
  DCHECK(bucket != token_buckets_.end());

  // Silently drop the packet on the floor if out of tokens
  if (bucket->second < packet.size) {
    return false;
  }

  bucket->second -= packet.size;
  return true;
}

}  // namespace simulator
}  // namespace gfe_quic
