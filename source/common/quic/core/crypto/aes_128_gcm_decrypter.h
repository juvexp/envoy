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

#ifndef GFE_QUIC_CORE_CRYPTO_AES_128_GCM_DECRYPTER_H_
#define GFE_QUIC_CORE_CRYPTO_AES_128_GCM_DECRYPTER_H_

#include <cstdint>

#include "base/macros.h"
#include "common/quic/core/crypto/aead_base_decrypter.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

// An Aes128GcmDecrypter is a QuicDecrypter that implements the
// AEAD_AES_128_GCM algorithm specified in RFC 5116 for use in IETF QUIC.
//
// It uses an authentication tag of 16 bytes (128 bits). It uses a 12 byte IV
// that is XOR'd with the packet number to compute the nonce.
class QUIC_EXPORT_PRIVATE Aes128GcmDecrypter : public AeadBaseDecrypter {
 public:
  enum {
    kAuthTagSize = 16,
  };

  Aes128GcmDecrypter();
  ~Aes128GcmDecrypter() override;

  uint32_t cipher_id() const override;

 private:
  DISALLOW_COPY_AND_ASSIGN(Aes128GcmDecrypter);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_AES_128_GCM_DECRYPTER_H_
