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

#ifndef GFE_QUIC_PLATFORM_API_QUIC_PREFETCH_H_
#define GFE_QUIC_PLATFORM_API_QUIC_PREFETCH_H_

#include "common/quic/platform/impl/quic_prefetch_impl.h"

namespace gfe_quic {

// Move data into the cache before it is read, or "prefetch" it.
//
// The value of `addr` is the address of the memory to prefetch. If
// the target and compiler support it, data prefetch instructions are
// generated. If the prefetch is done some time before the memory is
// read, it may be in the cache by the time the read occurs.
//
// The function names specify the temporal locality heuristic applied,
// using the names of Intel prefetch instructions:
//
//   T0 - high degree of temporal locality; data should be left in as
//        many levels of the cache possible
//   T1 - moderate degree of temporal locality
//   T2 - low degree of temporal locality
//   Nta - no temporal locality, data need not be left in the cache
//         after the read
//
// Incorrect or gratuitous use of these functions can degrade
// performance, so use them only when representative benchmarks show
// an improvement.

inline void QuicPrefetchT0(const void* addr) {
  return QuicPrefetchT0Impl(addr);
}

}  // namespace gfe_quic

#endif  // GFE_QUIC_PLATFORM_API_QUIC_PREFETCH_H_
