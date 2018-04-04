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

#include "common/quic/core/congestion_control/prr_sender.h"

#include "common/quic/core/quic_packets.h"

namespace gfe_quic {

namespace {
// Constant based on TCP defaults.
const QuicByteCount kMaxSegmentSize = kDefaultTCPMSS;
}  // namespace

PrrSender::PrrSender()
    : bytes_sent_since_loss_(0),
      bytes_delivered_since_loss_(0),
      ack_count_since_loss_(0),
      bytes_in_flight_before_loss_(0) {}

void PrrSender::OnPacketSent(QuicByteCount sent_bytes) {
  bytes_sent_since_loss_ += sent_bytes;
}

void PrrSender::OnPacketLost(QuicByteCount prior_in_flight) {
  bytes_sent_since_loss_ = 0;
  bytes_in_flight_before_loss_ = prior_in_flight;
  bytes_delivered_since_loss_ = 0;
  ack_count_since_loss_ = 0;
}

void PrrSender::OnPacketAcked(QuicByteCount acked_bytes) {
  bytes_delivered_since_loss_ += acked_bytes;
  ++ack_count_since_loss_;
}

bool PrrSender::CanSend(QuicByteCount congestion_window,
                        QuicByteCount bytes_in_flight,
                        QuicByteCount slowstart_threshold) const {
  // Return QuicTime::Zero in order to ensure limited transmit always works.
  if (bytes_sent_since_loss_ == 0 || bytes_in_flight < kMaxSegmentSize) {
    return true;
  }
  if (congestion_window > bytes_in_flight) {
    // During PRR-SSRB, limit outgoing packets to 1 extra MSS per ack, instead
    // of sending the entire available window. This prevents burst retransmits
    // when more packets are lost than the CWND reduction.
    //   limit = MAX(prr_delivered - prr_out, DeliveredData) + MSS
    if (bytes_delivered_since_loss_ + ack_count_since_loss_ * kMaxSegmentSize <=
        bytes_sent_since_loss_) {
      return false;
    }
    return true;
  }
  // Implement Proportional Rate Reduction (RFC6937).
  // Checks a simplified version of the PRR formula that doesn't use division:
  // AvailableSendWindow =
  //   CEIL(prr_delivered * ssthresh / BytesInFlightAtLoss) - prr_sent
  if (bytes_delivered_since_loss_ * slowstart_threshold >
      bytes_sent_since_loss_ * bytes_in_flight_before_loss_) {
    return true;
  }
  return false;
}

}  // namespace gfe_quic