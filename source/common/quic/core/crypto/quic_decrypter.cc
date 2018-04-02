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

#include "common/quic/core/crypto/quic_decrypter.h"

#include "common/quic/core/crypto/aes_128_gcm_12_decrypter.h"
#include "common/quic/core/crypto/aes_128_gcm_decrypter.h"
#include "common/quic/core/crypto/aes_256_gcm_decrypter.h"
#include "common/quic/core/crypto/base/hkdf.h"
#include "common/quic/core/crypto/chacha20_poly1305_decrypter.h"
#include "common/quic/core/crypto/chacha20_poly1305_tls_decrypter.h"
#include "common/quic/core/crypto/crypto_protocol.h"
#include "common/quic/core/crypto/null_decrypter.h"
#include "common/quic/platform/api/quic_bug_tracker.h"
#include "common/quic/platform/api/quic_logging.h"
#include "common/quic/platform/api/quic_string.h"
#include "third_party/openssl/tls1.h"

namespace gfe_quic {

// static
QuicDecrypter* QuicDecrypter::Create(QuicTag algorithm) {
  switch (algorithm) {
    case kAESG:
      return new Aes128Gcm12Decrypter();
    case kCC20:
      return new ChaCha20Poly1305Decrypter();
    default:
      QUIC_LOG(FATAL) << "Unsupported algorithm: " << algorithm;
      return nullptr;
  }
}

// static
QuicDecrypter* QuicDecrypter::CreateFromCipherSuite(uint32_t cipher_suite) {
  QuicDecrypter* decrypter;
  switch (cipher_suite) {
    case TLS1_CK_AES_128_GCM_SHA256:
      decrypter = new Aes128GcmDecrypter();
      break;
    case TLS1_CK_AES_256_GCM_SHA384:
      decrypter = new Aes256GcmDecrypter();
      break;
    case TLS1_CK_CHACHA20_POLY1305_SHA256:
      decrypter = new ChaCha20Poly1305TlsDecrypter();
      break;
    default:
      QUIC_BUG << "TLS cipher suite is unknown to QUIC";
      return nullptr;
  }
  return decrypter;
}

// static
void QuicDecrypter::DiversifyPreliminaryKey(QuicStringPiece preliminary_key,
                                            QuicStringPiece nonce_prefix,
                                            const DiversificationNonce& nonce,
                                            size_t key_size,
                                            size_t nonce_prefix_size,
                                            QuicString* out_key,
                                            QuicString* out_nonce_prefix) {
  crypto::HKDF hkdf((string(preliminary_key)) + (string(nonce_prefix)),
                    QuicStringPiece(nonce.data(), nonce.size()),
                    "QUIC key diversification", 0, key_size, 0,
                    nonce_prefix_size, 0);
  *out_key = string(hkdf.server_write_key());
  *out_nonce_prefix = string(hkdf.server_write_iv());
}

}  // namespace gfe_quic
