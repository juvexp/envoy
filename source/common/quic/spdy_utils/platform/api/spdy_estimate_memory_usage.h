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

#ifndef GFE_SPDY_PLATFORM_API_SPDY_ESTIMATE_MEMORY_USAGE_H_
#define GFE_SPDY_PLATFORM_API_SPDY_ESTIMATE_MEMORY_USAGE_H_

#include <cstddef>

#include "common/quic/spdy_utils/platform/impl/spdy_estimate_memory_usage_impl.h"

namespace gfe_spdy {

template <class T>
size_t SpdyEstimateMemoryUsage(const T& object) {
  return SpdyEstimateMemoryUsageImpl(object);
}

}  // namespace gfe_spdy

#endif  // GFE_SPDY_PLATFORM_API_SPDY_ESTIMATE_MEMORY_USAGE_H_
