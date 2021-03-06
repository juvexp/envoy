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

#include "common/quic/core/quic_crypto_handshaker.h"

#include "common/quic/core/quic_session.h"

namespace gfe_quic {

#define ENDPOINT                                                               \
  (session()->perspective() == Perspective::IS_SERVER ? "Server: " : "Client:" \
                                                                     " ")

QuicCryptoHandshaker::QuicCryptoHandshaker(QuicCryptoStream* stream,
                                           QuicSession* session)
    : stream_(stream), session_(session), last_sent_handshake_message_tag_(0) {
  crypto_framer_.set_visitor(this);
}

QuicCryptoHandshaker::~QuicCryptoHandshaker() {}

void QuicCryptoHandshaker::SendHandshakeMessage(
    const CryptoHandshakeMessage& message) {
  QUIC_DVLOG(1) << ENDPOINT << "Sending "
                << message.DebugString(session()->perspective());
  session()->NeuterUnencryptedData();
  session()->OnCryptoHandshakeMessageSent(message);
  last_sent_handshake_message_tag_ = message.tag();
  const QuicData& data = message.GetSerialized(session()->perspective());
  stream_->WriteOrBufferData(QuicStringPiece(data.data(), data.length()), false,
                             nullptr);
}

void QuicCryptoHandshaker::OnError(CryptoFramer* framer) {
  QUIC_DLOG(WARNING) << "Error processing crypto data: "
                     << QuicErrorCodeToString(framer->error());
}

void QuicCryptoHandshaker::OnHandshakeMessage(
    const CryptoHandshakeMessage& message) {
  QUIC_DVLOG(1) << ENDPOINT << "Received "
                << message.DebugString(session()->perspective());
  session()->OnCryptoHandshakeMessageReceived(message);
}

CryptoMessageParser* QuicCryptoHandshaker::crypto_message_parser() {
  return &crypto_framer_;
}

}  // namespace gfe_quic
