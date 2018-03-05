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

#ifndef GFE_QUIC_CORE_QUIC_UTILS_H_
#define GFE_QUIC_CORE_QUIC_UTILS_H_

#include <cstddef>
#include <cstdint>

#include "base/int128.h"
#include "base/macros.h"
#include "common/quic/core/quic_error_codes.h"
#include "common/quic/core/quic_types.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_socket_address.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {

class QUIC_EXPORT_PRIVATE QuicUtils {
 public:
  // Returns the 64 bit FNV1a hash of the data.  See
  // http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-param
  static uint64_t FNV1a_64_Hash(QuicStringPiece data);

  // Returns the 128 bit FNV1a hash of the data.  See
  // http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-param
  static uint128 FNV1a_128_Hash(QuicStringPiece data);

  // Returns the 128 bit FNV1a hash of the two sequences of data.  See
  // http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-param
  static uint128 FNV1a_128_Hash_Two(QuicStringPiece data1,
                                    QuicStringPiece data2);

  // Returns the 128 bit FNV1a hash of the three sequences of data.  See
  // http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-param
  static uint128 FNV1a_128_Hash_Three(QuicStringPiece data1,
                                      QuicStringPiece data2,
                                      QuicStringPiece data3);

  // SerializeUint128 writes the first 96 bits of |v| in little-endian form
  // to |out|.
  static void SerializeUint128Short(uint128 v, uint8_t* out);

  // Returns the level of encryption as a char*
  static const char* EncryptionLevelToString(EncryptionLevel level);

  // Returns TransmissionType as a char*
  static const char* TransmissionTypeToString(TransmissionType type);

  // Returns AddressChangeType as a string.
  static QuicString AddressChangeTypeToString(AddressChangeType type);

  // Returns SentPacketState as a char*.
  static const char* SentPacketStateToString(SentPacketState state);

  // Determines and returns change type of address change from |old_address| to
  // |new_address|.
  static AddressChangeType DetermineAddressChangeType(
      const QuicSocketAddress& old_address,
      const QuicSocketAddress& new_address);

  // Copies |buffer_length| bytes from iov starting at offset |iov_offset| into
  // buffer. |iov| must be at least iov_offset+length total length and buffer
  // must be at least |length| long.
  static void CopyToBuffer(const struct iovec* iov,
                           int iov_count,
                           size_t iov_offset,
                           size_t buffer_length,
                           char* buffer);

  // Returns true if a packet is ackable. A packet is unackable if it can never
  // be acked. Occurs when a packet is never sent, after it is acknowledged
  // once, or if it's a crypto packet we never expect to receive an ack for.
  static bool IsAckable(SentPacketState state);

  // Returns packet state corresponding to |retransmission_type|.
  static SentPacketState RetransmissionTypeToPacketState(
      TransmissionType retransmission_type);

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicUtils);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_UTILS_H_
