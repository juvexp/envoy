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

#ifndef GFE_QUIC_CORE_CRYPTO_KEY_EXCHANGE_H_
#define GFE_QUIC_CORE_CRYPTO_KEY_EXCHANGE_H_

#include "common/quic/core/crypto/crypto_protocol.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {

class QuicRandom;

// KeyExchange is an abstract class that provides an interface to a
// key-exchange primitive.
class QUIC_EXPORT_PRIVATE KeyExchange {
 public:
  virtual ~KeyExchange() {}

  // NewKeyPair generates a new public, private key pair. The caller takes
  // ownership of the return value. (This is intended for servers that need to
  // generate forward-secure keys.)
  virtual KeyExchange* NewKeyPair(QuicRandom* rand) const = 0;

  // CalculateSharedKey computes the shared key between the local private key
  // (which is implicitly known by a KeyExchange object) and a public value
  // from the peer.
  virtual bool CalculateSharedKey(QuicStringPiece peer_public_value,
                                  QuicString* shared_key) const = 0;

  // public_value returns the local public key which can be sent to a peer in
  // order to complete a key exchange. The returned QuicStringPiece is a
  // reference to a member of the KeyExchange and is only valid for as long as
  // the KeyExchange exists.
  virtual QuicStringPiece public_value() const = 0;

  // tag returns the tag value that identifies this key exchange function.
  virtual QuicTag tag() const = 0;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_KEY_EXCHANGE_H_
