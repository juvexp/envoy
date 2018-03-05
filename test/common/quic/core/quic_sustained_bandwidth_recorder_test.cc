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

#include "common/quic/core/quic_sustained_bandwidth_recorder.h"

#include "common/quic/core/quic_bandwidth.h"
#include "common/quic/core/quic_time.h"
#include "test/common/quic/platform/api/quic_test.h"

namespace gfe_quic {
namespace test {
namespace {

class QuicSustainedBandwidthRecorderTest : public QuicTest {};

TEST_F(QuicSustainedBandwidthRecorderTest, BandwidthEstimates) {
  QuicSustainedBandwidthRecorder recorder;
  EXPECT_FALSE(recorder.HasEstimate());

  QuicTime estimate_time = QuicTime::Zero();
  QuicWallTime wall_time = QuicWallTime::Zero();
  QuicTime::Delta srtt = QuicTime::Delta::FromMilliseconds(150);
  const int kBandwidthBitsPerSecond = 12345678;
  QuicBandwidth bandwidth =
      QuicBandwidth::FromBitsPerSecond(kBandwidthBitsPerSecond);

  bool in_recovery = false;
  bool in_slow_start = false;

  // This triggers recording, but should not yield a valid estimate yet.
  recorder.RecordEstimate(in_recovery, in_slow_start, bandwidth, estimate_time,
                          wall_time, srtt);
  EXPECT_FALSE(recorder.HasEstimate());

  // Send a second reading, again this should not result in a valid estimate,
  // as not enough time has passed.
  estimate_time = estimate_time + srtt;
  recorder.RecordEstimate(in_recovery, in_slow_start, bandwidth, estimate_time,
                          wall_time, srtt);
  EXPECT_FALSE(recorder.HasEstimate());

  // Now 3 * kSRTT has elapsed since first recording, expect a valid estimate.
  estimate_time = estimate_time + srtt;
  estimate_time = estimate_time + srtt;
  recorder.RecordEstimate(in_recovery, in_slow_start, bandwidth, estimate_time,
                          wall_time, srtt);
  EXPECT_TRUE(recorder.HasEstimate());
  EXPECT_EQ(recorder.BandwidthEstimate(), bandwidth);
  EXPECT_EQ(recorder.BandwidthEstimate(), recorder.MaxBandwidthEstimate());

  // Resetting, and sending a different estimate will only change output after
  // a further 3 * kSRTT has passed.
  QuicBandwidth second_bandwidth =
      QuicBandwidth::FromBitsPerSecond(2 * kBandwidthBitsPerSecond);
  // Reset the recorder by passing in a measurement while in recovery.
  in_recovery = true;
  recorder.RecordEstimate(in_recovery, in_slow_start, bandwidth, estimate_time,
                          wall_time, srtt);
  in_recovery = false;
  recorder.RecordEstimate(in_recovery, in_slow_start, bandwidth, estimate_time,
                          wall_time, srtt);
  EXPECT_EQ(recorder.BandwidthEstimate(), bandwidth);

  estimate_time = estimate_time + 3 * srtt;
  const int64_t kSeconds = 556677;
  QuicWallTime second_bandwidth_wall_time =
      QuicWallTime::FromUNIXSeconds(kSeconds);
  recorder.RecordEstimate(in_recovery, in_slow_start, second_bandwidth,
                          estimate_time, second_bandwidth_wall_time, srtt);
  EXPECT_EQ(recorder.BandwidthEstimate(), second_bandwidth);
  EXPECT_EQ(recorder.BandwidthEstimate(), recorder.MaxBandwidthEstimate());
  EXPECT_EQ(recorder.MaxBandwidthTimestamp(), kSeconds);

  // Reset again, this time recording a lower bandwidth than before.
  QuicBandwidth third_bandwidth =
      QuicBandwidth::FromBitsPerSecond(0.5 * kBandwidthBitsPerSecond);
  // Reset the recorder by passing in an unreliable measurement.
  recorder.RecordEstimate(in_recovery, in_slow_start, third_bandwidth,
                          estimate_time, wall_time, srtt);
  recorder.RecordEstimate(in_recovery, in_slow_start, third_bandwidth,
                          estimate_time, wall_time, srtt);
  EXPECT_EQ(recorder.BandwidthEstimate(), third_bandwidth);

  estimate_time = estimate_time + 3 * srtt;
  recorder.RecordEstimate(in_recovery, in_slow_start, third_bandwidth,
                          estimate_time, wall_time, srtt);
  EXPECT_EQ(recorder.BandwidthEstimate(), third_bandwidth);

  // Max bandwidth should not have changed.
  EXPECT_LT(third_bandwidth, second_bandwidth);
  EXPECT_EQ(recorder.MaxBandwidthEstimate(), second_bandwidth);
  EXPECT_EQ(recorder.MaxBandwidthTimestamp(), kSeconds);
}

TEST_F(QuicSustainedBandwidthRecorderTest, SlowStart) {
  // Verify that slow start status is correctly recorded.
  QuicSustainedBandwidthRecorder recorder;
  EXPECT_FALSE(recorder.HasEstimate());

  QuicTime estimate_time = QuicTime::Zero();
  QuicWallTime wall_time = QuicWallTime::Zero();
  QuicTime::Delta srtt = QuicTime::Delta::FromMilliseconds(150);
  const int kBandwidthBitsPerSecond = 12345678;
  QuicBandwidth bandwidth =
      QuicBandwidth::FromBitsPerSecond(kBandwidthBitsPerSecond);

  bool in_recovery = false;
  bool in_slow_start = true;

  // This triggers recording, but should not yield a valid estimate yet.
  recorder.RecordEstimate(in_recovery, in_slow_start, bandwidth, estimate_time,
                          wall_time, srtt);

  // Now 3 * kSRTT has elapsed since first recording, expect a valid estimate.
  estimate_time = estimate_time + 3 * srtt;
  recorder.RecordEstimate(in_recovery, in_slow_start, bandwidth, estimate_time,
                          wall_time, srtt);
  EXPECT_TRUE(recorder.HasEstimate());
  EXPECT_TRUE(recorder.EstimateRecordedDuringSlowStart());

  // Now send another estimate, this time not in slow start.
  estimate_time = estimate_time + 3 * srtt;
  in_slow_start = false;
  recorder.RecordEstimate(in_recovery, in_slow_start, bandwidth, estimate_time,
                          wall_time, srtt);
  EXPECT_TRUE(recorder.HasEstimate());
  EXPECT_FALSE(recorder.EstimateRecordedDuringSlowStart());
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
