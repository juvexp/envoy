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

#ifndef GFE_QUIC_CORE_QUIC_CRYPTO_HANDSHAKER_H_
#define GFE_QUIC_CORE_QUIC_CRYPTO_HANDSHAKER_H_

#include "common/quic/core/quic_crypto_stream.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

class QUIC_EXPORT_PRIVATE QuicCryptoHandshaker
    : public CryptoFramerVisitorInterface {
 public:
  QuicCryptoHandshaker(QuicCryptoStream* stream, QuicSession* session);

  ~QuicCryptoHandshaker() override;

  // Sends |message| to the peer.
  // TODO: return a success/failure status.
  void SendHandshakeMessage(const CryptoHandshakeMessage& message);

  void OnError(CryptoFramer* framer) override;
  void OnHandshakeMessage(const CryptoHandshakeMessage& message) override;

  CryptoMessageParser* crypto_message_parser();

 protected:
  QuicTag last_sent_handshake_message_tag() const {
    return last_sent_handshake_message_tag_;
  }

 private:
  QuicSession* session() { return session_; }

  QuicCryptoStream* stream_;
  QuicSession* session_;

  CryptoFramer crypto_framer_;

  // Records last sent crypto handshake message tag.
  QuicTag last_sent_handshake_message_tag_;

  DISALLOW_COPY_AND_ASSIGN(QuicCryptoHandshaker);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_CRYPTO_HANDSHAKER_H_
