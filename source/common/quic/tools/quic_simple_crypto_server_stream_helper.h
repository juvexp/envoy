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

#ifndef GFE_QUIC_TOOLS_QUIC_SIMPLE_CRYPTO_SERVER_STREAM_HELPER_H_
#define GFE_QUIC_TOOLS_QUIC_SIMPLE_CRYPTO_SERVER_STREAM_HELPER_H_

#include "common/quic/core/crypto/quic_random.h"
#include "common/quic/core/quic_crypto_server_stream.h"

namespace gfe_quic {

// Simple helper for server crypto streams which generates a new random
// connection ID for stateless rejects.
class QuicSimpleCryptoServerStreamHelper
    : public QuicCryptoServerStream::Helper {
 public:
  explicit QuicSimpleCryptoServerStreamHelper(QuicRandom* random);

  ~QuicSimpleCryptoServerStreamHelper() override;

  QuicConnectionId GenerateConnectionIdForReject(
      QuicConnectionId /*connection_id*/) const override;

  bool CanAcceptClientHello(const CryptoHandshakeMessage& message,
                            const QuicSocketAddress& self_address,
                            string* error_details) const override;

 private:
  QuicRandom* random_;  // Unowned.
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_TOOLS_QUIC_SIMPLE_CRYPTO_SERVER_STREAM_HELPER_H_
