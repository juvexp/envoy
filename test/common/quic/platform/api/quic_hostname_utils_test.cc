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

#include "common/quic/platform/api/quic_hostname_utils.h"

#include "common/quic/platform/api/quic_arraysize.h"
#include "common/quic/platform/api/quic_string.h"
#include "test/common/quic/platform/api/quic_test.h"

namespace gfe_quic {
namespace test {
namespace {

class QuicHostnameUtilsTest : public QuicTest {};

TEST_F(QuicHostnameUtilsTest, IsValidSNI) {
  // IP as SNI.
  EXPECT_FALSE(QuicHostnameUtils::IsValidSNI("192.168.0.1"));
  // SNI without any dot.
  EXPECT_FALSE(QuicHostnameUtils::IsValidSNI("somedomain"));
  // Invalid by RFC2396 but unfortunately domains of this form exist.
  EXPECT_TRUE(QuicHostnameUtils::IsValidSNI("some_domain.com"));
  // An empty string must be invalid otherwise the QUIC client will try sending
  // it.
  EXPECT_FALSE(QuicHostnameUtils::IsValidSNI(""));

  // Valid SNI
  EXPECT_TRUE(QuicHostnameUtils::IsValidSNI("test.google.com"));
}

TEST_F(QuicHostnameUtilsTest, NormalizeHostname) {
  struct {
    const char *input, *expected;
  } tests[] = {
      {
          "www.google.com",
          "www.google.com",
      },
      {
          "WWW.GOOGLE.COM",
          "www.google.com",
      },
      {
          "www.google.com.",
          "www.google.com",
      },
      {
          "www.google.COM.",
          "www.google.com",
      },
      {
          "www.google.com..",
          "www.google.com",
      },
      {
          "www.google.com........",
          "www.google.com",
      },
  };

  for (size_t i = 0; i < QUIC_ARRAYSIZE(tests); ++i) {
    char buf[256];
    snprintf(buf, sizeof(buf), "%s", tests[i].input);
    EXPECT_EQ(QuicString(tests[i].expected),
              QuicHostnameUtils::NormalizeHostname(buf));
  }
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
