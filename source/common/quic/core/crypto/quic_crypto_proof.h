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

#ifndef GFE_QUIC_CORE_CRYPTO_QUIC_CRYPTO_PROOF_H_
#define GFE_QUIC_CORE_CRYPTO_QUIC_CRYPTO_PROOF_H_

#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {

// Contains the crypto-related data provided by ProofSource
struct QUIC_EXPORT_PRIVATE QuicCryptoProof {
  QuicCryptoProof();

  // Signature generated by ProofSource
  QuicString signature;
  // SCTList (RFC6962) to be sent to the client, if it supports receiving it.
  QuicString leaf_cert_scts;
  // Should the Expect-CT header be sent on the connection where the
  // certificate is used.
  bool send_expect_ct_header;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_QUIC_CRYPTO_PROOF_H_
