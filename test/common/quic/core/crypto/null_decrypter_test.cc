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

#include "common/quic/core/crypto/null_decrypter.h"
#include "common/quic/platform/api/quic_arraysize.h"
#include "test/common/quic/platform/api/quic_test.h"
#include "test/common/quic/test_tools/quic_test_utils.h"

namespace gfe_quic {
namespace test {

class NullDecrypterTest : public QuicTestWithParam<bool> {};

TEST_F(NullDecrypterTest, DecryptClient) {
  unsigned char expected[] = {
      // fnv hash
      0x97, 0xdc, 0x27, 0x2f, 0x18, 0xa8, 0x56, 0x73, 0xdf, 0x8d, 0x1d, 0xd0,
      // payload
      'g', 'o', 'o', 'd', 'b', 'y', 'e', '!',
  };
  const char* data = reinterpret_cast<const char*>(expected);
  size_t len = QUIC_ARRAYSIZE(expected);
  NullDecrypter decrypter(Perspective::IS_SERVER);
  char buffer[256];
  size_t length = 0;
  ASSERT_TRUE(decrypter.DecryptPacket(QUIC_VERSION_37, 0, "hello world!",
                                      QuicStringPiece(data, len), buffer,
                                      &length, 256));
  EXPECT_LT(0u, length);
  EXPECT_EQ("goodbye!", QuicStringPiece(buffer, length));
}

TEST_F(NullDecrypterTest, DecryptServer) {
  unsigned char expected[] = {
      // fnv hash
      0x63, 0x5e, 0x08, 0x03, 0x32, 0x80, 0x8f, 0x73, 0xdf, 0x8d, 0x1d, 0x1a,
      // payload
      'g', 'o', 'o', 'd', 'b', 'y', 'e', '!',
  };
  const char* data = reinterpret_cast<const char*>(expected);
  size_t len = QUIC_ARRAYSIZE(expected);
  NullDecrypter decrypter(Perspective::IS_CLIENT);
  char buffer[256];
  size_t length = 0;
  ASSERT_TRUE(decrypter.DecryptPacket(QUIC_VERSION_37, 0, "hello world!",
                                      QuicStringPiece(data, len), buffer,
                                      &length, 256));
  EXPECT_LT(0u, length);
  EXPECT_EQ("goodbye!", QuicStringPiece(buffer, length));
}

TEST_F(NullDecrypterTest, DecryptClientPre37) {
  unsigned char expected[] = {
      // fnv hash
      0xa0, 0x6f, 0x44, 0x8a, 0x44, 0xf8, 0x18, 0x3b, 0x47, 0x91, 0xb2, 0x13,
      // payload
      'g', 'o', 'o', 'd', 'b', 'y', 'e', '!',
  };
  const char* data = reinterpret_cast<const char*>(expected);
  size_t len = QUIC_ARRAYSIZE(expected);
  NullDecrypter decrypter(Perspective::IS_CLIENT);
  char buffer[256];
  size_t length = 0;
  ASSERT_TRUE(decrypter.DecryptPacket(QUIC_VERSION_35, 0, "hello world!",
                                      QuicStringPiece(data, len), buffer,
                                      &length, 256));
  EXPECT_LT(0u, length);
  EXPECT_EQ("goodbye!", QuicStringPiece(buffer, length));
}

TEST_F(NullDecrypterTest, DecryptServerPre37) {
  unsigned char expected[] = {
      // fnv hash
      0xa0, 0x6f, 0x44, 0x8a, 0x44, 0xf8, 0x18, 0x3b, 0x47, 0x91, 0xb2, 0x13,
      // payload
      'g', 'o', 'o', 'd', 'b', 'y', 'e', '!',
  };
  const char* data = reinterpret_cast<const char*>(expected);
  size_t len = QUIC_ARRAYSIZE(expected);
  NullDecrypter decrypter(Perspective::IS_SERVER);
  char buffer[256];
  size_t length = 0;
  ASSERT_TRUE(decrypter.DecryptPacket(QUIC_VERSION_35, 0, "hello world!",
                                      QuicStringPiece(data, len), buffer,
                                      &length, 256));
  EXPECT_LT(0u, length);
  EXPECT_EQ("goodbye!", QuicStringPiece(buffer, length));
}

TEST_F(NullDecrypterTest, BadHash) {
  unsigned char expected[] = {
      // fnv hash
      0x46, 0x11, 0xea, 0x5f, 0xcf, 0x1d, 0x66, 0x5b, 0xba, 0xf0, 0xbc, 0xfd,
      // payload
      'g', 'o', 'o', 'd', 'b', 'y', 'e', '!',
  };
  const char* data = reinterpret_cast<const char*>(expected);
  size_t len = QUIC_ARRAYSIZE(expected);
  NullDecrypter decrypter(Perspective::IS_CLIENT);
  char buffer[256];
  size_t length = 0;
  ASSERT_FALSE(decrypter.DecryptPacket(QUIC_VERSION_35, 0, "hello world!",
                                       QuicStringPiece(data, len), buffer,
                                       &length, 256));
}

TEST_F(NullDecrypterTest, ShortInput) {
  unsigned char expected[] = {
      // fnv hash (truncated)
      0x46, 0x11, 0xea, 0x5f, 0xcf, 0x1d, 0x66, 0x5b, 0xba, 0xf0, 0xbc,
  };
  const char* data = reinterpret_cast<const char*>(expected);
  size_t len = QUIC_ARRAYSIZE(expected);
  NullDecrypter decrypter(Perspective::IS_CLIENT);
  char buffer[256];
  size_t length = 0;
  ASSERT_FALSE(decrypter.DecryptPacket(QUIC_VERSION_35, 0, "hello world!",
                                       QuicStringPiece(data, len), buffer,
                                       &length, 256));
}

}  // namespace test
}  // namespace gfe_quic
