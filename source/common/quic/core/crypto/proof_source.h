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

#ifndef GFE_QUIC_CORE_CRYPTO_PROOF_SOURCE_H_
#define GFE_QUIC_CORE_CRYPTO_PROOF_SOURCE_H_

#include <memory>
#include <vector>

#include "common/quic/core/crypto/quic_crypto_proof.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_reference_counted.h"
#include "common/quic/platform/api/quic_socket_address.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {

// ProofSource is an interface by which a QUIC server can obtain certificate
// chains and signatures that prove its identity.
class QUIC_EXPORT_PRIVATE ProofSource {
 public:
  // Chain is a reference-counted wrapper for a vector of stringified
  // certificates.
  struct QUIC_EXPORT_PRIVATE Chain : public QuicReferenceCounted {
    explicit Chain(const std::vector<QuicString>& certs);

    const std::vector<QuicString> certs;

   protected:
    ~Chain() override;

   private:
    DISALLOW_COPY_AND_ASSIGN(Chain);
  };

  // Details is an abstract class which acts as a container for any
  // implementation-specific details that a ProofSource wants to return.
  class Details {
   public:
    virtual ~Details() {}
  };

  // Callback base class for receiving the results of an async call to GetProof.
  class Callback {
   public:
    Callback() {}
    virtual ~Callback() {}

    // Invoked upon completion of GetProof.
    //
    // |ok| indicates whether the operation completed successfully.  If false,
    // the values of the remaining three arguments are undefined.
    //
    // |chain| is a reference-counted pointer to an object representing the
    // certificate chain.
    //
    // |signature| contains the signature of the server config.
    //
    // |leaf_cert_sct| holds the signed timestamp (RFC6962) of the leaf cert.
    //
    // |details| holds a pointer to an object representing the statistics, if
    // any, gathered during the operation of GetProof.  If no stats are
    // available, this will be nullptr.
    virtual void Run(bool ok,
                     const QuicReferenceCountedPointer<Chain>& chain,
                     const QuicCryptoProof& proof,
                     std::unique_ptr<Details> details) = 0;

   private:
    Callback(const Callback&) = delete;
    Callback& operator=(const Callback&) = delete;
  };

  // Base class for signalling the completion of a call to ComputeTlsSignature.
  class SignatureCallback {
   public:
    SignatureCallback() {}
    virtual ~SignatureCallback() = default;

    // Invoked upon completion of ComputeTlsSignature.
    //
    // |ok| indicates whether the operation completed successfully.
    //
    // |signature| contains the signature of the data provided to
    // ComputeTlsSignature. Its value is undefined if |ok| is false.
    virtual void Run(bool ok, QuicString signature) = 0;

   private:
    SignatureCallback(const SignatureCallback&) = delete;
    SignatureCallback& operator=(const SignatureCallback&) = delete;
  };

  virtual ~ProofSource() {}

  // GetProof finds a certificate chain for |hostname| (in leaf-first order),
  // and calculates a signature of |server_config| using that chain.
  //
  // The signature uses SHA-256 as the hash function and PSS padding when the
  // key is RSA.
  //
  // The signature uses SHA-256 as the hash function when the key is ECDSA.
  // The signature may use an ECDSA key.
  //
  // The signature depends on |chlo_hash| which means that the signature can not
  // be cached.
  //
  // |hostname| may be empty to signify that a default certificate should be
  // used.
  //
  // This function may be called concurrently.
  //
  // Callers should expect that |callback| might be invoked synchronously.
  virtual void GetProof(const QuicSocketAddress& server_address,
                        const QuicString& hostname,
                        const QuicString& server_config,
                        QuicTransportVersion transport_version,
                        QuicStringPiece chlo_hash,
                        std::unique_ptr<Callback> callback) = 0;

  // Returns the certificate chain for |hostname| in leaf-first order.
  virtual QuicReferenceCountedPointer<Chain> GetCertChain(
      const QuicSocketAddress& server_address,
      const QuicString& hostname) = 0;

  // Computes a signature using the private key of the certificate for
  // |hostname|. The value in |in| is signed using the algorithm specified by
  // |signature_algorithm|, which is an |SSL_SIGN_*| value (as defined in TLS
  // 1.3). Implementations can only assume that |in| is valid during the call to
  // ComputeTlsSignature - an implementation computing signatures asynchronously
  // must copy it if the value to be signed is used outside of this function.
  //
  // Callers should expect that |callback| might be invoked synchronously.
  virtual void ComputeTlsSignature(
      const QuicSocketAddress& server_address,
      const QuicString& hostname,
      uint16_t signature_algorithm,
      QuicStringPiece in,
      std::unique_ptr<SignatureCallback> callback) = 0;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_PROOF_SOURCE_H_
