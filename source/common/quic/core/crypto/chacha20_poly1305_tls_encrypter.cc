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

#include "common/quic/core/crypto/chacha20_poly1305_tls_encrypter.h"

#include "third_party/openssl/evp.h"

namespace gfe_quic {

namespace {

const size_t kKeySize = 32;
const size_t kNonceSize = 12;

}  // namespace

ChaCha20Poly1305TlsEncrypter::ChaCha20Poly1305TlsEncrypter()
    : AeadBaseEncrypter(EVP_aead_chacha20_poly1305(),
                        kKeySize,
                        kAuthTagSize,
                        kNonceSize,
                        /* use_ietf_nonce_construction */ true) {
  static_assert(kKeySize <= kMaxKeySize, "key size too big");
  static_assert(kNonceSize <= kMaxNonceSize, "nonce size too big");
}

ChaCha20Poly1305TlsEncrypter::~ChaCha20Poly1305TlsEncrypter() {}

}  // namespace gfe_quic
