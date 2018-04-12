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

#ifndef GFE_QUIC_CORE_TLS_CLIENT_HANDSHAKER_H_
#define GFE_QUIC_CORE_TLS_CLIENT_HANDSHAKER_H_

#include "common/quic/core/crypto/proof_verifier.h"
#include "common/quic/core/crypto/quic_tls_adapter.h"
#include "common/quic/core/quic_crypto_client_stream.h"
#include "common/quic/core/quic_crypto_stream.h"
#include "common/quic/core/tls_handshaker.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string.h"
#include "third_party/openssl/ssl.h"

namespace gfe_quic {

// An implementation of QuicCryptoClientStream::HandshakerDelegate which uses
// TLS 1.3 for the crypto handshake protocol.
class QUIC_EXPORT_PRIVATE TlsClientHandshaker
    : public QuicCryptoClientStream::HandshakerDelegate,
      public TlsHandshaker {
 public:
  TlsClientHandshaker(QuicCryptoStream* stream,
                      QuicSession* session,
                      const QuicServerId& server_id,
                      ProofVerifier* proof_verifier,
                      SSL_CTX* ssl_ctx,
                      // Takes ownership of |verify_context|.
                      ProofVerifyContext* verify_context);

  ~TlsClientHandshaker() override;

  // Creates and configures an SSL_CTX to be used with a TlsClientHandshaker.
  // The caller is responsible for ownership of the newly created struct.
  static bssl::UniquePtr<SSL_CTX> CreateSslCtx();

  // From QuicCryptoClientStream::HandshakerDelegate
  bool CryptoConnect() override;
  int num_sent_client_hellos() const override;
  int num_scup_messages_received() const override;
  bool WasChannelIDSent() const override;
  bool WasChannelIDSourceCallbackRun() const override;
  QuicString chlo_hash() const override;

  // From QuicCryptoClientStream::HandshakerDelegate and TlsHandshaker
  QuicLongHeaderType GetLongHeaderType(QuicStreamOffset offset) const override;
  bool encryption_established() const override;
  bool handshake_confirmed() const override;
  const QuicCryptoNegotiatedParameters& crypto_negotiated_params()
      const override;
  CryptoMessageParser* crypto_message_parser() override;

 private:
  // ProofVerifierCallbackImpl handles the result of an asynchronous certificate
  // verification operation.
  class ProofVerifierCallbackImpl : public ProofVerifierCallback {
   public:
    explicit ProofVerifierCallbackImpl(TlsClientHandshaker* parent);
    ~ProofVerifierCallbackImpl() override;

    // ProofVerifierCallback interface.
    void Run(bool ok,
             const QuicString& error_details,
             std::unique_ptr<ProofVerifyDetails>* details) override;

    // If called, Cancel causes the pending callback to be a no-op.
    void Cancel();

   private:
    TlsClientHandshaker* parent_;
  };

  enum State {
    STATE_IDLE,
    STATE_HANDSHAKE_RUNNING,
    STATE_CERT_VERIFY_PENDING,
    STATE_HANDSHAKE_COMPLETE,
    STATE_CONNECTION_CLOSED,
  } state_ = STATE_IDLE;

  void AdvanceHandshake() override;
  void FinishHandshake();

  void CloseConnection();

  // Certificate verification functions:

  enum ssl_verify_result_t VerifyCert(uint8_t* out_alert);
  // Static method to supply to SSL_set_custom_verify.
  static enum ssl_verify_result_t VerifyCallback(SSL* ssl, uint8_t* out_alert);

  // Takes an SSL* |ssl| and returns a pointer to the TlsClientHandshaker that
  // it belongs to. This is a specialization of
  // TlsHandshaker::HandshakerFromSsl.
  static TlsClientHandshaker* HandshakerFromSsl(SSL* ssl);

  QuicServerId server_id_;

  // Objects used for verifying the server's certificate chain.
  // |proof_verifier_| is owned by the caller of TlsClientHandshaker's
  // constructor.
  ProofVerifier* proof_verifier_;
  std::unique_ptr<ProofVerifyContext> verify_context_;

  // ProofVerifierCallback used for async certificate verification. This object
  // is owned by |proof_verifier_|.
  ProofVerifierCallbackImpl* proof_verify_callback_ = nullptr;
  std::unique_ptr<ProofVerifyDetails> verify_details_;
  enum ssl_verify_result_t verify_result_ = ssl_verify_retry;
  QuicString cert_verify_error_details_;

  bool encryption_established_ = false;
  bool handshake_confirmed_ = false;
  QuicReferenceCountedPointer<QuicCryptoNegotiatedParameters>
      crypto_negotiated_params_;

  DISALLOW_COPY_AND_ASSIGN(TlsClientHandshaker);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_TLS_CLIENT_HANDSHAKER_H_
