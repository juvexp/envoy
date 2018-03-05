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

#include "common/quic/core/crypto/quic_encrypter.h"

#include "common/quic/core/crypto/aes_128_gcm_12_encrypter.h"
#include "common/quic/core/crypto/aes_128_gcm_encrypter.h"
#include "common/quic/core/crypto/aes_256_gcm_encrypter.h"
#include "common/quic/core/crypto/chacha20_poly1305_encrypter.h"
#include "common/quic/core/crypto/chacha20_poly1305_tls_encrypter.h"
#include "common/quic/core/crypto/crypto_protocol.h"
#include "common/quic/core/crypto/null_encrypter.h"
#include "common/quic/platform/api/quic_bug_tracker.h"
#include "common/quic/platform/api/quic_logging.h"
#include "third_party/openssl/tls1.h"

namespace gfe_quic {

// static
QuicEncrypter* QuicEncrypter::Create(QuicTag algorithm) {
  switch (algorithm) {
    case kAESG:
      return new Aes128Gcm12Encrypter();
    case kCC20:
      return new ChaCha20Poly1305Encrypter();
    default:
      QUIC_LOG(FATAL) << "Unsupported algorithm: " << algorithm;
      return nullptr;
  }
}

// static
QuicEncrypter* QuicEncrypter::CreateFromCipherSuite(uint32_t cipher_suite) {
  QuicEncrypter* encrypter;
  switch (cipher_suite) {
    case TLS1_CK_AES_128_GCM_SHA256:
      encrypter = new Aes128GcmEncrypter();
      break;
    case TLS1_CK_AES_256_GCM_SHA384:
      encrypter = new Aes256GcmEncrypter();
      break;
    case TLS1_CK_CHACHA20_POLY1305_SHA256:
      encrypter = new ChaCha20Poly1305TlsEncrypter();
      break;
    default:
      QUIC_BUG << "TLS cipher suite is unknown to QUIC";
      return nullptr;
  }
  return encrypter;
}

}  // namespace gfe_quic
