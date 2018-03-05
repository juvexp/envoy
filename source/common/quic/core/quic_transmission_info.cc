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

#include "common/quic/core/quic_transmission_info.h"

namespace gfe_quic {

QuicTransmissionInfo::QuicTransmissionInfo()
    : encryption_level(ENCRYPTION_NONE),
      packet_number_length(PACKET_1BYTE_PACKET_NUMBER),
      bytes_sent(0),
      sent_time(QuicTime::Zero()),
      transmission_type(NOT_RETRANSMISSION),
      in_flight(false),
      state(OUTSTANDING),
      has_crypto_handshake(false),
      num_padding_bytes(0),
      retransmission(0),
      largest_acked(0) {}

QuicTransmissionInfo::QuicTransmissionInfo(
    EncryptionLevel level,
    QuicPacketNumberLength packet_number_length,
    TransmissionType transmission_type,
    QuicTime sent_time,
    QuicPacketLength bytes_sent,
    bool has_crypto_handshake,
    int num_padding_bytes)
    : encryption_level(level),
      packet_number_length(packet_number_length),
      bytes_sent(bytes_sent),
      sent_time(sent_time),
      transmission_type(transmission_type),
      in_flight(false),
      state(OUTSTANDING),
      has_crypto_handshake(has_crypto_handshake),
      num_padding_bytes(num_padding_bytes),
      retransmission(0),
      largest_acked(0) {}

QuicTransmissionInfo::QuicTransmissionInfo(const QuicTransmissionInfo& other) =
    default;

QuicTransmissionInfo::~QuicTransmissionInfo() {}

}  // namespace gfe_quic
