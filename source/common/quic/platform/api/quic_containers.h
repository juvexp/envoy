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

#ifndef GFE_QUIC_PLATFORM_API_QUIC_CONTAINERS_H_
#define GFE_QUIC_PLATFORM_API_QUIC_CONTAINERS_H_

#include "common/quic/platform/impl/quic_containers_impl.h"

namespace gfe_quic {

// A general-purpose unordered map.
template <typename Key,
          typename Value,
          typename Hash = typename QuicUnorderedMapImpl<Key, Value>::hasher,
          typename Eq = typename QuicUnorderedMapImpl<Key, Value>::key_equal,
          typename Alloc =
              typename QuicUnorderedMapImpl<Key, Value>::allocator_type>
using QuicUnorderedMap = QuicUnorderedMapImpl<Key, Value, Hash, Eq, Alloc>;

// A general-purpose unordered set.
template <typename Key,
          typename Hash = typename QuicUnorderedSetImpl<Key>::hasher,
          typename Eq = typename QuicUnorderedSetImpl<Key>::key_equal,
          typename Alloc = typename QuicUnorderedSetImpl<Key>::allocator_type>
using QuicUnorderedSet = QuicUnorderedSetImpl<Key, Hash, Eq, Alloc>;

// A map which offers insertion-ordered iteration.
template <typename Key, typename Value>
using QuicLinkedHashMap = QuicLinkedHashMapImpl<Key, Value>;

// Used for maps that are typically small, then it is faster than (for example)
// hash_map which is optimized for large data sets. QuicSmallMap upgrades itself
// automatically to a QuicSmallMapImpl-specified map when it runs out of space.
//
// DOES NOT GUARANTEE POINTER OR ITERATOR STABILITY!
template <typename Key, typename Value, int Size>
using QuicSmallMap = QuicSmallMapImpl<Key, Value, Size>;

// A data structure used to represent a sorted set of non-empty, non-adjacent,
// and mutually disjoint intervals.
template <typename T>
using QuicIntervalSet = QuicIntervalSetImpl<T>;

// Represents a simple queue which may be backed by a list or
// a flat circular buffer.
//
// DOES NOT GUARANTEE POINTER OR ITERATOR STABILITY!
template <typename T>
using QuicQueue = QuicQueueImpl<T>;

// Represents a double-ended queue which may be backed by a list or
// a flat circular buffer.
//
// DOES NOT GUARANTEE POINTER OR ITERATOR STABILITY!
template <typename T>
using QuicDeque = QuicDequeImpl<T>;

}  // namespace gfe_quic

#endif  // GFE_QUIC_PLATFORM_API_QUIC_CONTAINERS_H_
