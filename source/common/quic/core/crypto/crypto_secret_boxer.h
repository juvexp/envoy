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

#ifndef GFE_QUIC_CORE_CRYPTO_CRYPTO_SECRET_BOXER_H_
#define GFE_QUIC_CORE_CRYPTO_CRYPTO_SECRET_BOXER_H_

#include <cstddef>
#include <memory>
#include <vector>

#include "base/macros.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_mutex.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {

class QuicRandom;

// CryptoSecretBoxer encrypts small chunks of plaintext (called 'boxing') and
// then, later, can authenticate+decrypt the resulting boxes. This object is
// thread-safe.
class QUIC_EXPORT_PRIVATE CryptoSecretBoxer {
 public:
  CryptoSecretBoxer();
  ~CryptoSecretBoxer();

  // GetKeySize returns the number of bytes in a key.
  static size_t GetKeySize();

  // SetKeys sets a list of encryption keys. The first key in the list will be
  // used by |Box|, but all supplied keys will be tried by |Unbox|, to handle
  // key skew across the fleet. This must be called before |Box| or |Unbox|.
  // Keys must be |GetKeySize()| bytes long.
  void SetKeys(const std::vector<QuicString>& keys);

  // Box encrypts |plaintext| using a random nonce generated from |rand| and
  // returns the resulting ciphertext. Since an authenticator and nonce are
  // included, the result will be slightly larger than |plaintext|. The first
  // key in the vector supplied to |SetKeys| will be used.
  QuicString Box(QuicRandom* rand, QuicStringPiece plaintext) const;

  // Unbox takes the result of a previous call to |Box| in |ciphertext| and
  // authenticates+decrypts it. If |ciphertext| cannot be decrypted with any of
  // the supplied keys, the function returns false. Otherwise, |out_storage| is
  // used to store the result and |out| is set to point into |out_storage| and
  // contains the original plaintext.
  bool Unbox(QuicStringPiece ciphertext,
             QuicString* out_storage,
             QuicStringPiece* out) const;

 private:
  struct State;

  mutable QuicMutex lock_;

  // state_ is an opaque pointer to whatever additional state the concrete
  // implementation of CryptoSecretBoxer requires.
  std::unique_ptr<State> state_ GUARDED_BY(lock_);

  DISALLOW_COPY_AND_ASSIGN(CryptoSecretBoxer);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_CRYPTO_SECRET_BOXER_H_
