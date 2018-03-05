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

#ifndef GFE_QUIC_CORE_CRYPTO_CERT_COMPRESSOR_H_
#define GFE_QUIC_CORE_CRYPTO_CERT_COMPRESSOR_H_

#include <vector>

#include "base/macros.h"
#include "common/quic/core/crypto/common_cert_set.h"
#include "common/quic/core/crypto/crypto_protocol.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {

// CertCompressor provides functions for compressing and decompressing
// certificate chains using three techniquies:
//   1) The peer may provide a list of a 64-bit, FNV-1a hashes of certificates
//      that they already have. In the event that one of them is to be
//      compressed, it can be replaced with just the hash.
//   2) The peer may provide a number of hashes that represent sets of
//      pre-shared certificates (CommonCertSets). If one of those certificates
//      is to be compressed, and it's known to the given CommonCertSets, then it
//      can be replaced with a set hash and certificate index.
//   3) Otherwise the certificates are compressed with zlib using a pre-shared
//      dictionary that consists of the certificates handled with the above
//      methods and a small chunk of common substrings.
class QUIC_EXPORT_PRIVATE CertCompressor {
 public:
  // CompressChain compresses the certificates in |certs| and returns a
  // compressed representation. |common_sets| contains the common certificate
  // sets known locally and |client_common_set_hashes| contains the hashes of
  // the common sets known to the peer. |client_cached_cert_hashes| contains
  // 64-bit, FNV-1a hashes of certificates that the peer already possesses.
  static QuicString CompressChain(const std::vector<QuicString>& certs,
                                  QuicStringPiece client_common_set_hashes,
                                  QuicStringPiece client_cached_cert_hashes,
                                  const CommonCertSets* common_sets);

  // DecompressChain decompresses the result of |CompressChain|, given in |in|,
  // into a series of certificates that are written to |out_certs|.
  // |cached_certs| contains certificates that the peer may have omitted and
  // |common_sets| contains the common certificate sets known locally.
  static bool DecompressChain(QuicStringPiece in,
                              const std::vector<QuicString>& cached_certs,
                              const CommonCertSets* common_sets,
                              std::vector<QuicString>* out_certs);

 private:
  DISALLOW_COPY_AND_ASSIGN(CertCompressor);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_CERT_COMPRESSOR_H_
