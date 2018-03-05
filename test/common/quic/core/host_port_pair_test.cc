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

#include "common/quic/core/host_port_pair.h"

#include "common/quic/platform/api/quic_arraysize.h"
#include "common/quic/platform/api/quic_str_cat.h"
#include "common/quic/platform/api/quic_string.h"
#include "test/common/quic/platform/api/quic_test.h"
#include "test/common/quic/test_tools/quic_test_utils.h"
#include "testing/base/public/test_utils.h"

namespace gfe_quic {

namespace {

struct TestData {
  QuicString host;
  uint16_t port;
  QuicString to_string;
  QuicString host_for_url;
} tests[] = {
    {"www.google.com", 80, "www.google.com:80", "www.google.com"},
    {"www.google.com", 443, "www.google.com:443", "www.google.com"},
    {"127.0.0.1", 80, "127.0.0.1:80", "127.0.0.1"},
    {"192.168.1.1", 80, "192.168.1.1:80", "192.168.1.1"},
    {"::1", 80, "[::1]:80", "[::1]"},
    {"2001:db8::42", 80, "[2001:db8::42]:80", "[2001:db8::42]"},
};

class HostPortPairTest : public QuicTest {};

TEST_F(HostPortPairTest, Parsing) {
  for (int index = 0; index < QUIC_ARRAYSIZE(tests); ++index) {
    HostPortPair foo(tests[index].host, tests[index].port);
    EXPECT_EQ(tests[index].to_string, foo.ToString());
  }

  // Test empty hostname.
  HostPortPair foo(QuicString(), 10);
}

TEST_F(HostPortPairTest, HostForURL) {
  for (int index = 0; index < QUIC_ARRAYSIZE(tests); ++index) {
    HostPortPair foo(tests[index].host, tests[index].port);
    EXPECT_EQ(tests[index].host_for_url, foo.HostForURL());
  }

  // Test hostname with null character.
  QuicString bar_hostname("a\0.com", 6);
  HostPortPair bar(bar_hostname, 80);
  QuicString expected_error =
      QuicStrCat("Host has a null char: ", bar_hostname);
  EXPECT_QUIC_BUG(bar.HostForURL(), expected_error);
}

TEST_F(HostPortPairTest, LessThan) {
  HostPortPair a_10("a.com", 10);
  HostPortPair a_11("a.com", 11);
  HostPortPair b_10("b.com", 10);
  HostPortPair b_11("b.com", 11);

  EXPECT_FALSE(a_10 < a_10);
  EXPECT_TRUE(a_10 < a_11);
  EXPECT_TRUE(a_10 < b_10);
  EXPECT_TRUE(a_10 < b_11);

  EXPECT_FALSE(a_11 < a_10);
  EXPECT_FALSE(a_11 < b_10);

  EXPECT_FALSE(b_10 < a_10);
  EXPECT_TRUE(b_10 < a_11);

  EXPECT_FALSE(b_11 < a_10);
}

TEST_F(HostPortPairTest, Equals) {
  HostPortPair a_10("a.com", 10);
  HostPortPair a_11("a.com", 11);
  HostPortPair b_10("b.com", 10);
  HostPortPair b_11("b.com", 11);

  HostPortPair new_a_10("a.com", 10);

  EXPECT_TRUE(new_a_10.Equals(a_10));
  EXPECT_FALSE(new_a_10.Equals(a_11));
  EXPECT_FALSE(new_a_10.Equals(b_10));
  EXPECT_FALSE(new_a_10.Equals(b_11));
}

}  // namespace

}  // namespace gfe_quic
