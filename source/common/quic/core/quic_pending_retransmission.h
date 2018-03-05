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

#ifndef GFE_QUIC_CORE_QUIC_PENDING_RETRANSMISSION_H_
#define GFE_QUIC_CORE_QUIC_PENDING_RETRANSMISSION_H_

#include "common/quic/core/frames/quic_frame.h"
#include "common/quic/core/quic_transmission_info.h"
#include "common/quic/core/quic_types.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

// Struct to store the pending retransmission information.
struct QUIC_EXPORT_PRIVATE QuicPendingRetransmission {
  QuicPendingRetransmission(QuicPacketNumber packet_number,
                            TransmissionType transmission_type,
                            const QuicFrames& retransmittable_frames,
                            bool has_crypto_handshake,
                            int num_padding_bytes,
                            EncryptionLevel encryption_level,
                            QuicPacketNumberLength packet_number_length)
      : packet_number(packet_number),
        retransmittable_frames(retransmittable_frames),
        transmission_type(transmission_type),
        has_crypto_handshake(has_crypto_handshake),
        num_padding_bytes(num_padding_bytes),
        encryption_level(encryption_level),
        packet_number_length(packet_number_length) {}

  QuicPendingRetransmission(QuicPacketNumber packet_number,
                            TransmissionType transmission_type,
                            const QuicTransmissionInfo& tranmission_info)
      : packet_number(packet_number),
        retransmittable_frames(tranmission_info.retransmittable_frames),
        transmission_type(transmission_type),
        has_crypto_handshake(tranmission_info.has_crypto_handshake),
        num_padding_bytes(tranmission_info.num_padding_bytes),
        encryption_level(tranmission_info.encryption_level),
        packet_number_length(tranmission_info.packet_number_length) {}

  QuicPacketNumber packet_number;
  const QuicFrames& retransmittable_frames;
  TransmissionType transmission_type;
  bool has_crypto_handshake;
  int num_padding_bytes;
  EncryptionLevel encryption_level;
  QuicPacketNumberLength packet_number_length;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_PENDING_RETRANSMISSION_H_
