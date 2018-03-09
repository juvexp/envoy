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

#include <algorithm>
#include <map>

#include "base/logging.h"
#include "common/quic/platform/api/quic_str_cat.h"
#include "common/quic/platform/api/quic_string_utils.h"
#include "testing/base/public/googletest.h"
#include "gtest/gtest.h"
#include "util/random/mt_random.h"

namespace gfe_quic {
namespace test {
namespace {

class RandomUtilTest : public ::testing::Test {
 protected:
  RandomUtilTest() : random_(FLAGS_test_random_seed) {}

  void EvalSkewedLow(size_t max) {
    std::map<size_t, size_t> v2c;
    for (int j = 0; j <= max; ++j) {
      v2c.insert({j, 0});
    }
    const size_t kCount = 100000;
    size_t max_seen = max;
    for (int j = 0; j < kCount; ++j) {
      auto r = GenerateRandomSizeSkewedLow(max, &random_);
      EXPECT_LE(r, max_seen);
      max_seen = std::max(r, max_seen);
      v2c[r]++;
    }
    LOG(INFO) << "GenerateRandomSizeSkewedLow(" << max << ") produces:";
    for (int j = 0; j <= max_seen; ++j) {
      LOG(INFO) << QuicStringPrintf("%2d", j) << "    " << v2c[j] << " times";
    }
    LOG(INFO) << "";
  }

  MTRandom random_;
};

// Not really a test, just intended to allow one to understand the distribution
// of values that GenerateRandomSizeSkewedLow provides.
TEST_F(RandomUtilTest, GenerateRandomSizeSkewedLow) {
  for (int max = 1; max <= 20; ++max) {
    EvalSkewedLow(max);
  }
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
