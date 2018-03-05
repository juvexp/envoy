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

#ifndef GFE_QUIC_CORE_CRYPTO_BASE_HMAC_H_
#define GFE_QUIC_CORE_CRYPTO_BASE_HMAC_H_

#include <vector>

#include "base/macros.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {
namespace crypto {

class HMAC {
 public:
  // In google3, only SHA-256 is supported.
  enum HashAlgorithm {
    SHA256,
  };

  explicit HMAC(HashAlgorithm hash_alg);

  // DigestLength returns the length, in bytes, of the resulting digest.
  size_t DigestLength() const;

  // Initializes this instance using |key|. Call Init only once. It returns
  // false on the second of later calls.
  bool Init(const uint8_t* key, size_t key_length);
  bool Init(QuicStringPiece key);

  // Sign calculates the HMAC of |data| with the key supplied to the Init
  // method. At most |digest_length| bytes of the resulting digest are written
  // to |digest|.
  bool Sign(QuicStringPiece data, uint8_t* digest, size_t digest_length) const;

  // Verify returns true if |digest| is a valid HMAC of |data| using the key
  // supplied to Init. |digest| must be exactly |DigestLength()| bytes long.
  // Use of this method is strongly recommended over using Sign() with a manual
  // comparison (such as memcmp), as such comparisons may result in
  // side-channel disclosures, such as timing, that undermine the cryptographic
  // integrity.
  bool Verify(QuicStringPiece data, QuicStringPiece digest) const;

 private:
  std::vector<uint8_t> key_;

  DISALLOW_COPY_AND_ASSIGN(HMAC);
};

}  // namespace crypto
}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_BASE_HMAC_H_
