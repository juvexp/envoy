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

#include "common/quic/core/crypto/quic_random.h"

#include "test/common/quic/platform/api/quic_test.h"

namespace gfe_quic {
namespace test {

class QuicRandomTest : public QuicTest {};

TEST_F(QuicRandomTest, RandBytes) {
  unsigned char buf1[16];
  unsigned char buf2[16];
  memset(buf1, 0xaf, sizeof(buf1));
  memset(buf2, 0xaf, sizeof(buf2));
  ASSERT_EQ(0, memcmp(buf1, buf2, sizeof(buf1)));

  QuicRandom* rng = QuicRandom::GetInstance();
  rng->RandBytes(buf1, sizeof(buf1));
  EXPECT_NE(0, memcmp(buf1, buf2, sizeof(buf1)));
}

TEST_F(QuicRandomTest, RandUint64) {
  QuicRandom* rng = QuicRandom::GetInstance();
  uint64_t value1 = rng->RandUint64();
  uint64_t value2 = rng->RandUint64();
  EXPECT_NE(value1, value2);
}

TEST_F(QuicRandomTest, Reseed) {
  char buf[1024];
  memset(buf, 0xaf, sizeof(buf));

  QuicRandom* rng = QuicRandom::GetInstance();
  rng->Reseed(buf, sizeof(buf));
}

}  // namespace test
}  // namespace gfe_quic
