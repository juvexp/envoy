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

#ifndef GFE_QUIC_CORE_CRYPTO_AES_128_GCM_12_ENCRYPTER_H_
#define GFE_QUIC_CORE_CRYPTO_AES_128_GCM_12_ENCRYPTER_H_

#include "base/macros.h"
#include "common/quic/core/crypto/aead_base_encrypter.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

// An Aes128Gcm12Encrypter is a QuicEncrypter that implements the
// AEAD_AES_128_GCM_12 algorithm specified in RFC 5282. Create an instance by
// calling QuicEncrypter::Create(kAESG).
//
// It uses an authentication tag of 12 bytes (96 bits). The fixed prefix
// of the nonce is four bytes.
class QUIC_EXPORT_PRIVATE Aes128Gcm12Encrypter : public AeadBaseEncrypter {
 public:
  enum {
    // Authentication tags are truncated to 96 bits.
    kAuthTagSize = 12,
  };

  Aes128Gcm12Encrypter();
  ~Aes128Gcm12Encrypter() override;

 private:
  DISALLOW_COPY_AND_ASSIGN(Aes128Gcm12Encrypter);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_AES_128_GCM_12_ENCRYPTER_H_
