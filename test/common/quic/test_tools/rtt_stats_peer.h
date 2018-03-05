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

#ifndef GFE_QUIC_TEST_TOOLS_RTT_STATS_PEER_H_
#define GFE_QUIC_TEST_TOOLS_RTT_STATS_PEER_H_

#include "base/macros.h"
#include "common/quic/core/congestion_control/rtt_stats.h"
#include "common/quic/core/quic_time.h"

namespace gfe_quic {
namespace test {

class RttStatsPeer {
 public:
  static void SetSmoothedRtt(RttStats* rtt_stats, QuicTime::Delta rtt_ms);

  static void SetMinRtt(RttStats* rtt_stats, QuicTime::Delta rtt_ms);

 private:
  DISALLOW_COPY_AND_ASSIGN(RttStatsPeer);
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_RTT_STATS_PEER_H_
