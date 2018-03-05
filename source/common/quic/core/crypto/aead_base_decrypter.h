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

#ifndef GFE_QUIC_CORE_CRYPTO_AEAD_BASE_DECRYPTER_H_
#define GFE_QUIC_CORE_CRYPTO_AEAD_BASE_DECRYPTER_H_

#include <cstddef>

#include "base/macros.h"
#include "common/quic/core/crypto/quic_decrypter.h"
#include "common/quic/core/crypto/scoped_evp_aead_ctx.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {

// AeadBaseDecrypter is the base class of AEAD QuicDecrypter subclasses.
class QUIC_EXPORT_PRIVATE AeadBaseDecrypter : public QuicDecrypter {
 public:
  AeadBaseDecrypter(const EVP_AEAD* aead_alg,
                    size_t key_size,
                    size_t auth_tag_size,
                    size_t nonce_size,
                    bool use_ietf_nonce_construction);
  ~AeadBaseDecrypter() override;

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

 protected:
  // Make these constants available to the subclasses so that the subclasses
  // can assert at compile time their key_size_ and nonce_size_ do not
  // exceed the maximum.
  static const size_t kMaxKeySize = 32;
  static const size_t kMaxNonceSize = 12;

 private:
  const EVP_AEAD* const aead_alg_;
  const size_t key_size_;
  const size_t auth_tag_size_;
  const size_t nonce_size_;
  const bool use_ietf_nonce_construction_;
  bool have_preliminary_key_;

  // The key.
  unsigned char key_[kMaxKeySize];
  // The IV used to construct the nonce.
  unsigned char iv_[kMaxNonceSize];

  ScopedEVPAEADCtx ctx_;

  DISALLOW_COPY_AND_ASSIGN(AeadBaseDecrypter);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_AEAD_BASE_DECRYPTER_H_
