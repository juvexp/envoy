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

#ifndef GFE_QUIC_CORE_CRYPTO_PROOF_VERIFIER_H_
#define GFE_QUIC_CORE_CRYPTO_PROOF_VERIFIER_H_

#include <memory>
#include <vector>

#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_types.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {

// ProofVerifyDetails is an abstract class that acts as a container for any
// implementation specific details that a ProofVerifier wishes to return. These
// details are saved in the CachedState for the origin in question.
class QUIC_EXPORT_PRIVATE ProofVerifyDetails {
 public:
  virtual ~ProofVerifyDetails() {}

  // Returns an new ProofVerifyDetails object with the same contents
  // as this one.
  virtual ProofVerifyDetails* Clone() const = 0;
};

// ProofVerifyContext is an abstract class that acts as a container for any
// implementation specific context that a ProofVerifier needs.
class QUIC_EXPORT_PRIVATE ProofVerifyContext {
 public:
  virtual ~ProofVerifyContext() {}
};

// ProofVerifierCallback provides a generic mechanism for a ProofVerifier to
// call back after an asynchronous verification.
class QUIC_EXPORT_PRIVATE ProofVerifierCallback {
 public:
  virtual ~ProofVerifierCallback() {}

  // Run is called on the original thread to mark the completion of an
  // asynchonous verification. If |ok| is true then the certificate is valid
  // and |error_details| is unused. Otherwise, |error_details| contains a
  // description of the error. |details| contains implementation-specific
  // details of the verification. |Run| may take ownership of |details| by
  // calling |release| on it.
  virtual void Run(bool ok,
                   const QuicString& error_details,
                   std::unique_ptr<ProofVerifyDetails>* details) = 0;
};

// A ProofVerifier checks the signature on a server config, and the certificate
// chain that backs the public key.
class QUIC_EXPORT_PRIVATE ProofVerifier {
 public:
  virtual ~ProofVerifier() {}

  // VerifyProof checks that |signature| is a valid signature of
  // |server_config| by the public key in the leaf certificate of |certs|, and
  // that |certs| is a valid chain for |hostname|. On success, it returns
  // QUIC_SUCCESS. On failure, it returns QUIC_FAILURE and sets |*error_details|
  // to a description of the problem. In either case it may set |*details|,
  // which the caller takes ownership of.
  //
  // |context| specifies an implementation specific struct (which may be nullptr
  // for some implementations) that provides useful information for the
  // verifier, e.g. logging handles.
  //
  // This function may also return QUIC_PENDING, in which case the ProofVerifier
  // will call back, on the original thread, via |callback| when complete.
  //
  // The signature uses SHA-256 as the hash function and PSS padding in the
  // case of RSA.
  virtual QuicAsyncStatus VerifyProof(
      const QuicString& hostname,
      const uint16_t port,
      const QuicString& server_config,
      QuicTransportVersion transport_version,
      QuicStringPiece chlo_hash,
      const std::vector<QuicString>& certs,
      const QuicString& cert_sct,
      const QuicString& signature,
      const ProofVerifyContext* context,
      QuicString* error_details,
      std::unique_ptr<ProofVerifyDetails>* details,
      std::unique_ptr<ProofVerifierCallback> callback) = 0;

  // VerifyCertChain checks that |certs| is a valid chain for |hostname|. On
  // success, it returns QUIC_SUCCESS. On failure, it returns QUIC_FAILURE and
  // sets |*error_details| to a description of the problem. In either case it
  // may set |*details|, which the caller takes ownership of.
  //
  // |context| specifies an implementation specific struct (which may be nullptr
  // for some implementations) that provides useful information for the
  // verifier, e.g. logging handles.
  //
  // This function may also return QUIC_PENDING, in which case the ProofVerifier
  // will call back, on the original thread, via |callback| when complete.
  // In this case, the ProofVerifier will take ownership of |callback|.
  virtual QuicAsyncStatus VerifyCertChain(
      const QuicString& hostname,
      const std::vector<QuicString>& certs,
      const ProofVerifyContext* context,
      QuicString* error_details,
      std::unique_ptr<ProofVerifyDetails>* details,
      std::unique_ptr<ProofVerifierCallback> callback) = 0;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_PROOF_VERIFIER_H_
