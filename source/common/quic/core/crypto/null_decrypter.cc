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

#include "common/quic/core/crypto/null_decrypter.h"

#include <cstdint>

#include "base/int128.h"
#include "common/quic/core/quic_data_reader.h"
#include "common/quic/core/quic_utils.h"
#include "common/quic/platform/api/quic_bug_tracker.h"

using absl::MakeUint128;

namespace gfe_quic {

NullDecrypter::NullDecrypter(Perspective perspective)
    : perspective_(perspective) {}

bool NullDecrypter::SetKey(QuicStringPiece key) {
  return key.empty();
}

bool NullDecrypter::SetNoncePrefix(QuicStringPiece nonce_prefix) {
  return nonce_prefix.empty();
}

bool NullDecrypter::SetIV(QuicStringPiece iv) {
  return iv.empty();
}

bool NullDecrypter::SetPreliminaryKey(QuicStringPiece key) {
  QUIC_BUG << "Should not be called";
  return false;
}

bool NullDecrypter::SetDiversificationNonce(const DiversificationNonce& nonce) {
  QUIC_BUG << "Should not be called";
  return true;
}

bool NullDecrypter::DecryptPacket(QuicTransportVersion version,
                                  QuicPacketNumber /*packet_number*/,
                                  QuicStringPiece associated_data,
                                  QuicStringPiece ciphertext,
                                  char* output,
                                  size_t* output_length,
                                  size_t max_output_length) {
  QuicDataReader reader(ciphertext.data(), ciphertext.length(),
                        HOST_BYTE_ORDER);
  uint128 hash;

  if (!ReadHash(&reader, &hash)) {
    return false;
  }

  QuicStringPiece plaintext = reader.ReadRemainingPayload();
  if (plaintext.length() > max_output_length) {
    QUIC_BUG << "Output buffer must be larger than the plaintext.";
    return false;
  }
  if (hash != ComputeHash(version, associated_data, plaintext)) {
    return false;
  }
  // Copy the plaintext to output.
  memcpy(output, plaintext.data(), plaintext.length());
  *output_length = plaintext.length();
  return true;
}

size_t NullDecrypter::GetKeySize() const {
  return 0;
}

size_t NullDecrypter::GetIVSize() const {
  return 0;
}

QuicStringPiece NullDecrypter::GetKey() const {
  return QuicStringPiece();
}

QuicStringPiece NullDecrypter::GetNoncePrefix() const {
  return QuicStringPiece();
}

uint32_t NullDecrypter::cipher_id() const {
  return 0;
}

bool NullDecrypter::ReadHash(QuicDataReader* reader, uint128* hash) {
  uint64_t lo;
  uint32_t hi;
  if (!reader->ReadUInt64(&lo) || !reader->ReadUInt32(&hi)) {
    return false;
  }
  *hash = MakeUint128(hi, lo);
  return true;
}

uint128 NullDecrypter::ComputeHash(QuicTransportVersion version,
                                   const QuicStringPiece data1,
                                   const QuicStringPiece data2) const {
  uint128 correct_hash;
  if (version > QUIC_VERSION_35) {
    if (perspective_ == Perspective::IS_CLIENT) {
      // Peer is a server.
      correct_hash = QuicUtils::FNV1a_128_Hash_Three(data1, data2, "Server");

    } else {
      // Peer is a client.
      correct_hash = QuicUtils::FNV1a_128_Hash_Three(data1, data2, "Client");
    }
  } else {
    correct_hash = QuicUtils::FNV1a_128_Hash_Two(data1, data2);
  }
  uint128 mask = MakeUint128(UINT64_C(0x0), UINT64_C(0xffffffff));
  mask <<= 96;
  correct_hash &= ~mask;
  return correct_hash;
}

}  // namespace gfe_quic
