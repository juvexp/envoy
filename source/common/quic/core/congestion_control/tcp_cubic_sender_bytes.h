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

// TCP cubic send side congestion algorithm, emulates the behavior of TCP cubic.

#ifndef GFE_QUIC_CORE_CONGESTION_CONTROL_TCP_CUBIC_SENDER_BYTES_H_
#define GFE_QUIC_CORE_CONGESTION_CONTROL_TCP_CUBIC_SENDER_BYTES_H_

#include <cstdint>

#include "base/macros.h"
#include "common/quic/core/congestion_control/cubic_bytes.h"
#include "common/quic/core/congestion_control/hybrid_slow_start.h"
#include "common/quic/core/congestion_control/prr_sender.h"
#include "common/quic/core/congestion_control/send_algorithm_interface.h"
#include "common/quic/core/quic_bandwidth.h"
#include "common/quic/core/quic_connection_stats.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_time.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {

class RttStats;

// Maximum window to allow when doing bandwidth resumption.
const QuicPacketCount kMaxResumptionCongestionWindow = 200;

namespace test {
class TcpCubicSenderBytesPeer;
}  // namespace test

class QUIC_EXPORT_PRIVATE TcpCubicSenderBytes : public SendAlgorithmInterface {
 public:
  TcpCubicSenderBytes(const QuicClock* clock,
                      const RttStats* rtt_stats,
                      bool reno,
                      QuicPacketCount initial_tcp_congestion_window,
                      QuicPacketCount max_congestion_window,
                      QuicConnectionStats* stats);
  ~TcpCubicSenderBytes() override;

  // Start implementation of SendAlgorithmInterface.
  void SetFromConfig(const QuicConfig& config,
                     Perspective perspective) override;
  void AdjustNetworkParameters(QuicBandwidth bandwidth,
                               QuicTime::Delta rtt) override;
  void SetNumEmulatedConnections(int num_connections) override;
  void OnConnectionMigration() override;
  void OnCongestionEvent(bool rtt_updated,
                         QuicByteCount prior_in_flight,
                         QuicTime event_time,
                         const AckedPacketVector& acked_packets,
                         const LostPacketVector& lost_packets) override;
  void OnPacketSent(QuicTime sent_time,
                    QuicByteCount bytes_in_flight,
                    QuicPacketNumber packet_number,
                    QuicByteCount bytes,
                    HasRetransmittableData is_retransmittable) override;
  void OnRetransmissionTimeout(bool packets_retransmitted) override;
  bool CanSend(QuicByteCount bytes_in_flight) override;
  QuicBandwidth PacingRate(QuicByteCount bytes_in_flight) const override;
  QuicBandwidth BandwidthEstimate() const override;
  QuicByteCount GetCongestionWindow() const override;
  QuicByteCount GetSlowStartThreshold() const override;
  CongestionControlType GetCongestionControlType() const override;
  bool InSlowStart() const override;
  bool InRecovery() const override;
  bool IsProbingForMoreBandwidth() const override;
  QuicString GetDebugState() const override;
  void OnApplicationLimited(QuicByteCount bytes_in_flight) override;
  // End implementation of SendAlgorithmInterface.

  QuicByteCount min_congestion_window() const { return min_congestion_window_; }

 protected:
  // Compute the TCP Reno beta based on the current number of connections.
  float RenoBeta() const;

  bool IsCwndLimited(QuicByteCount bytes_in_flight) const;

  // TODO: Remove these and migrate to OnCongestionEvent.
  void OnPacketAcked(QuicPacketNumber acked_packet_number,
                     QuicByteCount acked_bytes,
                     QuicByteCount prior_in_flight,
                     QuicTime event_time);
  void SetCongestionWindowFromBandwidthAndRtt(QuicBandwidth bandwidth,
                                              QuicTime::Delta rtt);
  void SetCongestionWindowInPackets(QuicPacketCount congestion_window);
  void SetMinCongestionWindowInPackets(QuicPacketCount congestion_window);
  void ExitSlowstart();
  void OnPacketLost(QuicPacketNumber largest_loss,
                    QuicByteCount lost_bytes,
                    QuicByteCount prior_in_flight);
  void MaybeIncreaseCwnd(QuicPacketNumber acked_packet_number,
                         QuicByteCount acked_bytes,
                         QuicByteCount prior_in_flight,
                         QuicTime event_time);
  void HandleRetransmissionTimeout();

 private:
  friend class test::TcpCubicSenderBytesPeer;

  HybridSlowStart hybrid_slow_start_;
  PrrSender prr_;
  const RttStats* rtt_stats_;
  QuicConnectionStats* stats_;

  // If true, Reno congestion control is used instead of Cubic.
  const bool reno_;

  // Number of connections to simulate.
  uint32_t num_connections_;

  // Track the largest packet that has been sent.
  QuicPacketNumber largest_sent_packet_number_;

  // Track the largest packet that has been acked.
  QuicPacketNumber largest_acked_packet_number_;

  // Track the largest packet number outstanding when a CWND cutback occurs.
  QuicPacketNumber largest_sent_at_last_cutback_;

  // Whether to use 4 packets as the actual min, but pace lower.
  bool min4_mode_;

  // Whether the last loss event caused us to exit slowstart.
  // Used for stats collection of slowstart_packets_lost
  bool last_cutback_exited_slowstart_;

  // When true, exit slow start with large cutback of congestion window.
  bool slow_start_large_reduction_;

  // When true, use unity pacing instead of PRR.
  bool no_prr_;

  CubicBytes cubic_;

  // ACK counter for the Reno implementation.
  uint64_t num_acked_packets_;

  // Congestion window in bytes.
  QuicByteCount congestion_window_;

  // Minimum congestion window in bytes.
  QuicByteCount min_congestion_window_;

  // Maximum congestion window in bytes.
  QuicByteCount max_congestion_window_;

  // Slow start congestion window in bytes, aka ssthresh.
  QuicByteCount slowstart_threshold_;

  // Initial TCP congestion window in bytes. This variable can only be set when
  // this algorithm is created.
  const QuicByteCount initial_tcp_congestion_window_;

  // Initial maximum TCP congestion window in bytes. This variable can only be
  // set when this algorithm is created.
  const QuicByteCount initial_max_tcp_congestion_window_;

  // The minimum window when exiting slow start with large reduction.
  QuicByteCount min_slow_start_exit_window_;

  DISALLOW_COPY_AND_ASSIGN(TcpCubicSenderBytes);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CONGESTION_CONTROL_TCP_CUBIC_SENDER_BYTES_H_