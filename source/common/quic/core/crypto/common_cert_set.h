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

#ifndef GFE_QUIC_CORE_CRYPTO_COMMON_CERT_SET_H_
#define GFE_QUIC_CORE_CRYPTO_COMMON_CERT_SET_H_

#include <cstdint>

#include "common/quic/core/crypto/crypto_protocol.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {

// CommonCertSets is an interface to an object that contains a number of common
// certificate sets and can match against them.
class QUIC_EXPORT_PRIVATE CommonCertSets {
 public:
  virtual ~CommonCertSets();

  // GetInstanceQUIC returns the standard QUIC common certificate sets.
  static const CommonCertSets* GetInstanceQUIC();

  // GetCommonHashes returns a QuicStringPiece containing the hashes of common
  // sets supported by this object. The 64-bit hashes are concatenated in the
  // QuicStringPiece.
  virtual QuicStringPiece GetCommonHashes() const = 0;

  // GetCert returns a specific certificate (at index |index|) in the common
  // set identified by |hash|. If no such certificate is known, an empty
  // QuicStringPiece is returned.
  virtual QuicStringPiece GetCert(uint64_t hash, uint32_t index) const = 0;

  // MatchCert tries to find |cert| in one of the common certificate sets
  // identified by |common_set_hashes|. On success it puts the hash of the
  // set in |out_hash|, the index of |cert| in the set in |out_index| and
  // returns true. Otherwise it returns false.
  virtual bool MatchCert(QuicStringPiece cert,
                         QuicStringPiece common_set_hashes,
                         uint64_t* out_hash,
                         uint32_t* out_index) const = 0;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_COMMON_CERT_SET_H_
