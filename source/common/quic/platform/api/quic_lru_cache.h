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

#ifndef GFE_QUIC_PLATFORM_API_QUIC_LRU_CACHE_H_
#define GFE_QUIC_PLATFORM_API_QUIC_LRU_CACHE_H_

#include <memory>

#include "common/quic/platform/impl/quic_lru_cache_impl.h"

namespace gfe_quic {

// A LRU cache that maps from type Key to Value* in QUIC.
// This cache CANNOT be shared by multiple threads (even with locks) because
// Value* returned by Lookup() can be invalid if the entry is evicted by other
// threads.
template <class K, class V>
class QuicLRUCache {
 public:
  explicit QuicLRUCache(int64_t total_units) : impl_(total_units) {}

  // Inserts one unit of |key|, |value| pair to the cache. Cache takes ownership
  // of inserted |value|.
  void Insert(const K& key, std::unique_ptr<V> value) {
    impl_.Insert(key, std::move(value));
  }

  // If cache contains an entry for |key|, return a pointer to it. This returned
  // value is guaranteed to be valid until Insert or Clear.
  // Else return nullptr.
  V* Lookup(const K& key) { return impl_.Lookup(key); }

  // Removes all entries from the cache. This method MUST be called before
  // destruction.
  void Clear() { impl_.Clear(); }

  // Returns maximum size of the cache.
  int64_t MaxSize() const { return impl_.MaxSize(); }

  // Returns current size of the cache.
  int64_t Size() const { return impl_.Size(); }

 private:
  QuicLRUCacheImpl<K, V> impl_;

  DISALLOW_COPY_AND_ASSIGN(QuicLRUCache);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_PLATFORM_API_QUIC_LRU_CACHE_H_
