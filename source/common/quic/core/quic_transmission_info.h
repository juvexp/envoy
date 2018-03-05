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

#ifndef GFE_QUIC_CORE_QUIC_TRANSMISSION_INFO_H_
#define GFE_QUIC_CORE_QUIC_TRANSMISSION_INFO_H_

#include <list>

#include "common/quic/core/frames/quic_frame.h"
#include "common/quic/core/quic_ack_listener_interface.h"
#include "common/quic/core/quic_types.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

// Stores details of a single sent packet.
struct QUIC_EXPORT_PRIVATE QuicTransmissionInfo {
  // Used by STL when assigning into a map.
  QuicTransmissionInfo();

  // Constructs a Transmission with a new all_transmissions set
  // containing |packet_number|.
  QuicTransmissionInfo(EncryptionLevel level,
                       QuicPacketNumberLength packet_number_length,
                       TransmissionType transmission_type,
                       QuicTime sent_time,
                       QuicPacketLength bytes_sent,
                       bool has_crypto_handshake,
                       int num_padding_bytes);

  QuicTransmissionInfo(const QuicTransmissionInfo& other);

  ~QuicTransmissionInfo();

  QuicFrames retransmittable_frames;
  EncryptionLevel encryption_level;
  QuicPacketNumberLength packet_number_length;
  QuicPacketLength bytes_sent;
  QuicTime sent_time;
  // Reason why this packet was transmitted.
  TransmissionType transmission_type;
  // In flight packets have not been abandoned or lost.
  bool in_flight;
  // State of this packet.
  SentPacketState state;
  // True if the packet contains stream data from the crypto stream.
  bool has_crypto_handshake;
  // Non-zero if the packet needs padding if it's retransmitted.
  int16_t num_padding_bytes;
  // Stores the packet number of the next retransmission of this packet.
  // Zero if the packet has not been retransmitted.
  QuicPacketNumber retransmission;
  // The largest_acked in the ack frame, if the packet contains an ack.
  QuicPacketNumber largest_acked;
};
// TODO: Add static_assert when size of this struct is reduced below
// 64 bytes.
// NOTE: Existing static_assert removed because padding differences on
// 64-bit iOS resulted in an 88-byte struct that is greater than the 84-byte
// limit on other platforms.  Removing per ianswett's request.

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_TRANSMISSION_INFO_H_
