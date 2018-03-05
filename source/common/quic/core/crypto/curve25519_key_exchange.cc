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

#include "common/quic/core/crypto/curve25519_key_exchange.h"

#include <cstdint>

#include "common/quic/core/crypto/quic_random.h"
#include "common/quic/platform/api/quic_string.h"
#include "third_party/openssl/curve25519.h"

namespace gfe_quic {

Curve25519KeyExchange::Curve25519KeyExchange() {}

Curve25519KeyExchange::~Curve25519KeyExchange() {}

// static
Curve25519KeyExchange* Curve25519KeyExchange::New(QuicStringPiece private_key) {
  Curve25519KeyExchange* ka;
  // We don't want to #include the BoringSSL headers in the public header file,
  // so we use literals for the sizes of private_key_ and public_key_. Here we
  // assert that those values are equal to the values from the BoringSSL
  // header.
  static_assert(sizeof(ka->private_key_) == X25519_PRIVATE_KEY_LEN,
                "header out of sync");
  static_assert(sizeof(ka->public_key_) == X25519_PUBLIC_VALUE_LEN,
                "header out of sync");

  if (private_key.size() != X25519_PRIVATE_KEY_LEN) {
    return nullptr;
  }

  ka = new Curve25519KeyExchange();
  memcpy(ka->private_key_, private_key.data(), X25519_PRIVATE_KEY_LEN);
  X25519_public_from_private(ka->public_key_, ka->private_key_);
  return ka;
}

// static
QuicString Curve25519KeyExchange::NewPrivateKey(QuicRandom* rand) {
  uint8_t private_key[X25519_PRIVATE_KEY_LEN];
  rand->RandBytes(private_key, sizeof(private_key));
  return QuicString(reinterpret_cast<char*>(private_key), sizeof(private_key));
}

KeyExchange* Curve25519KeyExchange::NewKeyPair(QuicRandom* rand) const {
  const QuicString private_value = NewPrivateKey(rand);
  return Curve25519KeyExchange::New(private_value);
}

bool Curve25519KeyExchange::CalculateSharedKey(
    QuicStringPiece peer_public_value,
    QuicString* out_result) const {
  if (peer_public_value.size() != X25519_PUBLIC_VALUE_LEN) {
    return false;
  }

  uint8_t result[X25519_PUBLIC_VALUE_LEN];
  if (!X25519(result, private_key_,
              reinterpret_cast<const uint8_t*>(peer_public_value.data()))) {
    return false;
  }

  out_result->assign(reinterpret_cast<char*>(result), sizeof(result));
  return true;
}

QuicStringPiece Curve25519KeyExchange::public_value() const {
  return QuicStringPiece(reinterpret_cast<const char*>(public_key_),
                         sizeof(public_key_));
}

QuicTag Curve25519KeyExchange::tag() const {
  return kC255;
}

}  // namespace gfe_quic
