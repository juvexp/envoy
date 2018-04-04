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

#include "common/quic/core/crypto/channel_id.h"

#include <cstdint>

#include "third_party/openssl/bn.h"
#include "third_party/openssl/ec.h"
#include "third_party/openssl/ecdsa.h"
#include "third_party/openssl/nid.h"
#include "third_party/openssl/sha.h"

namespace gfe_quic {

// static
const char ChannelIDVerifier::kContextStr[] = "QUIC ChannelID";
// static
const char ChannelIDVerifier::kClientToServerStr[] = "client -> server";

// static
bool ChannelIDVerifier::Verify(QuicStringPiece key,
                               QuicStringPiece signed_data,
                               QuicStringPiece signature) {
  return VerifyRaw(key, signed_data, signature, true);
}

// static
bool ChannelIDVerifier::VerifyRaw(QuicStringPiece key,
                                  QuicStringPiece signed_data,
                                  QuicStringPiece signature,
                                  bool is_channel_id_signature) {
  if (key.size() != 32 * 2 || signature.size() != 32 * 2) {
    return false;
  }

  bssl::UniquePtr<EC_GROUP> p256(
      EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1));
  if (p256.get() == nullptr) {
    return false;
  }

  bssl::UniquePtr<BIGNUM> x(BN_new()), y(BN_new()), r(BN_new()), s(BN_new());

  ECDSA_SIG sig;
  sig.r = r.get();
  sig.s = s.get();

  const uint8_t* key_bytes = reinterpret_cast<const uint8_t*>(key.data());
  const uint8_t* signature_bytes =
      reinterpret_cast<const uint8_t*>(signature.data());

  if (BN_bin2bn(key_bytes + 0, 32, x.get()) == nullptr ||
      BN_bin2bn(key_bytes + 32, 32, y.get()) == nullptr ||
      BN_bin2bn(signature_bytes + 0, 32, sig.r) == nullptr ||
      BN_bin2bn(signature_bytes + 32, 32, sig.s) == nullptr) {
    return false;
  }

  bssl::UniquePtr<EC_POINT> point(EC_POINT_new(p256.get()));
  if (point.get() == nullptr ||
      !EC_POINT_set_affine_coordinates_GFp(p256.get(), point.get(), x.get(),
                                           y.get(), nullptr)) {
    return false;
  }

  bssl::UniquePtr<EC_KEY> ecdsa_key(EC_KEY_new());
  if (ecdsa_key.get() == nullptr ||
      !EC_KEY_set_group(ecdsa_key.get(), p256.get()) ||
      !EC_KEY_set_public_key(ecdsa_key.get(), point.get())) {
    return false;
  }

  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  if (is_channel_id_signature) {
    SHA256_Update(&sha256, kContextStr, strlen(kContextStr) + 1);
    SHA256_Update(&sha256, kClientToServerStr, strlen(kClientToServerStr) + 1);
  }
  SHA256_Update(&sha256, signed_data.data(), signed_data.size());

  unsigned char digest[SHA256_DIGEST_LENGTH];
  SHA256_Final(digest, &sha256);

  return ECDSA_do_verify(digest, sizeof(digest), &sig, ecdsa_key.get()) == 1;
}

}  // namespace gfe_quic