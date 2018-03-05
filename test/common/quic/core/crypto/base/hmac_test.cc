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

#include "common/quic/core/crypto/base/hmac.h"

#include <cstdint>

#include "common/quic/platform/api/quic_string.h"
#include "test/common/quic/platform/api/quic_test.h"
#include "strings/escaping.h"

using gfe_quic::crypto::HMAC;

namespace gfe_quic {
namespace test {
namespace {

struct HMACInput {
  const char* data;
  const char* key_hex;
  uint8_t digest[32];
};

static const HMACInput kHMACInputs[] = {
    {
        "Test Using Larger Than Block-Size Key - Hash Key First",
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
        {
            0x60, 0xe4, 0x31, 0x59, 0x1e, 0xe0, 0xb6, 0x7f, 0x0d, 0x8a, 0x26,
            0xaa, 0xcb, 0xf5, 0xb7, 0x7f, 0x8e, 0x0b, 0xc6, 0x21, 0x37, 0x28,
            0xc5, 0x14, 0x05, 0x46, 0x04, 0x0f, 0x0e, 0xe3, 0x7f, 0x54,
        },
    },
    {
        "The test message for the MD2, MD5, and SHA-1 hashing algorithms.",
        "46697265666f7820616e64205468756e64657242697264206172652061776573"
        "6f6d652100",
        {
            0x05, 0x75, 0x9a, 0x9e, 0x70, 0x5e, 0xe7, 0x44, 0xe2, 0x46, 0x4b,
            0x92, 0x22, 0x14, 0x22, 0xe0, 0x1b, 0x92, 0x8a, 0x0c, 0xfe, 0xf5,
            0x49, 0xe9, 0xa7, 0x1b, 0x56, 0x7d, 0x1d, 0x29, 0x40, 0x48,
        },
    },
};

class HMACTest : public QuicTest {};

TEST_F(HMACTest, SHA256) {
  for (size_t i = 0; i < arraysize(kHMACInputs); i++) {
    const HMACInput& test(kHMACInputs[i]);
    SCOPED_TRACE(i);

    HMAC hmac(HMAC::SHA256);
    const QuicString key = strings::a2b_hex(test.key_hex);
    ASSERT_TRUE(hmac.Init(key));

    uint8_t digest_copy[sizeof(test.digest)];
    memcpy(digest_copy, test.digest, sizeof(test.digest));
    QuicStringPiece digest_sp(reinterpret_cast<char*>(digest_copy),
                              sizeof(digest_copy));
    ASSERT_TRUE(hmac.Verify(test.data, digest_sp));

    digest_copy[16] ^= 0x80;
    ASSERT_FALSE(hmac.Verify(test.data, digest_sp));
  }
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
