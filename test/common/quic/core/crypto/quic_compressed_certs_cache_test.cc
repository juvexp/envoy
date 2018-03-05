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

#include "common/quic/core/crypto/quic_compressed_certs_cache.h"

#include "base/macros.h"
#include "common/quic/core/crypto/cert_compressor.h"
#include "common/quic/platform/api/quic_string.h"
#include "test/common/quic/platform/api/quic_test.h"
#include "common/quic/platform/api/quic_text_utils.h"
#include "test/common/quic/test_tools/crypto_test_utils.h"

namespace gfe_quic {

namespace test {

namespace {

class QuicCompressedCertsCacheTest : public testing::Test {
 public:
  QuicCompressedCertsCacheTest()
      : certs_cache_(QuicCompressedCertsCache::kQuicCompressedCertsCacheSize) {}

 protected:
  QuicCompressedCertsCache certs_cache_;
};

TEST_F(QuicCompressedCertsCacheTest, CacheHit) {
  std::vector<QuicString> certs = {"leaf cert", "intermediate cert",
                                   "root cert"};
  QuicReferenceCountedPointer<ProofSource::Chain> chain(
      new ProofSource::Chain(certs));
  QuicString common_certs = "common certs";
  QuicString cached_certs = "cached certs";
  QuicString compressed = "compressed cert";

  certs_cache_.Insert(chain, common_certs, cached_certs, compressed);

  const QuicString* cached_value =
      certs_cache_.GetCompressedCert(chain, common_certs, cached_certs);
  ASSERT_NE(nullptr, cached_value);
  EXPECT_EQ(*cached_value, compressed);
}

TEST_F(QuicCompressedCertsCacheTest, CacheMiss) {
  std::vector<QuicString> certs = {"leaf cert", "intermediate cert",
                                   "root cert"};
  QuicReferenceCountedPointer<ProofSource::Chain> chain(
      new ProofSource::Chain(certs));

  QuicString common_certs = "common certs";
  QuicString cached_certs = "cached certs";
  QuicString compressed = "compressed cert";

  certs_cache_.Insert(chain, common_certs, cached_certs, compressed);

  EXPECT_EQ(nullptr, certs_cache_.GetCompressedCert(
                         chain, "mismatched common certs", cached_certs));
  EXPECT_EQ(nullptr, certs_cache_.GetCompressedCert(chain, common_certs,
                                                    "mismatched cached certs"));

  // A different chain though with equivalent certs should get a cache miss.
  QuicReferenceCountedPointer<ProofSource::Chain> chain2(
      new ProofSource::Chain(certs));
  EXPECT_EQ(nullptr,
            certs_cache_.GetCompressedCert(chain2, common_certs, cached_certs));
}

TEST_F(QuicCompressedCertsCacheTest, CacheMissDueToEviction) {
  // Test cache returns a miss when a queried uncompressed certs was cached but
  // then evicted.
  std::vector<QuicString> certs = {"leaf cert", "intermediate cert",
                                   "root cert"};
  QuicReferenceCountedPointer<ProofSource::Chain> chain(
      new ProofSource::Chain(certs));

  QuicString common_certs = "common certs";
  QuicString cached_certs = "cached certs";
  QuicString compressed = "compressed cert";
  certs_cache_.Insert(chain, common_certs, cached_certs, compressed);

  // Insert another kQuicCompressedCertsCacheSize certs to evict the first
  // cached cert.
  for (unsigned int i = 0;
       i < QuicCompressedCertsCache::kQuicCompressedCertsCacheSize; i++) {
    EXPECT_EQ(certs_cache_.Size(), i + 1);
    certs_cache_.Insert(chain, QuicTextUtils::Uint64ToString(i), "",
                        QuicTextUtils::Uint64ToString(i));
  }
  EXPECT_EQ(certs_cache_.MaxSize(), certs_cache_.Size());

  EXPECT_EQ(nullptr,
            certs_cache_.GetCompressedCert(chain, common_certs, cached_certs));
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
