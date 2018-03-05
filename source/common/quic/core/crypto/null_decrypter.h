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

#ifndef GFE_QUIC_CORE_CRYPTO_NULL_DECRYPTER_H_
#define GFE_QUIC_CORE_CRYPTO_NULL_DECRYPTER_H_

#include <cstddef>
#include <cstdint>

#include "base/int128.h"
#include "base/macros.h"
#include "common/quic/core/crypto/quic_decrypter.h"
#include "common/quic/core/quic_types.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {

class QuicDataReader;

// A NullDecrypter is a QuicDecrypter used before a crypto negotiation
// has occurred.  It does not actually decrypt the payload, but does
// verify a hash (fnv128) over both the payload and associated data.
class QUIC_EXPORT_PRIVATE NullDecrypter : public QuicDecrypter {
 public:
  explicit NullDecrypter(Perspective perspective);
  ~NullDecrypter() override {}

  // QuicDecrypter implementation
  bool SetKey(QuicStringPiece key) override;
  bool SetNoncePrefix(QuicStringPiece nonce_prefix) override;
  bool SetIV(QuicStringPiece iv) override;
  bool SetPreliminaryKey(QuicStringPiece key) override;
  bool SetDiversificationNonce(const DiversificationNonce& nonce) override;
  bool DecryptPacket(QuicTransportVersion version,
                     QuicPacketNumber packet_number,
                     QuicStringPiece associated_data,
                     QuicStringPiece ciphertext,
                     char* output,
                     size_t* output_length,
                     size_t max_output_length) override;
  size_t GetKeySize() const override;
  size_t GetIVSize() const override;
  QuicStringPiece GetKey() const override;
  QuicStringPiece GetNoncePrefix() const override;

  uint32_t cipher_id() const override;

 private:
  bool ReadHash(QuicDataReader* reader, uint128* hash);
  uint128 ComputeHash(QuicTransportVersion version,
                      QuicStringPiece data1,
                      QuicStringPiece data2) const;

  Perspective perspective_;

  DISALLOW_COPY_AND_ASSIGN(NullDecrypter);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_NULL_DECRYPTER_H_
