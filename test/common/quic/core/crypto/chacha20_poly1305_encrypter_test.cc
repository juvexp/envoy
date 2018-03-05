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

#include "common/quic/core/crypto/chacha20_poly1305_encrypter.h"

#include <memory>

#include "common/quic/core/crypto/chacha20_poly1305_decrypter.h"
#include "common/quic/core/quic_utils.h"
#include "common/quic/platform/api/quic_arraysize.h"
#include "common/quic/platform/api/quic_string.h"
#include "test/common/quic/platform/api/quic_test.h"
#include "common/quic/platform/api/quic_text_utils.h"
#include "test/common/quic/test_tools/quic_test_utils.h"

namespace {

// The test vectors come from RFC 7539 Section 2.8.2.

// Each test vector consists of five strings of lowercase hexadecimal digits.
// The strings may be empty (zero length). A test vector with a nullptr |key|
// marks the end of an array of test vectors.
struct TestVector {
  const char* key;
  const char* pt;
  const char* iv;
  const char* fixed;
  const char* aad;
  const char* ct;
};

const TestVector test_vectors[] = {
    {
        "808182838485868788898a8b8c8d8e8f"
        "909192939495969798999a9b9c9d9e9f",

        "4c616469657320616e642047656e746c"
        "656d656e206f662074686520636c6173"
        "73206f66202739393a20496620492063"
        "6f756c64206f6666657220796f75206f"
        "6e6c79206f6e652074697020666f7220"
        "746865206675747572652c2073756e73"
        "637265656e20776f756c642062652069"
        "742e",

        "4041424344454647",

        "07000000",

        "50515253c0c1c2c3c4c5c6c7",

        "d31a8d34648e60db7b86afbc53ef7ec2"
        "a4aded51296e08fea9e2b5a736ee62d6"
        "3dbea45e8ca9671282fafb69da92728b"
        "1a71de0a9e060b2905d6a5b67ecd3b36"
        "92ddbd7f2d778b8c9803aee328091b58"
        "fab324e4fad675945585808b4831d7bc"
        "3ff4def08e4b7a9de576d26586cec64b"
        "6116"
        "1ae10b594f09e26a7e902ecb",  // "d0600691" truncated
    },
    {nullptr}};

}  // namespace

namespace gfe_quic {
namespace test {

// EncryptWithNonce wraps the |Encrypt| method of |encrypter| to allow passing
// in an nonce and also to allocate the buffer needed for the ciphertext.
QuicData* EncryptWithNonce(ChaCha20Poly1305Encrypter* encrypter,
                           QuicStringPiece nonce,
                           QuicStringPiece associated_data,
                           QuicStringPiece plaintext) {
  size_t ciphertext_size = encrypter->GetCiphertextSize(plaintext.length());
  std::unique_ptr<char[]> ciphertext(new char[ciphertext_size]);

  if (!encrypter->Encrypt(nonce, associated_data, plaintext,
                          reinterpret_cast<unsigned char*>(ciphertext.get()))) {
    return nullptr;
  }

  return new QuicData(ciphertext.release(), ciphertext_size, true);
}

class ChaCha20Poly1305EncrypterTest : public QuicTest {};

TEST_F(ChaCha20Poly1305EncrypterTest, EncryptThenDecrypt) {
  ChaCha20Poly1305Encrypter encrypter;
  ChaCha20Poly1305Decrypter decrypter;

  QuicString key = QuicTextUtils::HexDecode(test_vectors[0].key);
  ASSERT_TRUE(encrypter.SetKey(key));
  ASSERT_TRUE(decrypter.SetKey(key));
  ASSERT_TRUE(encrypter.SetNoncePrefix("abcd"));
  ASSERT_TRUE(decrypter.SetNoncePrefix("abcd"));

  QuicPacketNumber packet_number = UINT64_C(0x123456789ABC);
  QuicString associated_data = "associated_data";
  QuicString plaintext = "plaintext";
  char encrypted[1024];
  size_t len;
  ASSERT_TRUE(encrypter.EncryptPacket(QuicTransportVersionMax(), packet_number,
                                      associated_data, plaintext, encrypted,
                                      &len, QUIC_ARRAYSIZE(encrypted)));
  QuicStringPiece ciphertext(encrypted, len);
  char decrypted[1024];
  ASSERT_TRUE(decrypter.DecryptPacket(QuicTransportVersionMax(), packet_number,
                                      associated_data, ciphertext, decrypted,
                                      &len, QUIC_ARRAYSIZE(decrypted)));
}

TEST_F(ChaCha20Poly1305EncrypterTest, Encrypt) {
  for (size_t i = 0; test_vectors[i].key != nullptr; i++) {
    // Decode the test vector.
    QuicString key = QuicTextUtils::HexDecode(test_vectors[i].key);
    QuicString pt = QuicTextUtils::HexDecode(test_vectors[i].pt);
    QuicString iv = QuicTextUtils::HexDecode(test_vectors[i].iv);
    QuicString fixed = QuicTextUtils::HexDecode(test_vectors[i].fixed);
    QuicString aad = QuicTextUtils::HexDecode(test_vectors[i].aad);
    QuicString ct = QuicTextUtils::HexDecode(test_vectors[i].ct);

    ChaCha20Poly1305Encrypter encrypter;
    ASSERT_TRUE(encrypter.SetKey(key));
    std::unique_ptr<QuicData> encrypted(EncryptWithNonce(
        &encrypter, fixed + iv,
        // This deliberately tests that the encrypter can handle an AAD that
        // is set to nullptr, as opposed to a zero-length, non-nullptr pointer.
        QuicStringPiece(aad.length() ? aad.data() : nullptr, aad.length()),
        pt));
    ASSERT_TRUE(encrypted.get());
    EXPECT_EQ(12u, ct.size() - pt.size());
    EXPECT_EQ(12u, encrypted->length() - pt.size());

    test::CompareCharArraysWithHexError("ciphertext", encrypted->data(),
                                        encrypted->length(), ct.data(),
                                        ct.length());
  }
}

TEST_F(ChaCha20Poly1305EncrypterTest, GetMaxPlaintextSize) {
  ChaCha20Poly1305Encrypter encrypter;
  EXPECT_EQ(1000u, encrypter.GetMaxPlaintextSize(1012));
  EXPECT_EQ(100u, encrypter.GetMaxPlaintextSize(112));
  EXPECT_EQ(10u, encrypter.GetMaxPlaintextSize(22));
}

TEST_F(ChaCha20Poly1305EncrypterTest, GetCiphertextSize) {
  ChaCha20Poly1305Encrypter encrypter;
  EXPECT_EQ(1012u, encrypter.GetCiphertextSize(1000));
  EXPECT_EQ(112u, encrypter.GetCiphertextSize(100));
  EXPECT_EQ(22u, encrypter.GetCiphertextSize(10));
}

}  // namespace test
}  // namespace gfe_quic
