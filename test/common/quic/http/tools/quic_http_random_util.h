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

#ifndef GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_RANDOM_UTIL_H_
#define GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_RANDOM_UTIL_H_

#include <stddef.h>

#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_test_random.h"

namespace gfe_quic {
namespace test {

// Returns a random integer in the range [lo, hi).
size_t GenerateUniformInRange(size_t lo, size_t hi, QuicTestRandomBase* rng);

// Generate a string with the web-safe string character set of specified len.
QuicString GenerateWebSafeString(size_t len, QuicTestRandomBase* rng);

// Generate a string with the web-safe string character set of length [lo, hi).
QuicString GenerateWebSafeString(size_t lo, size_t hi, QuicTestRandomBase* rng);

// Returns a random integer in the range [0, max], with a bias towards producing
// lower numbers.
size_t GenerateRandomSizeSkewedLow(size_t max, QuicTestRandomBase* rng);

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_RANDOM_UTIL_H_
