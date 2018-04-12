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

#include "common/quic/core/quic_crypto_client_stream.h"

#include <memory>

#include "common/quic/core/crypto/crypto_protocol.h"
#include "common/quic/core/crypto/crypto_utils.h"
#include "common/quic/core/crypto/null_encrypter.h"
#include "common/quic/core/quic_crypto_client_handshaker.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_session.h"
#include "common/quic/core/quic_utils.h"
#include "common/quic/core/tls_client_handshaker.h"
#include "common/quic/platform/api/quic_flags.h"
#include "common/quic/platform/api/quic_logging.h"
#include "common/quic/platform/api/quic_ptr_util.h"
#include "common/quic/platform/api/quic_str_cat.h"
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {

const int QuicCryptoClientStream::kMaxClientHellos;

QuicCryptoClientStreamBase::QuicCryptoClientStreamBase(QuicSession* session)
    : QuicCryptoStream(session) {}

QuicCryptoClientStream::QuicCryptoClientStream(
    const QuicServerId& server_id,
    QuicSession* session,
    ProofVerifyContext* verify_context,
    QuicCryptoClientConfig* crypto_config,
    ProofHandler* proof_handler)
    : QuicCryptoClientStreamBase(session) {
  DCHECK_EQ(Perspective::IS_CLIENT, session->connection()->perspective());
  switch (session->connection()->version().handshake_protocol) {
    case PROTOCOL_QUIC_CRYPTO:
      handshaker_ = QuicMakeUnique<QuicCryptoClientHandshaker>(
          server_id, this, session, verify_context, crypto_config,
          proof_handler);
      break;
    case PROTOCOL_TLS1_3:
      handshaker_ = QuicMakeUnique<TlsClientHandshaker>(
          this, session, server_id, crypto_config->proof_verifier(),
          crypto_config->ssl_ctx(), verify_context);
      break;
    case PROTOCOL_UNSUPPORTED:
      QUIC_BUG << "Attempting to create QuicCryptoClientStream for unknown "
                  "handshake protocol";
  }
}

QuicCryptoClientStream::~QuicCryptoClientStream() {}

bool QuicCryptoClientStream::CryptoConnect() {
  return handshaker_->CryptoConnect();
}

int QuicCryptoClientStream::num_sent_client_hellos() const {
  return handshaker_->num_sent_client_hellos();
}

int QuicCryptoClientStream::num_scup_messages_received() const {
  return handshaker_->num_scup_messages_received();
}

bool QuicCryptoClientStream::encryption_established() const {
  return handshaker_->encryption_established();
}

bool QuicCryptoClientStream::handshake_confirmed() const {
  return handshaker_->handshake_confirmed();
}

const QuicCryptoNegotiatedParameters&
QuicCryptoClientStream::crypto_negotiated_params() const {
  return handshaker_->crypto_negotiated_params();
}

CryptoMessageParser* QuicCryptoClientStream::crypto_message_parser() {
  return handshaker_->crypto_message_parser();
}

bool QuicCryptoClientStream::WasChannelIDSent() const {
  return handshaker_->WasChannelIDSent();
}

bool QuicCryptoClientStream::WasChannelIDSourceCallbackRun() const {
  return handshaker_->WasChannelIDSourceCallbackRun();
}

QuicLongHeaderType QuicCryptoClientStream::GetLongHeaderType(
    QuicStreamOffset offset) const {
  return handshaker_->GetLongHeaderType(offset);
}

QuicString QuicCryptoClientStream::chlo_hash() const {
  return handshaker_->chlo_hash();
}

}  // namespace gfe_quic
