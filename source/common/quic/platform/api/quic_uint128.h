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

#ifndef GFE_QUIC_PLATFORM_API_QUIC_UINT128_H_
#define GFE_QUIC_PLATFORM_API_QUIC_UINT128_H_

#include "common/quic/platform/impl/quic_uint128_impl.h"

namespace gfe_quic {

using QuicUint128 = QuicUint128Impl;
#define MakeQuicUint128(hi, lo) MakeQuicUint128Impl(hi, lo);
#define QuicUint128Low64(x) QuicUint128Low64Impl(x);
#define QuicUint128High64(x) QuicUint128High64Impl(x);

}  // namespace gfe_quic

#endif  // GFE_QUIC_PLATFORM_API_QUIC_UINT128_H_
