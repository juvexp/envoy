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

#include "test/common/quic/test_tools/quic_sustained_bandwidth_recorder_peer.h"

#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_sustained_bandwidth_recorder.h"

namespace gfe_quic {
namespace test {

// static
void QuicSustainedBandwidthRecorderPeer::SetBandwidthEstimate(
    QuicSustainedBandwidthRecorder* bandwidth_recorder,
    int32_t bandwidth_estimate_kbytes_per_second) {
  bandwidth_recorder->has_estimate_ = true;
  bandwidth_recorder->bandwidth_estimate_ =
      QuicBandwidth::FromKBytesPerSecond(bandwidth_estimate_kbytes_per_second);
}

// static
void QuicSustainedBandwidthRecorderPeer::SetMaxBandwidthEstimate(
    QuicSustainedBandwidthRecorder* bandwidth_recorder,
    int32_t max_bandwidth_estimate_kbytes_per_second,
    int32_t max_bandwidth_timestamp) {
  bandwidth_recorder->max_bandwidth_estimate_ =
      QuicBandwidth::FromKBytesPerSecond(
          max_bandwidth_estimate_kbytes_per_second);
  bandwidth_recorder->max_bandwidth_timestamp_ = max_bandwidth_timestamp;
}

}  // namespace test
}  // namespace gfe_quic
