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

// The pure virtual class for send side loss detection algorithm.

#ifndef GFE_QUIC_CORE_CONGESTION_CONTROL_LOSS_DETECTION_INTERFACE_H_
#define GFE_QUIC_CORE_CONGESTION_CONTROL_LOSS_DETECTION_INTERFACE_H_

#include "common/quic/core/congestion_control/send_algorithm_interface.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_time.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

class QuicUnackedPacketMap;
class RttStats;

class QUIC_EXPORT_PRIVATE LossDetectionInterface {
 public:
  virtual ~LossDetectionInterface() {}

  virtual LossDetectionType GetLossDetectionType() const = 0;

  // Called when a new ack arrives or the loss alarm fires.
  virtual void DetectLosses(const QuicUnackedPacketMap& unacked_packets,
                            QuicTime time,
                            const RttStats& rtt_stats,
                            QuicPacketNumber largest_newly_acked,
                            LostPacketVector* packets_lost) = 0;

  // Get the time the LossDetectionAlgorithm wants to re-evaluate losses.
  // Returns QuicTime::Zero if no alarm needs to be set.
  virtual QuicTime GetLossTimeout() const = 0;

  // Called when a |spurious_retransmission| is detected.  The original
  // transmission must have been caused by DetectLosses.
  virtual void SpuriousRetransmitDetected(
      const QuicUnackedPacketMap& unacked_packets,
      QuicTime time,
      const RttStats& rtt_stats,
      QuicPacketNumber spurious_retransmission) = 0;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CONGESTION_CONTROL_LOSS_DETECTION_INTERFACE_H_
