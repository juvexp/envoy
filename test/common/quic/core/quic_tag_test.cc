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

#include "common/quic/core/quic_tag.h"

#include "common/quic/core/crypto/crypto_protocol.h"
#include "test/common/quic/platform/api/quic_test.h"

namespace gfe_quic {
namespace test {
namespace {

class QuicTagTest : public QuicTest {};

TEST_F(QuicTagTest, TagToString) {
  EXPECT_EQ("SCFG", QuicTagToString(kSCFG));
  EXPECT_EQ("SNO ", QuicTagToString(kServerNonceTag));
  EXPECT_EQ("CRT ", QuicTagToString(kCertificateTag));
  EXPECT_EQ("CHLO", QuicTagToString(MakeQuicTag('C', 'H', 'L', 'O')));
  // A tag that contains a non-printing character will be printed as a decimal
  // number.
  EXPECT_EQ("525092931", QuicTagToString(MakeQuicTag('C', 'H', 'L', '\x1f')));
}

TEST_F(QuicTagTest, MakeQuicTag) {
  QuicTag tag = MakeQuicTag('A', 'B', 'C', 'D');
  char bytes[4];
  memcpy(bytes, &tag, 4);
  EXPECT_EQ('A', bytes[0]);
  EXPECT_EQ('B', bytes[1]);
  EXPECT_EQ('C', bytes[2]);
  EXPECT_EQ('D', bytes[3]);
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
