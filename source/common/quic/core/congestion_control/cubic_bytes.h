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

// Cubic algorithm, helper class to TCP cubic.
// For details see http://netsrv.csc.ncsu.edu/export/cubic_a_new_tcp_2008.pdf.

#ifndef GFE_QUIC_CORE_CONGESTION_CONTROL_CUBIC_BYTES_H_
#define GFE_QUIC_CORE_CONGESTION_CONTROL_CUBIC_BYTES_H_

#include <cstdint>

#include "base/macros.h"
#include "common/quic/core/quic_bandwidth.h"
#include "common/quic/core/quic_connection_stats.h"
#include "common/quic/core/quic_time.h"
#include "common/quic/platform/api/quic_clock.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

namespace test {
class CubicBytesTest;
}  // namespace test

class QUIC_EXPORT_PRIVATE CubicBytes {
 public:
  explicit CubicBytes(const QuicClock* clock);

  void SetNumConnections(int num_connections);

  // Call after a timeout to reset the cubic state.
  void ResetCubicState();

  // Compute a new congestion window to use after a loss event.
  // Returns the new congestion window in packets. The new congestion window is
  // a multiplicative decrease of our current window.
  QuicByteCount CongestionWindowAfterPacketLoss(QuicPacketCount current);

  // Compute a new congestion window to use after a received ACK.
  // Returns the new congestion window in bytes. The new congestion window
  // follows a cubic function that depends on the time passed since last packet
  // loss.
  QuicByteCount CongestionWindowAfterAck(QuicByteCount acked_bytes,
                                         QuicByteCount current,
                                         QuicTime::Delta delay_min,
                                         QuicTime event_time);

  // Call on ack arrival when sender is unable to use the available congestion
  // window. Resets Cubic state during quiescence.
  void OnApplicationLimited();

 private:
  friend class test::CubicBytesTest;

  static const QuicTime::Delta MaxCubicTimeInterval() {
    return QuicTime::Delta::FromMilliseconds(30);
  }

  // Compute the TCP Cubic alpha, beta, and beta-last-max based on the
  // current number of connections.
  float Alpha() const;
  float Beta() const;
  float BetaLastMax() const;

  QuicByteCount last_max_congestion_window() const {
    return last_max_congestion_window_;
  }

  const QuicClock* clock_;

  // Number of connections to simulate.
  int num_connections_;

  // Time when this cycle started, after last loss event.
  QuicTime epoch_;

  // Max congestion window used just before last loss event.
  // Note: to improve fairness to other streams an additional back off is
  // applied to this value if the new value is below our latest value.
  QuicByteCount last_max_congestion_window_;

  // Number of acked bytes since the cycle started (epoch).
  QuicByteCount acked_bytes_count_;

  // TCP Reno equivalent congestion window in packets.
  QuicByteCount estimated_tcp_congestion_window_;

  // Origin point of cubic function.
  QuicByteCount origin_point_congestion_window_;

  // Time to origin point of cubic function in 2^10 fractions of a second.
  uint32_t time_to_origin_point_;

  // Last congestion window in packets computed by cubic function.
  QuicByteCount last_target_congestion_window_;

  DISALLOW_COPY_AND_ASSIGN(CubicBytes);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CONGESTION_CONTROL_CUBIC_BYTES_H_
