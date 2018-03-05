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

#include "test/common/quic/test_tools/quic_test_utils.h"

#include "test/common/quic/platform/api/quic_test.h"
#include "testing/base/public/gunit-spi.h"

namespace gfe_quic {
namespace test {

class QuicTestUtilsTest : public QuicTest {};

TEST_F(QuicTestUtilsTest, BasicApproxEq) {
  ExpectApproxEq(10, 10, 1e-6f);
  ExpectApproxEq(1000, 1001, 0.01f);
  EXPECT_NONFATAL_FAILURE(ExpectApproxEq(1000, 1100, 0.01f), "");

  ExpectApproxEq(64, 31, 0.55f);
  EXPECT_NONFATAL_FAILURE(ExpectApproxEq(31, 64, 0.55f), "");
}

TEST_F(QuicTestUtilsTest, QuicTimeDelta) {
  ExpectApproxEq(QuicTime::Delta::FromMicroseconds(1000),
                 QuicTime::Delta::FromMicroseconds(1003), 0.01f);
  EXPECT_NONFATAL_FAILURE(
      ExpectApproxEq(QuicTime::Delta::FromMicroseconds(1000),
                     QuicTime::Delta::FromMicroseconds(1200), 0.01f),
      "");
}

TEST_F(QuicTestUtilsTest, QuicBandwidth) {
  ExpectApproxEq(QuicBandwidth::FromBytesPerSecond(1000),
                 QuicBandwidth::FromBitsPerSecond(8005), 0.01f);
  EXPECT_NONFATAL_FAILURE(
      ExpectApproxEq(QuicBandwidth::FromBytesPerSecond(1000),
                     QuicBandwidth::FromBitsPerSecond(9005), 0.01f),
      "");
}

// Ensure that SimpleRandom does not change its output for a fixed seed.
TEST_F(QuicTestUtilsTest, SimpleRandomStability) {
  SimpleRandom rng;
  rng.set_seed(UINT64_C(0x1234567800010001));
  EXPECT_EQ(UINT64_C(14865409841904857791), rng.RandUint64());
  EXPECT_EQ(UINT64_C(12139094019410129741), rng.RandUint64());
}

}  // namespace test
}  // namespace gfe_quic
