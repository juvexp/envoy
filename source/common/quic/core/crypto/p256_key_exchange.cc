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

#include "common/quic/core/crypto/p256_key_exchange.h"

#include <cstdint>
#include <memory>
#include <utility>

#include "common/quic/platform/api/quic_logging.h"
#include "common/quic/platform/api/quic_string.h"
#include "third_party/openssl/ec.h"
#include "third_party/openssl/ecdh.h"
#include "third_party/openssl/err.h"
#include "third_party/openssl/evp.h"

namespace gfe_quic {

P256KeyExchange::P256KeyExchange(bssl::UniquePtr<EC_KEY> private_key,
                                 const uint8_t* public_key)
    : private_key_(std::move(private_key)) {
  memcpy(public_key_, public_key, sizeof(public_key_));
}

P256KeyExchange::~P256KeyExchange() {}

// static
P256KeyExchange* P256KeyExchange::New(QuicStringPiece key) {
  if (key.empty()) {
    QUIC_DLOG(INFO) << "Private key is empty";
    return nullptr;
  }

  const uint8_t* keyp = reinterpret_cast<const uint8_t*>(key.data());
  bssl::UniquePtr<EC_KEY> private_key(
      d2i_ECPrivateKey(nullptr, &keyp, key.size()));
  if (!private_key.get() || !EC_KEY_check_key(private_key.get())) {
    QUIC_DLOG(INFO) << "Private key is invalid.";
    return nullptr;
  }

  uint8_t public_key[kUncompressedP256PointBytes];
  if (EC_POINT_point2oct(EC_KEY_get0_group(private_key.get()),
                         EC_KEY_get0_public_key(private_key.get()),
                         POINT_CONVERSION_UNCOMPRESSED, public_key,
                         sizeof(public_key), nullptr) != sizeof(public_key)) {
    QUIC_DLOG(INFO) << "Can't get public key.";
    return nullptr;
  }

  return new P256KeyExchange(std::move(private_key), public_key);
}

// static
QuicString P256KeyExchange::NewPrivateKey() {
  bssl::UniquePtr<EC_KEY> key(EC_KEY_new_by_curve_name(NID_X9_62_prime256v1));
  if (!key.get() || !EC_KEY_generate_key(key.get())) {
    QUIC_DLOG(INFO) << "Can't generate a new private key.";
    return QuicString();
  }

  int key_len = i2d_ECPrivateKey(key.get(), nullptr);
  if (key_len <= 0) {
    QUIC_DLOG(INFO) << "Can't convert private key to string";
    return QuicString();
  }
  std::unique_ptr<uint8_t[]> private_key(new uint8_t[key_len]);
  uint8_t* keyp = private_key.get();
  if (!i2d_ECPrivateKey(key.get(), &keyp)) {
    QUIC_DLOG(INFO) << "Can't convert private key to string.";
    return QuicString();
  }
  return QuicString(reinterpret_cast<char*>(private_key.get()), key_len);
}

KeyExchange* P256KeyExchange::NewKeyPair(QuicRandom* /*rand*/) const {
  // TODO: avoid the serialisation/deserialisation in this function.
  const QuicString private_value = NewPrivateKey();
  return P256KeyExchange::New(private_value);
}

bool P256KeyExchange::CalculateSharedKey(QuicStringPiece peer_public_value,
                                         QuicString* out_result) const {
  if (peer_public_value.size() != kUncompressedP256PointBytes) {
    QUIC_DLOG(INFO) << "Peer public value is invalid";
    return false;
  }

  bssl::UniquePtr<EC_POINT> point(
      EC_POINT_new(EC_KEY_get0_group(private_key_.get())));
  if (!point.get() ||
      !EC_POINT_oct2point(/* also test if point is on curve */
                          EC_KEY_get0_group(private_key_.get()), point.get(),
                          reinterpret_cast<const uint8_t*>(
                              peer_public_value.data()),
                          peer_public_value.size(), nullptr)) {
    QUIC_DLOG(INFO) << "Can't convert peer public value to curve point.";
    return false;
  }

  uint8_t result[kP256FieldBytes];
  if (ECDH_compute_key(result, sizeof(result), point.get(), private_key_.get(),
                       nullptr) != sizeof(result)) {
    QUIC_DLOG(INFO) << "Can't compute ECDH shared key.";
    return false;
  }

  out_result->assign(reinterpret_cast<char*>(result), sizeof(result));
  return true;
}

QuicStringPiece P256KeyExchange::public_value() const {
  return QuicStringPiece(reinterpret_cast<const char*>(public_key_),
                         sizeof(public_key_));
}

QuicTag P256KeyExchange::tag() const {
  return kP256;
}

}  // namespace gfe_quic
