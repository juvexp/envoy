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

#ifndef GFE_QUIC_CORE_CRYPTO_CHACHA20_POLY1305_DECRYPTER_H_
#define GFE_QUIC_CORE_CRYPTO_CHACHA20_POLY1305_DECRYPTER_H_

#include <cstdint>

#include "base/macros.h"
#include "common/quic/core/crypto/aead_base_decrypter.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

// A ChaCha20Poly1305Decrypter is a QuicDecrypter that implements the
// AEAD_CHACHA20_POLY1305 algorithm specified in RFC 7539, except that
// it truncates the Poly1305 authenticator to 12 bytes. Create an instance
// by calling QuicDecrypter::Create(kCC20).
//
// It uses an authentication tag of 12 bytes (96 bits). The fixed prefix of the
// nonce is four bytes.
class QUIC_EXPORT_PRIVATE ChaCha20Poly1305Decrypter : public AeadBaseDecrypter {
 public:
  enum {
    kAuthTagSize = 12,
  };

  ChaCha20Poly1305Decrypter();
  ~ChaCha20Poly1305Decrypter() override;

  uint32_t cipher_id() const override;

 private:
  DISALLOW_COPY_AND_ASSIGN(ChaCha20Poly1305Decrypter);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_CHACHA20_POLY1305_DECRYPTER_H_
