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

#ifndef GFE_QUIC_TEST_TOOLS_FAILING_PROOF_SOURCE_H_
#define GFE_QUIC_TEST_TOOLS_FAILING_PROOF_SOURCE_H_

#include "common/quic/core/crypto/proof_source.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {
namespace test {

class FailingProofSource : public ProofSource {
 public:
  void GetProof(const QuicSocketAddress& server_address,
                const string& hostname,
                const string& server_config,
                QuicTransportVersion transport_version,
                QuicStringPiece chlo_hash,
                std::unique_ptr<Callback> callback) override;

  QuicReferenceCountedPointer<Chain> GetCertChain(
      const QuicSocketAddress& server_address,
      const string& hostname) override;

  void ComputeTlsSignature(
      const QuicSocketAddress& server_address,
      const string& hostname,
      uint16_t signature_algorithm,
      QuicStringPiece in,
      std::unique_ptr<SignatureCallback> callback) override;
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_FAILING_PROOF_SOURCE_H_
