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

#include "common/quic/platform/api/quic_lru_cache.h"

#include "test/common/quic/platform/api/quic_test.h"

namespace gfe_quic {
namespace test {
namespace {

struct CachedItem {
  explicit CachedItem(uint32_t new_value) : value(new_value) {}

  uint32_t value;
};

class QuicLRUCacheTest : public QuicTest {};

TEST_F(QuicLRUCacheTest, InsertAndLookup) {
  QuicLRUCache<int, CachedItem> cache(5);
  EXPECT_EQ(nullptr, cache.Lookup(1));
  EXPECT_EQ(0u, cache.Size());
  EXPECT_EQ(5u, cache.MaxSize());

  // Check that item 1 was properly inserted.
  std::unique_ptr<CachedItem> item1(new CachedItem(11));
  cache.Insert(1, std::move(item1));
  EXPECT_EQ(1u, cache.Size());
  EXPECT_EQ(11u, cache.Lookup(1)->value);

  // Check that item 2 overrides item 1.
  std::unique_ptr<CachedItem> item2(new CachedItem(12));
  cache.Insert(1, std::move(item2));
  EXPECT_EQ(1u, cache.Size());
  EXPECT_EQ(12u, cache.Lookup(1)->value);

  std::unique_ptr<CachedItem> item3(new CachedItem(13));
  cache.Insert(3, std::move(item3));
  EXPECT_EQ(2u, cache.Size());
  EXPECT_EQ(13u, cache.Lookup(3)->value);

  // No memory leakage.
  cache.Clear();
  EXPECT_EQ(0u, cache.Size());
}

TEST_F(QuicLRUCacheTest, Eviction) {
  QuicLRUCache<int, CachedItem> cache(3);

  for (size_t i = 1; i <= 4; ++i) {
    std::unique_ptr<CachedItem> item(new CachedItem(10 + i));
    cache.Insert(i, std::move(item));
  }

  EXPECT_EQ(3u, cache.Size());
  EXPECT_EQ(3u, cache.MaxSize());

  // Make sure item 1 is evicted.
  EXPECT_EQ(nullptr, cache.Lookup(1));
  EXPECT_EQ(14u, cache.Lookup(4)->value);

  EXPECT_EQ(12u, cache.Lookup(2)->value);
  std::unique_ptr<CachedItem> item5(new CachedItem(15));
  cache.Insert(5, std::move(item5));
  // Make sure item 3 is evicted.
  EXPECT_EQ(nullptr, cache.Lookup(3));
  EXPECT_EQ(15u, cache.Lookup(5)->value);

  // No memory leakage.
  cache.Clear();
  EXPECT_EQ(0u, cache.Size());
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
