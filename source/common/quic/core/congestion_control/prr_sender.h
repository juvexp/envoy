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

// Implements Proportional Rate Reduction (PRR) per RFC 6937.

#ifndef GFE_QUIC_CORE_CONGESTION_CONTROL_PRR_SENDER_H_
#define GFE_QUIC_CORE_CONGESTION_CONTROL_PRR_SENDER_H_

#include "common/quic/core/quic_bandwidth.h"
#include "common/quic/core/quic_time.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

class QUIC_EXPORT_PRIVATE PrrSender {
 public:
  PrrSender();
  // OnPacketLost should be called on the first loss that triggers a recovery
  // period and all other methods in this class should only be called when in
  // recovery.
  void OnPacketLost(QuicByteCount prior_in_flight);
  void OnPacketSent(QuicByteCount sent_bytes);
  void OnPacketAcked(QuicByteCount acked_bytes);
  bool CanSend(QuicByteCount congestion_window,
               QuicByteCount bytes_in_flight,
               QuicByteCount slowstart_threshold) const;

 private:
  // Bytes sent and acked since the last loss event.
  // |bytes_sent_since_loss_| is the same as "prr_out_" in RFC 6937,
  // and |bytes_delivered_since_loss_| is the same as "prr_delivered_".
  QuicByteCount bytes_sent_since_loss_;
  QuicByteCount bytes_delivered_since_loss_;
  size_t ack_count_since_loss_;

  // The congestion window before the last loss event.
  QuicByteCount bytes_in_flight_before_loss_;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CONGESTION_CONTROL_PRR_SENDER_H_
