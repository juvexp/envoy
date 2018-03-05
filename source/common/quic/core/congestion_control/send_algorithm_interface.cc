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

#include "common/quic/core/congestion_control/send_algorithm_interface.h"

#include "common/quic/core/congestion_control/bbr_sender.h"
#include "common/quic/core/congestion_control/tcp_cubic_sender_bytes.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/platform/api/quic_bug_tracker.h"
#include "common/quic/platform/api/quic_fallthrough.h"
#include "common/quic/platform/api/quic_flag_utils.h"
#include "common/quic/platform/api/quic_flags.h"
#include "common/quic/platform/api/quic_pcc_sender.h"

namespace gfe_quic {

class RttStats;

// Factory for send side congestion control algorithm.
SendAlgorithmInterface* SendAlgorithmInterface::Create(
    const QuicClock* clock,
    const RttStats* rtt_stats,
    const QuicUnackedPacketMap* unacked_packets,
    CongestionControlType congestion_control_type,
    QuicRandom* random,
    QuicConnectionStats* stats,
    QuicPacketCount initial_congestion_window) {
  QuicPacketCount max_congestion_window = kDefaultMaxCongestionWindowPackets;
  switch (congestion_control_type) {
    case kBBR:
      return new BbrSender(rtt_stats, unacked_packets,
                           initial_congestion_window, max_congestion_window,
                           random);
    case kPCC:
      if (GetQuicReloadableFlag(quic_enable_pcc)) {
        return CreatePccSender(clock, rtt_stats, unacked_packets, random, stats,
                               initial_congestion_window,
                               max_congestion_window);
      }
      // Fall back to CUBIC if PCC is disabled.
      QUIC_FALLTHROUGH_INTENDED;
    case kCubicBytes:
      return new TcpCubicSenderBytes(
          clock, rtt_stats, false /* don't use Reno */,
          initial_congestion_window, max_congestion_window, stats);
    case kRenoBytes:
      return new TcpCubicSenderBytes(clock, rtt_stats, true /* use Reno */,
                                     initial_congestion_window,
                                     max_congestion_window, stats);
  }
  return nullptr;
}

}  // namespace gfe_quic
