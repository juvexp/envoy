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

#ifndef GFE_QUIC_CORE_CHLO_EXTRACTOR_H_
#define GFE_QUIC_CORE_CHLO_EXTRACTOR_H_

#include "common/quic/core/crypto/crypto_handshake_message.h"
#include "common/quic/core/quic_packets.h"

namespace gfe_quic {

// A utility for extracting QUIC Client Hello messages from packets,
// without needs to spin up a full QuicSession.
class ChloExtractor {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}

    // Called when a CHLO message is found in the packets.
    virtual void OnChlo(QuicTransportVersion version,
                        QuicConnectionId connection_id,
                        const CryptoHandshakeMessage& chlo) = 0;
  };

  // Extracts a CHLO message from |packet| and invokes the OnChlo
  // method of |delegate|. Return true if a CHLO message was found,
  // and false otherwise. If non-empty,
  // |create_session_tag_indicators| contains a list of QUIC tags that
  // if found will result in the session being created early, to
  // enable support for multi-packet CHLOs.
  static bool Extract(const QuicEncryptedPacket& packet,
                      const ParsedQuicVersionVector& versions,
                      const QuicTagVector& create_session_tag_indicators,
                      Delegate* delegate);

  ChloExtractor(const ChloExtractor&) = delete;
  ChloExtractor operator=(const ChloExtractor&) = delete;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CHLO_EXTRACTOR_H_
