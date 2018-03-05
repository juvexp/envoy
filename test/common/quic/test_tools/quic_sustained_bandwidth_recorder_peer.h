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

#ifndef GFE_QUIC_TEST_TOOLS_QUIC_SUSTAINED_BANDWIDTH_RECORDER_PEER_H_
#define GFE_QUIC_TEST_TOOLS_QUIC_SUSTAINED_BANDWIDTH_RECORDER_PEER_H_

#include <cstdint>

#include "base/macros.h"
#include "common/quic/core/quic_packets.h"

namespace gfe_quic {

class QuicSustainedBandwidthRecorder;

namespace test {

class QuicSustainedBandwidthRecorderPeer {
 public:
  static void SetBandwidthEstimate(
      QuicSustainedBandwidthRecorder* bandwidth_recorder,
      int32_t bandwidth_estimate_kbytes_per_second);

  static void SetMaxBandwidthEstimate(
      QuicSustainedBandwidthRecorder* bandwidth_recorder,
      int32_t max_bandwidth_estimate_kbytes_per_second,
      int32_t max_bandwidth_timestamp);

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicSustainedBandwidthRecorderPeer);
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_QUIC_SUSTAINED_BANDWIDTH_RECORDER_PEER_H_
