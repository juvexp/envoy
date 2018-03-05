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

#include "common/quic/core/crypto/base/hkdf.h"

#include "common/quic/platform/api/quic_string.h"
#include "test/common/quic/platform/api/quic_test.h"
#include "strings/escaping.h"

using gfe_quic::crypto::HKDF;

namespace gfe_quic {
namespace test {
namespace {

struct HKDFInput {
  const char* key_hex;
  const char* salt_hex;
  const char* info_hex;
  const char* output_hex;
};

// These test cases are taken from
// https://tools.ietf.org/html/rfc5869#appendix-A.
static const HKDFInput kHKDFInputs[] = {
    {
        "0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b",
        "000102030405060708090a0b0c",
        "f0f1f2f3f4f5f6f7f8f9",
        "3cb25f25faacd57a90434f64d0362f2a2d2d0a90cf1a5a4c5db02d56ecc4c5bf340072"
        "08d5"
        "b887185865",
    },
    {
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122"
        "2324"
        "25262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f4041424344454647"
        "4849"
        "4a4b4c4d4e4f",
        "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182"
        "8384"
        "85868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9fa0a1a2a3a4a5a6a7"
        "a8a9"
        "aaabacadaeaf",
        "b0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2"
        "d3d4"
        "d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7"
        "f8f9"
        "fafbfcfdfeff",
        "b11e398dc80327a1c8e7f78c596a49344f012eda2d4efad8a050cc4c19afa97c59045a"
        "99ca"
        "c7827271cb41c65e590e09da3275600c2f09b8367793a9aca3db71cc30c58179ec3e87"
        "c14c"
        "01d5c1f3434f1d87",
    },
    {
        "0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b",
        "",
        "",
        "8da4e775a563c18f715f802a063c5a31b8a11f5c5ee1879ec3454e5f3c738d2d9d2013"
        "95fa"
        "a4b61a96c8",
    },
};

class HKDFTest : public QuicTest {};

TEST_F(HKDFTest, HKDF) {
  for (size_t i = 0; i < arraysize(kHKDFInputs); i++) {
    const HKDFInput& test(kHKDFInputs[i]);
    SCOPED_TRACE(i);

    const QuicString key = strings::a2b_hex(test.key_hex);
    const QuicString salt = strings::a2b_hex(test.salt_hex);
    const QuicString info = strings::a2b_hex(test.info_hex);
    const QuicString expected = strings::a2b_hex(test.output_hex);

    // We set the key_length to the length of the expected output and then take
    // the result from the first key, which is the client write key.
    HKDF hkdf(key, salt, info, expected.size(), 0, 0);

    ASSERT_EQ(expected.size(), hkdf.client_write_key().size());
    EXPECT_EQ(0, memcmp(expected.data(), hkdf.client_write_key().data(),
                        expected.size()));
  }
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
