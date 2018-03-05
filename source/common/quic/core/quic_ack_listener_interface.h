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

#ifndef GFE_QUIC_CORE_QUIC_ACK_LISTENER_INTERFACE_H_
#define GFE_QUIC_CORE_QUIC_ACK_LISTENER_INTERFACE_H_

#include "common/quic/core/quic_time.h"
#include "common/quic/core/quic_types.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_reference_counted.h"

namespace gfe_quic {

// Pure virtual class to listen for packet acknowledgements.
class QUIC_EXPORT_PRIVATE QuicAckListenerInterface
    : public QuicReferenceCounted {
 public:
  QuicAckListenerInterface() {}

  // Called when a packet is acked.  Called once per packet.
  // |acked_bytes| is the number of data bytes acked.
  virtual void OnPacketAcked(int acked_bytes,
                             QuicTime::Delta ack_delay_time) = 0;

  // Called when a packet is retransmitted.  Called once per packet.
  // |retransmitted_bytes| is the number of data bytes retransmitted.
  virtual void OnPacketRetransmitted(int retransmitted_bytes) = 0;

 protected:
  // Delegates are ref counted.
  ~QuicAckListenerInterface() override;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_ACK_LISTENER_INTERFACE_H_
