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

// A convenience class to store rtt samples and calculate smoothed rtt.

#ifndef GFE_QUIC_CORE_CONGESTION_CONTROL_RTT_STATS_H_
#define GFE_QUIC_CORE_CONGESTION_CONTROL_RTT_STATS_H_

#include <algorithm>
#include <cstdint>

#include "base/macros.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_time.h"
#include "common/quic/platform/api/quic_bug_tracker.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

namespace test {
class RttStatsPeer;
}  // namespace test

class QUIC_EXPORT_PRIVATE RttStats {
 public:
  RttStats();

  // Updates the RTT from an incoming ack which is received |send_delta| after
  // the packet is sent and the peer reports the ack being delayed |ack_delay|.
  void UpdateRtt(QuicTime::Delta send_delta,
                 QuicTime::Delta ack_delay,
                 QuicTime now);

  // Causes the smoothed_rtt to be increased to the latest_rtt if the latest_rtt
  // is larger. The mean deviation is increased to the most recent deviation if
  // it's larger.
  void ExpireSmoothedMetrics();

  // Called when connection migrates and rtt measurement needs to be reset.
  void OnConnectionMigration();

  // Returns the EWMA smoothed RTT for the connection.
  // May return Zero if no valid updates have occurred.
  QuicTime::Delta smoothed_rtt() const { return smoothed_rtt_; }

  // Returns the EWMA smoothed RTT prior to the most recent RTT sample.
  QuicTime::Delta previous_srtt() const { return previous_srtt_; }

  QuicTime::Delta initial_rtt() const { return initial_rtt_; }

  QuicTime::Delta SmoothedOrInitialRtt() const {
    return smoothed_rtt_.IsZero() ? initial_rtt_ : smoothed_rtt_;
  }

  // Sets an initial RTT to be used for SmoothedRtt before any RTT updates.
  void set_initial_rtt(QuicTime::Delta initial_rtt) {
    if (initial_rtt.ToMicroseconds() <= 0) {
      QUIC_BUG << "Attempt to set initial rtt to <= 0.";
      return;
    }
    initial_rtt_ = initial_rtt;
  }

  // The most recent rtt measurement.
  // May return Zero if no valid updates have occurred.
  QuicTime::Delta latest_rtt() const { return latest_rtt_; }

  // Returns the min_rtt for the entire connection.
  // May return Zero if no valid updates have occurred.
  QuicTime::Delta min_rtt() const { return min_rtt_; }

  QuicTime::Delta mean_deviation() const { return mean_deviation_; }

  QuicTime::Delta max_ack_delay() const { return max_ack_delay_; }

  bool ignore_max_ack_delay() const { return ignore_max_ack_delay_; }

  void set_ignore_max_ack_delay(bool ignore_max_ack_delay) {
    ignore_max_ack_delay_ = ignore_max_ack_delay;
  }

  void set_initial_max_ack_delay(QuicTime::Delta initial_max_ack_delay) {
    DCHECK(max_ack_delay_.IsZero());
    max_ack_delay_ = std::max(max_ack_delay_, initial_max_ack_delay);
  }

 private:
  friend class test::RttStatsPeer;

  QuicTime::Delta latest_rtt_;
  QuicTime::Delta min_rtt_;
  QuicTime::Delta smoothed_rtt_;
  QuicTime::Delta previous_srtt_;
  // Mean RTT deviation during this session.
  // Approximation of standard deviation, the error is roughly 1.25 times
  // larger than the standard deviation, for a normally distributed signal.
  QuicTime::Delta mean_deviation_;
  QuicTime::Delta initial_rtt_;
  // The maximum ack delay observed over the connection after excluding ack
  // delays that were too large to be included in an RTT measurement.
  QuicTime::Delta max_ack_delay_;
  // Whether to ignore the peer's max ack delay.
  bool ignore_max_ack_delay_;

  DISALLOW_COPY_AND_ASSIGN(RttStats);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CONGESTION_CONTROL_RTT_STATS_H_
