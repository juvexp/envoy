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

#include "common/quic/core/crypto/base/hkdf.h"

#include <memory>

#include "common/quic/platform/api/quic_logging.h"
#include "third_party/openssl/digest.h"
#include "third_party/openssl/hkdf.h"

namespace gfe_quic {
namespace crypto {

const size_t kSHA256HashLength = 32;
const size_t kMaxKeyMaterialSize = kSHA256HashLength * 256;

HKDF::HKDF(QuicStringPiece secret,
           QuicStringPiece salt,
           QuicStringPiece info,
           size_t key_bytes_to_generate,
           size_t iv_bytes_to_generate,
           size_t subkey_secret_bytes_to_generate)
    : HKDF(secret,
           salt,
           info,
           key_bytes_to_generate,
           key_bytes_to_generate,
           iv_bytes_to_generate,
           iv_bytes_to_generate,
           subkey_secret_bytes_to_generate) {}

HKDF::HKDF(QuicStringPiece secret,
           QuicStringPiece salt,
           QuicStringPiece info,
           size_t client_key_bytes_to_generate,
           size_t server_key_bytes_to_generate,
           size_t client_iv_bytes_to_generate,
           size_t server_iv_bytes_to_generate,
           size_t subkey_secret_bytes_to_generate) {
  const size_t material_length =
      client_key_bytes_to_generate + client_iv_bytes_to_generate +
      server_key_bytes_to_generate + server_iv_bytes_to_generate +
      subkey_secret_bytes_to_generate;
  DCHECK_LT(material_length, kMaxKeyMaterialSize);

  output_.resize(material_length);
  // On Windows, when the size of output_ is zero, dereference of 0'th element
  // results in a crash. C++11 solves this problem by adding a data() getter
  // method to std::vector.
  if (output_.empty()) {
    return;
  }

  ::HKDF(&output_[0], output_.size(), ::EVP_sha256(),
         reinterpret_cast<const uint8_t*>(secret.data()), secret.size(),
         reinterpret_cast<const uint8_t*>(salt.data()), salt.size(),
         reinterpret_cast<const uint8_t*>(info.data()), info.size());

  size_t j = 0;
  if (client_key_bytes_to_generate) {
    client_write_key_ = QuicStringPiece(reinterpret_cast<char*>(&output_[j]),
                                        client_key_bytes_to_generate);
    j += client_key_bytes_to_generate;
  }

  if (server_key_bytes_to_generate) {
    server_write_key_ = QuicStringPiece(reinterpret_cast<char*>(&output_[j]),
                                        server_key_bytes_to_generate);
    j += server_key_bytes_to_generate;
  }

  if (client_iv_bytes_to_generate) {
    client_write_iv_ = QuicStringPiece(reinterpret_cast<char*>(&output_[j]),
                                       client_iv_bytes_to_generate);
    j += client_iv_bytes_to_generate;
  }

  if (server_iv_bytes_to_generate) {
    server_write_iv_ = QuicStringPiece(reinterpret_cast<char*>(&output_[j]),
                                       server_iv_bytes_to_generate);
    j += server_iv_bytes_to_generate;
  }

  if (subkey_secret_bytes_to_generate) {
    subkey_secret_ = QuicStringPiece(reinterpret_cast<char*>(&output_[j]),
                                     subkey_secret_bytes_to_generate);
  }
}

HKDF::~HKDF() {}

}  // namespace crypto
}  // namespace gfe_quic
