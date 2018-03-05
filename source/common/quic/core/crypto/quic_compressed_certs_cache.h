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

#ifndef GFE_QUIC_CORE_CRYPTO_QUIC_COMPRESSED_CERTS_CACHE_H_
#define GFE_QUIC_CORE_CRYPTO_QUIC_COMPRESSED_CERTS_CACHE_H_

#include <vector>

#include "common/quic/core/crypto/proof_source.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_lru_cache.h"
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {

// QuicCompressedCertsCache is a cache to track most recently compressed certs.
class QUIC_EXPORT_PRIVATE QuicCompressedCertsCache {
 public:
  explicit QuicCompressedCertsCache(int64_t max_num_certs);
  ~QuicCompressedCertsCache();

  // Returns the pointer to the cached compressed cert if
  // |chain, client_common_set_hashes, client_cached_cert_hashes| hits cache.
  // Otherwise, return nullptr.
  // Returned pointer might become invalid on the next call to Insert().
  const QuicString* GetCompressedCert(
      const QuicReferenceCountedPointer<ProofSource::Chain>& chain,
      const QuicString& client_common_set_hashes,
      const QuicString& client_cached_cert_hashes);

  // Inserts the specified
  // |chain, client_common_set_hashes,
  //  client_cached_cert_hashes, compressed_cert| tuple to the cache.
  // If the insertion causes the cache to become overfull, entries will
  // be deleted in an LRU order to make room.
  void Insert(const QuicReferenceCountedPointer<ProofSource::Chain>& chain,
              const QuicString& client_common_set_hashes,
              const QuicString& client_cached_cert_hashes,
              const QuicString& compressed_cert);

  // Returns max number of cache entries the cache can carry.
  size_t MaxSize();

  // Returns current number of cache entries in the cache.
  size_t Size();

  // Default size of the QuicCompressedCertsCache per server side investigation.
  static const size_t kQuicCompressedCertsCacheSize = 225;

 private:
  // A wrapper of the tuple:
  //   |chain, client_common_set_hashes, client_cached_cert_hashes|
  // to identify uncompressed representation of certs.
  struct UncompressedCerts {
    UncompressedCerts();
    UncompressedCerts(
        const QuicReferenceCountedPointer<ProofSource::Chain>& chain,
        const QuicString* client_common_set_hashes,
        const QuicString* client_cached_cert_hashes);
    ~UncompressedCerts();

    const QuicReferenceCountedPointer<ProofSource::Chain> chain;
    const QuicString* client_common_set_hashes;
    const QuicString* client_cached_cert_hashes;
  };

  // Certs stored by QuicCompressedCertsCache where uncompressed certs data is
  // used to identify the uncompressed representation of certs and
  // |compressed_cert| is the cached compressed representation.
  class CachedCerts {
   public:
    CachedCerts();
    CachedCerts(const UncompressedCerts& uncompressed_certs,
                const QuicString& compressed_cert);
    CachedCerts(const CachedCerts& other);
    ~CachedCerts();

    // Returns true if the |uncompressed_certs| matches uncompressed
    // representation of this cert.
    bool MatchesUncompressedCerts(
        const UncompressedCerts& uncompressed_certs) const;

    const QuicString* compressed_cert() const;

   private:
    // Uncompressed certs data.
    QuicReferenceCountedPointer<ProofSource::Chain> chain_;
    const QuicString client_common_set_hashes_;
    const QuicString client_cached_cert_hashes_;

    // Cached compressed representation derived from uncompressed certs.
    const QuicString compressed_cert_;
  };

  // Computes a uint64_t hash for |uncompressed_certs|.
  uint64_t ComputeUncompressedCertsHash(
      const UncompressedCerts& uncompressed_certs);

  // Key is a unit64_t hash for UncompressedCerts. Stored associated value is
  // CachedCerts which has both original uncompressed certs data and the
  // compressed representation of the certs.
  QuicLRUCache<uint64_t, CachedCerts> certs_cache_;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_QUIC_COMPRESSED_CERTS_CACHE_H_
