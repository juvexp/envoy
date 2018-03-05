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

#include "test/common/quic/http/tools/quic_http_random_util.h"

#include <cmath>

#include "common/quic/platform/api/quic_string_piece.h"
#include "util/random/random_gaussian.h"
#include "util/random/util.h"

using ::util::random::RandomString;

namespace gfe_quic {
namespace test {

size_t GenerateUniformInRange(size_t lo, size_t hi, QuicTestRandomBase* rng) {
  if (lo + 1 >= hi) {
    return lo;
  }
  return lo + rng->Rand64() % (hi - lo);
}

// Here "word" means something that starts with a lower-case letter, and has
// zero or more additional characters that are numbers or lower-case letters.
QuicString GenerateQuicHttpHeaderName(size_t len, QuicTestRandomBase* rng) {
  QuicStringPiece alpha_lc = "abcdefghijklmnopqrstuvwxyz";
  // If the name is short, just make it one word.
  if (len < 8) {
    return RandomString(rng, len, alpha_lc);
  }
  // If the name is longer, ensure it starts with a word, and after that may
  // have any character in alphanumdash_lc. 4 is arbitrary, could be as low
  // as 1.
  QuicStringPiece alphanumdash_lc = "abcdefghijklmnopqrstuvwxyz0123456789-";
  return RandomString(rng, 4, alpha_lc) +
         RandomString(rng, len - 4, alphanumdash_lc);
}

QuicString GenerateWebSafeString(size_t len, QuicTestRandomBase* rng) {
  return RandomString(rng, len, ::util::random::kWebsafe64);
}

QuicString GenerateWebSafeString(size_t lo,
                                 size_t hi,
                                 QuicTestRandomBase* rng) {
  return GenerateWebSafeString(GenerateUniformInRange(lo, hi, rng), rng);
}

size_t GenerateRandomSizeSkewedLow(size_t max, QuicTestRandomBase* rng) {
  if (max == 0) {
    return 0;
  }
  // Generate a random number with a Gaussian distribution, centered on zero;
  // take the absolute, and then keep in range 0 to max.
  for (int i = 0; i < 10; i++) {
    auto r = static_cast<size_t>(std::abs(ZGaussian::RndGaussian(rng)) * max);
    if (r <= max) {
      return r;
    }
  }
  return rng->Uniform(max + 1);
}

}  // namespace test
}  // namespace gfe_quic
