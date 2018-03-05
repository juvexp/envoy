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

#ifndef GFE_QUIC_CORE_CRYPTO_AEAD_BASE_ENCRYPTER_H_
#define GFE_QUIC_CORE_CRYPTO_AEAD_BASE_ENCRYPTER_H_

#include <cstddef>

#include "base/macros.h"
#include "common/quic/core/crypto/quic_encrypter.h"
#include "common/quic/core/crypto/scoped_evp_aead_ctx.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {

// AeadBaseEncrypter is the base class of AEAD QuicEncrypter subclasses.
class QUIC_EXPORT_PRIVATE AeadBaseEncrypter : public QuicEncrypter {
 public:
  AeadBaseEncrypter(const EVP_AEAD* aead_alg,
                    size_t key_size,
                    size_t auth_tag_size,
                    size_t nonce_size,
                    bool use_ietf_nonce_construction);
  ~AeadBaseEncrypter() override;

  // QuicEncrypter implementation
  bool SetKey(QuicStringPiece key) override;
  bool SetNoncePrefix(QuicStringPiece nonce_prefix) override;
  bool SetIV(QuicStringPiece iv) override;
  bool EncryptPacket(QuicTransportVersion version,
                     QuicPacketNumber packet_number,
                     QuicStringPiece associated_data,
                     QuicStringPiece plaintext,
                     char* output,
                     size_t* output_length,
                     size_t max_output_length) override;
  size_t GetKeySize() const override;
  size_t GetNoncePrefixSize() const override;
  size_t GetIVSize() const override;
  size_t GetMaxPlaintextSize(size_t ciphertext_size) const override;
  size_t GetCiphertextSize(size_t plaintext_size) const override;
  QuicStringPiece GetKey() const override;
  QuicStringPiece GetNoncePrefix() const override;

  // Necessary so unit tests can explicitly specify a nonce, instead of an IV
  // (or nonce prefix) and packet number.
  bool Encrypt(QuicStringPiece nonce,
               QuicStringPiece associated_data,
               QuicStringPiece plaintext,
               unsigned char* output);

 protected:
  // Make these constants available to the subclasses so that the subclasses
  // can assert at compile time their key_size_ and nonce_size_ do not
  // exceed the maximum.
  static const size_t kMaxKeySize = 32;
  enum : size_t { kMaxNonceSize = 12 };

 private:
  const EVP_AEAD* const aead_alg_;
  const size_t key_size_;
  const size_t auth_tag_size_;
  const size_t nonce_size_;
  const bool use_ietf_nonce_construction_;

  // The key.
  unsigned char key_[kMaxKeySize];
  // The IV used to construct the nonce.
  unsigned char iv_[kMaxNonceSize];

  ScopedEVPAEADCtx ctx_;

  DISALLOW_COPY_AND_ASSIGN(AeadBaseEncrypter);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_AEAD_BASE_ENCRYPTER_H_
