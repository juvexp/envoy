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

#ifndef GFE_QUIC_CORE_CRYPTO_CURVE25519_KEY_EXCHANGE_H_
#define GFE_QUIC_CORE_CRYPTO_CURVE25519_KEY_EXCHANGE_H_

#include <cstdint>

#include "common/quic/core/crypto/key_exchange.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {

class QuicRandom;

// Curve25519KeyExchange implements a KeyExchange using elliptic-curve
// Diffie-Hellman on curve25519. See http://cr.yp.to/ecdh.html
class QUIC_EXPORT_PRIVATE Curve25519KeyExchange : public KeyExchange {
 public:
  ~Curve25519KeyExchange() override;

  // New creates a new object from a private key. If the private key is
  // invalid, nullptr is returned.
  static Curve25519KeyExchange* New(QuicStringPiece private_key);

  // NewPrivateKey returns a private key, generated from |rand|, suitable for
  // passing to |New|.
  static QuicString NewPrivateKey(QuicRandom* rand);

  // KeyExchange interface.
  KeyExchange* NewKeyPair(QuicRandom* rand) const override;
  bool CalculateSharedKey(QuicStringPiece peer_public_value,
                          QuicString* shared_key) const override;
  QuicStringPiece public_value() const override;
  QuicTag tag() const override;

 private:
  Curve25519KeyExchange();

  uint8_t private_key_[32];
  uint8_t public_key_[32];
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_CURVE25519_KEY_EXCHANGE_H_
