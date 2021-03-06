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

#include "common/quic/core/crypto/aes_128_gcm_decrypter.h"

#include "common/quic/platform/api/quic_flag_utils.h"
#include "common/quic/platform/api/quic_flags.h"
#include "third_party/openssl/aead.h"
#include "third_party/openssl/tls1.h"

namespace gfe_quic {

namespace {

const size_t kKeySize = 16;
const size_t kNonceSize = 12;

}  // namespace

Aes128GcmDecrypter::Aes128GcmDecrypter()
    : AeadBaseDecrypter(EVP_aead_aes_128_gcm(),
                        kKeySize,
                        kAuthTagSize,
                        kNonceSize,
                        /* use_ietf_nonce_construction */ true) {
  static_assert(kKeySize <= kMaxKeySize, "key size too big");
  static_assert(kNonceSize <= kMaxNonceSize, "nonce size too big");
}

Aes128GcmDecrypter::~Aes128GcmDecrypter() {}

uint32_t Aes128GcmDecrypter::cipher_id() const {
  return TLS1_CK_AES_128_GCM_SHA256;
}

}  // namespace gfe_quic
