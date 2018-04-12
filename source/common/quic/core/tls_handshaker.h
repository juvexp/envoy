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

#ifndef GFE_QUIC_CORE_TLS_HANDSHAKER_H_
#define GFE_QUIC_CORE_TLS_HANDSHAKER_H_

#include "common/quic/core/crypto/crypto_handshake.h"
#include "common/quic/core/crypto/quic_decrypter.h"
#include "common/quic/core/crypto/quic_encrypter.h"
#include "common/quic/core/crypto/quic_tls_adapter.h"
#include "common/quic/platform/api/quic_export.h"
#include "third_party/openssl/base.h"

namespace gfe_quic {

class QuicCryptoStream;
class QuicSession;

// Base class for TlsClientHandshaker and TlsServerHandshaker. TlsHandshaker
// provides functionality common to both the client and server, such as moving
// messages between the TLS stack and the QUIC crypto stream, and handling
// derivation of secrets.
class QUIC_EXPORT_PRIVATE TlsHandshaker : public QuicTlsAdapter::Visitor {
 public:
  // TlsHandshaker does not take ownership of any of its arguments; they must
  // outlive the TlsHandshaker.
  TlsHandshaker(QuicCryptoStream* stream,
                QuicSession* session,
                SSL_CTX* ssl_ctx);

  ~TlsHandshaker() override;

  // From QuicTlsAdapter::Visitor
  void OnDataAvailableForBIO() override;
  void OnDataReceivedFromBIO(const QuicStringPiece& data) override;

  // From QuicCryptoStream
  virtual QuicLongHeaderType GetLongHeaderType(
      QuicStreamOffset offset) const = 0;
  virtual bool encryption_established() const = 0;
  virtual bool handshake_confirmed() const = 0;
  virtual const QuicCryptoNegotiatedParameters& crypto_negotiated_params()
      const = 0;
  virtual CryptoMessageParser* crypto_message_parser();

 protected:
  virtual void AdvanceHandshake() = 0;

  // Creates an SSL_CTX and configures it with the options that are appropriate
  // for both client and server. The caller is responsible for ownership of the
  // newly created struct.
  static bssl::UniquePtr<SSL_CTX> CreateSslCtx();

  // From a given SSL* |ssl|, returns a pointer to the TlsHandshaker that it
  // belongs to. This is a helper method for implementing callbacks set on an
  // SSL, as it allows the callback function to find the TlsHandshaker instance
  // and call an instance method.
  static TlsHandshaker* HandshakerFromSsl(const SSL* ssl);

  // Returns the PRF used by the cipher suite negotiated in the TLS handshake.
  const EVP_MD* Prf();

  // Computes the 1-RTT secrets client_pp_secret_0 and server_pp_secret_0 from
  // which the packet protection keys are derived, as defined in
  // draft-ietf-quic-tls section 5.2.2.
  bool DeriveSecrets(std::vector<uint8_t>* client_secret_out,
                     std::vector<uint8_t>* server_secret_out);

  std::unique_ptr<QuicEncrypter> CreateEncrypter(
      const std::vector<uint8_t>& pp_secret);
  std::unique_ptr<QuicDecrypter> CreateDecrypter(
      const std::vector<uint8_t>& pp_secret);

  SSL* ssl() { return ssl_.get(); }
  QuicCryptoStream* stream() { return stream_; }
  QuicSession* session() { return session_; }

 private:
  QuicCryptoStream* stream_;
  QuicSession* session_;
  QuicTlsAdapter bio_adapter_;
  bssl::UniquePtr<SSL> ssl_;

  DISALLOW_COPY_AND_ASSIGN(TlsHandshaker);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_TLS_HANDSHAKER_H_
