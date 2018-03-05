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

// A send algorithm that adds pacing on top of an another send algorithm.
// It uses the underlying sender's pacing rate to schedule packets.
// It also takes into consideration the expected granularity of the underlying
// alarm to ensure that alarms are not set too aggressively, and err towards
// sending packets too early instead of too late.

#ifndef GFE_QUIC_CORE_CONGESTION_CONTROL_PACING_SENDER_H_
#define GFE_QUIC_CORE_CONGESTION_CONTROL_PACING_SENDER_H_

#include <cstdint>
#include <map>
#include <memory>

#include "base/macros.h"
#include "common/quic/core/congestion_control/send_algorithm_interface.h"
#include "common/quic/core/quic_bandwidth.h"
#include "common/quic/core/quic_config.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_time.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

namespace test {
class QuicSentPacketManagerPeer;
}  // namespace test

class QUIC_EXPORT_PRIVATE PacingSender {
 public:
  PacingSender();
  ~PacingSender();

  // Sets the underlying sender. Does not take ownership of |sender|. |sender|
  // must not be null. This must be called before any of the
  // SendAlgorithmInterface wrapper methods are called.
  void set_sender(SendAlgorithmInterface* sender);

  void set_max_pacing_rate(QuicBandwidth max_pacing_rate) {
    max_pacing_rate_ = max_pacing_rate;
  }

  QuicBandwidth max_pacing_rate() const { return max_pacing_rate_; }

  void OnCongestionEvent(bool rtt_updated,
                         QuicByteCount bytes_in_flight,
                         QuicTime event_time,
                         const AckedPacketVector& acked_packets,
                         const LostPacketVector& lost_packets);

  void OnPacketSent(QuicTime sent_time,
                    QuicByteCount bytes_in_flight,
                    QuicPacketNumber packet_number,
                    QuicByteCount bytes,
                    HasRetransmittableData has_retransmittable_data);

  QuicTime::Delta TimeUntilSend(QuicTime now, QuicByteCount bytes_in_flight);

  QuicBandwidth PacingRate(QuicByteCount bytes_in_flight) const;

 private:
  friend class test::QuicSentPacketManagerPeer;

  // Underlying sender. Not owned.
  SendAlgorithmInterface* sender_;
  // If not QuicBandidth::Zero, the maximum rate the PacingSender will use.
  QuicBandwidth max_pacing_rate_;

  // Number of unpaced packets to be sent before packets are delayed.
  uint32_t burst_tokens_;
  // Send time of the last packet considered delayed.
  QuicTime last_delayed_packet_sent_time_;
  QuicTime ideal_next_packet_send_time_;  // When can the next packet be sent.
  bool was_last_send_delayed_;  // True when the last send was delayed.
  uint32_t initial_burst_size_;

  DISALLOW_COPY_AND_ASSIGN(PacingSender);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CONGESTION_CONTROL_PACING_SENDER_H_
