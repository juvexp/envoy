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

#include "common/quic/core/crypto/crypto_secret_boxer.h"

#include "common/quic/core/crypto/quic_random.h"
#include "common/quic/platform/api/quic_string.h"
#include "test/common/quic/platform/api/quic_test.h"

namespace gfe_quic {
namespace test {

class CryptoSecretBoxerTest : public QuicTest {};

TEST_F(CryptoSecretBoxerTest, BoxAndUnbox) {
  QuicStringPiece message("hello world");

  CryptoSecretBoxer boxer;
  boxer.SetKeys({QuicString(CryptoSecretBoxer::GetKeySize(), 0x11)});

  const QuicString box = boxer.Box(QuicRandom::GetInstance(), message);

  QuicString storage;
  QuicStringPiece result;
  EXPECT_TRUE(boxer.Unbox(box, &storage, &result));
  EXPECT_EQ(result, message);

  EXPECT_FALSE(boxer.Unbox(QuicString(1, 'X') + box, &storage, &result));
  EXPECT_FALSE(boxer.Unbox(box.substr(1, QuicString::npos), &storage, &result));
  EXPECT_FALSE(boxer.Unbox(QuicString(), &storage, &result));
  EXPECT_FALSE(boxer.Unbox(
      QuicString(1, box[0] ^ 0x80) + box.substr(1, QuicString::npos), &storage,
      &result));
}

// Helper function to test whether one boxer can decode the output of another.
static bool CanDecode(const CryptoSecretBoxer& decoder,
                      const CryptoSecretBoxer& encoder) {
  QuicStringPiece message("hello world");
  const QuicString boxed = encoder.Box(QuicRandom::GetInstance(), message);
  QuicString storage;
  QuicStringPiece result;
  bool ok = decoder.Unbox(boxed, &storage, &result);
  if (ok) {
    EXPECT_EQ(result, message);
  }
  return ok;
}

TEST_F(CryptoSecretBoxerTest, MultipleKeys) {
  QuicString key_11(CryptoSecretBoxer::GetKeySize(), 0x11);
  QuicString key_12(CryptoSecretBoxer::GetKeySize(), 0x12);

  CryptoSecretBoxer boxer_11, boxer_12, boxer;
  boxer_11.SetKeys({key_11});
  boxer_12.SetKeys({key_12});
  boxer.SetKeys({key_12, key_11});

  // Neither single-key boxer can decode the other's tokens.
  EXPECT_FALSE(CanDecode(boxer_11, boxer_12));
  EXPECT_FALSE(CanDecode(boxer_12, boxer_11));

  // |boxer| encodes with the first key, which is key_12.
  EXPECT_TRUE(CanDecode(boxer_12, boxer));
  EXPECT_FALSE(CanDecode(boxer_11, boxer));

  // The boxer with both keys can decode tokens from either single-key boxer.
  EXPECT_TRUE(CanDecode(boxer, boxer_11));
  EXPECT_TRUE(CanDecode(boxer, boxer_12));

  // After we flush key_11 from |boxer|, it can no longer decode tokens from
  // |boxer_11|.
  boxer.SetKeys({key_12});
  EXPECT_FALSE(CanDecode(boxer, boxer_11));
}

}  // namespace test
}  // namespace gfe_quic
