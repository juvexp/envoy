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

#ifndef GFE_QUIC_CORE_CRYPTO_P256_KEY_EXCHANGE_H_
#define GFE_QUIC_CORE_CRYPTO_P256_KEY_EXCHANGE_H_

#include <cstdint>

#include "base/macros.h"
#include "common/quic/core/crypto/key_exchange.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"
#include "third_party/openssl/base.h"

namespace gfe_quic {

// P256KeyExchange implements a KeyExchange using elliptic-curve
// Diffie-Hellman on NIST P-256.
class QUIC_EXPORT_PRIVATE P256KeyExchange : public KeyExchange {
 public:
  ~P256KeyExchange() override;

  // New creates a new key exchange object from a private key. If
  // |private_key| is invalid, nullptr is returned.
  static P256KeyExchange* New(QuicStringPiece private_key);

  // |NewPrivateKey| returns a private key, suitable for passing to |New|.
  // If |NewPrivateKey| can't generate a private key, it returns an empty
  // string.
  static QuicString NewPrivateKey();

  // KeyExchange interface.
  KeyExchange* NewKeyPair(QuicRandom* rand) const override;
  bool CalculateSharedKey(QuicStringPiece peer_public_value,
                          QuicString* shared_key) const override;
  QuicStringPiece public_value() const override;
  QuicTag tag() const override;

 private:
  enum {
    // A P-256 field element consists of 32 bytes.
    kP256FieldBytes = 32,
    // A P-256 point in uncompressed form consists of 0x04 (to denote
    // that the point is uncompressed) followed by two, 32-byte field
    // elements.
    kUncompressedP256PointBytes = 1 + 2 * kP256FieldBytes,
    // The first byte in an uncompressed P-256 point.
    kUncompressedECPointForm = 0x04,
  };

  // P256KeyExchange wraps |private_key|, and expects |public_key| consists of
  // |kUncompressedP256PointBytes| bytes.
  P256KeyExchange(bssl::UniquePtr<EC_KEY> private_key,
                  const uint8_t* public_key);

  bssl::UniquePtr<EC_KEY> private_key_;
  // The public key stored as an uncompressed P-256 point.
  uint8_t public_key_[kUncompressedP256PointBytes];

  DISALLOW_COPY_AND_ASSIGN(P256KeyExchange);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_P256_KEY_EXCHANGE_H_
