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

#ifndef GFE_QUIC_CORE_CRYPTO_EPHEMERAL_KEY_SOURCE_H_
#define GFE_QUIC_CORE_CRYPTO_EPHEMERAL_KEY_SOURCE_H_

#include "common/quic/core/quic_time.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {

class KeyExchange;
class QuicRandom;

// EphemeralKeySource manages and rotates ephemeral keys as they can be reused
// for several connections in a short space of time. Since the implementation
// of this may involve locking or thread-local data, this interface abstracts
// that away.
class QUIC_EXPORT_PRIVATE EphemeralKeySource {
 public:
  virtual ~EphemeralKeySource() {}

  // CalculateForwardSecureKey generates an ephemeral public/private key pair
  // using the algorithm |key_exchange|, sets |*public_value| to the public key
  // and returns the shared key between |peer_public_value| and the private
  // key. |*public_value| will be sent to the peer to be used with the peer's
  // private key.
  virtual QuicString CalculateForwardSecureKey(
      const KeyExchange* key_exchange,
      QuicRandom* rand,
      QuicTime now,
      QuicStringPiece peer_public_value,
      QuicString* public_value) = 0;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_EPHEMERAL_KEY_SOURCE_H_
