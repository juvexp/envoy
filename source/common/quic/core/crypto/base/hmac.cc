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

#include "common/quic/core/crypto/base/hmac.h"

#include "common/quic/platform/api/quic_logging.h"
#include "third_party/openssl/crypto.h"
#include "third_party/openssl/evp.h"
#include "third_party/openssl/hmac.h"
#include "third_party/openssl/sha.h"

namespace gfe_quic {
namespace crypto {

HMAC::HMAC(HashAlgorithm hash_alg) {
  DCHECK_EQ(hash_alg, SHA256);
}

size_t HMAC::DigestLength() const {
  return SHA256_DIGEST_LENGTH;
}

bool HMAC::Init(const uint8_t* key, size_t key_length) {
  if (!key_.empty()) {
    return false;
  }

  key_.assign(key, key + key_length);
  return true;
}

bool HMAC::Init(QuicStringPiece key) {
  return Init(reinterpret_cast<const uint8_t*>(key.data()), key.size());
}

bool HMAC::Sign(QuicStringPiece data,
                uint8_t* out_digest,
                size_t digest_length) const {
  DCHECK(!key_.empty());
  DCHECK(digest_length);

  uint8_t tmp[SHA256_DIGEST_LENGTH];
  uint8_t* digest = tmp;
  if (digest_length >= SHA256_DIGEST_LENGTH) {
    digest = out_digest;
  }

  if (nullptr == ::HMAC(EVP_sha256(), &key_[0], key_.size(),
                        reinterpret_cast<const uint8_t*>(data.data()),
                        data.size(), digest, nullptr)) {
    return false;
  }
  if (digest_length < SHA256_DIGEST_LENGTH) {
    memcpy(out_digest, tmp, digest_length);
  }

  return true;
}

bool HMAC::Verify(QuicStringPiece data, QuicStringPiece digest) const {
  if (digest.size() != SHA256_DIGEST_LENGTH) {
    return false;
  }

  uint8_t computed_digest[SHA256_DIGEST_LENGTH];
  if (!Sign(data, computed_digest, sizeof(computed_digest))) {
    return false;
  }

  return 0 ==
         CRYPTO_memcmp(digest.data(), computed_digest, SHA256_DIGEST_LENGTH);
}

}  // namespace crypto
}  // namespace gfe_quic
