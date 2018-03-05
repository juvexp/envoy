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

#ifndef GFE_QUIC_PLATFORM_API_QUIC_STRING_UTILS_H_
#define GFE_QUIC_PLATFORM_API_QUIC_STRING_UTILS_H_

#include <utility>

#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"
#include "common/quic/platform/impl/quic_string_utils_impl.h"

namespace gfe_quic {

template <typename... Args>
inline void QuicStrAppend(QuicString* output, const Args&... args) {
  QuicStrAppendImpl(output, std::forward<const Args&>(args)...);
}

}  // namespace gfe_quic

#endif  // GFE_QUIC_PLATFORM_API_QUIC_STRING_UTILS_H_
