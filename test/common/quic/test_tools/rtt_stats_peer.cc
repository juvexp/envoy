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

#include "test/common/quic/test_tools/rtt_stats_peer.h"

namespace gfe_quic {
namespace test {

// static
void RttStatsPeer::SetSmoothedRtt(RttStats* rtt_stats, QuicTime::Delta rtt_ms) {
  rtt_stats->smoothed_rtt_ = rtt_ms;
}

// static
void RttStatsPeer::SetMinRtt(RttStats* rtt_stats, QuicTime::Delta rtt_ms) {
  rtt_stats->min_rtt_ = rtt_ms;
}

}  // namespace test
}  // namespace gfe_quic
