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

#include "common/quic/http/decoder/quic_http_decode_structures.h"

// Tests decoding all of the fixed size HTTP/2 structures (i.e. those defined
// in gfe/quic/http/quic_http_structures.h).

#include <stddef.h>

#include "base/commandlineflags.h"
#include "base/logging.h"
#include "common/quic/http/decoder/quic_http_decode_buffer.h"
#include "common/quic/http/decoder/quic_http_decode_status.h"
#include "common/quic/http/quic_http_constants.h"
#include "test/common/quic/http/quic_http_structures_test_util.h"
#include "test/common/quic/http/tools/quic_http_frame_builder.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"
#include "gtest/gtest.h"
#include "util/random/mt_random.h"

DEFINE_FLAG(uint64 /* allow-non-std-int */,
            random_decode_count,
            100,
            "Number of randomized structures of each type to decode.");

using ::testing::AssertionResult;

namespace gfe_quic {
namespace test {
namespace {

template <typename T, size_t N>
QuicStringPiece ToStringPiece(T (&data)[N]) {
  return QuicStringPiece(reinterpret_cast<const char*>(data), N * sizeof(T));
}

template <class S>
QuicString SerializeStructure(const S& s) {
  QuicHttpFrameBuilder fb;
  fb.Append(s);
  EXPECT_EQ(S::EncodedSize(), fb.size());
  return fb.buffer();
}

template <class S>
class StructureDecoderTest : public ::testing::Test {
 protected:
  typedef S Structure;

  StructureDecoderTest()
      : random_(FLAGS_test_random_seed),
        random_decode_count_(
            static_cast<size_t>(base::GetFlag(FLAGS_random_decode_count))) {
    LOG(INFO) << "Random seed (--test_random_seed): " << FLAGS_test_random_seed;
    CHECK_LE(random_decode_count_, 1000 * 1000) << "That should be plenty!";
  }

  // Set the fields of |*p| to random values.
  void Randomize(S* p) { ::gfe_quic::test::Randomize(p, &random_); }

  // Fully decodes the Structure at the start of data, and confirms it matches
  // *expected (if provided).
  void DecodeLeadingStructure(const S* expected, QuicStringPiece data) {
    ASSERT_LE(S::EncodedSize(), data.size());
    QuicHttpDecodeBuffer db(data);
    Randomize(&structure_);
    DoDecode(&structure_, &db);
    EXPECT_EQ(db.Offset(), S::EncodedSize());
    if (expected != nullptr) {
      EXPECT_EQ(structure_, *expected);
    }
  }

  template <size_t N>
  void DecodeLeadingStructure(const char (&data)[N]) {
    DecodeLeadingStructure(nullptr, QuicStringPiece(data, N));
  }

  // Encode the structure |in_s| into bytes, then decode the bytes
  // and validate that the decoder produced the same field values.
  void EncodeThenDecode(const S& in_s) {
    QuicString bytes = SerializeStructure(in_s);
    EXPECT_EQ(S::EncodedSize(), bytes.size());
    DecodeLeadingStructure(&in_s, bytes);
  }

  // Generate
  void TestDecodingRandomizedStructures(size_t count) {
    EXPECT_LT(count, 1000 * 1000) << "That should be plenty!";
    for (size_t i = 0; i < count && !HasFailure(); ++i) {
      Structure input;
      Randomize(&input);
      EncodeThenDecode(input);
    }
  }

  void TestDecodingRandomizedStructures() {
    TestDecodingRandomizedStructures(random_decode_count_);
  }

  MTRandom random_;
  const size_t random_decode_count_;
  uint32_t decode_offset_ = 0;
  S structure_;
  size_t fast_decode_count_ = 0;
  size_t slow_decode_count_ = 0;
};

class QuicHttpFrameHeaderDecoderTest
    : public StructureDecoderTest<QuicHttpFrameHeader> {};

TEST_F(QuicHttpFrameHeaderDecoderTest, DecodesLiteral) {
  {
    // Realistic input.
    const char kData[] = {
        0x00, 0x00, 0x05,        // Payload length: 5
        0x01,                    // Frame type: HEADERS
        0x08,                    // Flags: QUIC_HTTP_PADDED
        0x00, 0x00, 0x00, 0x01,  // Stream ID: 1
        0x04,                    // Padding length: 4
        0x00, 0x00, 0x00, 0x00,  // Padding bytes
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_EQ(5, structure_.payload_length);
      EXPECT_EQ(QuicHttpFrameType::HEADERS, structure_.type);
      EXPECT_EQ(QuicHttpFrameFlag::QUIC_HTTP_PADDED, structure_.flags);
      EXPECT_EQ(1, structure_.stream_id);
    }
  }
  {
    // Unlikely input.
    const char kData[] = {
        0xff, 0xff, 0xff,        // Payload length: uint24 max
        0xff,                    // Frame type: Unknown
        0xff,                    // Flags: Unknown/All
        0xff, 0xff, 0xff, 0xff,  // Stream ID: uint31 max, plus R-bit
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_EQ((1 << 24) - 1, structure_.payload_length);
      EXPECT_EQ(static_cast<QuicHttpFrameType>(255), structure_.type);
      EXPECT_EQ(255, structure_.flags);
      EXPECT_EQ(0x7fffffff, structure_.stream_id);
    }
  }
}

TEST_F(QuicHttpFrameHeaderDecoderTest, DecodesRandomized) {
  TestDecodingRandomizedStructures();
}

//------------------------------------------------------------------------------

class QuicHttpPriorityFieldsDecoderTest
    : public StructureDecoderTest<QuicHttpPriorityFields> {};

TEST_F(QuicHttpPriorityFieldsDecoderTest, DecodesLiteral) {
  {
    const char kData[] = {
        0x80, 0x00, 0x00, 0x05,  // Exclusive (yes) and Dependency (5)
        0xff,                    // Weight: 256 (after adding 1)
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_EQ(5, structure_.stream_dependency);
      EXPECT_EQ(256, structure_.weight);
      EXPECT_EQ(true, structure_.is_exclusive);
    }
  }
  {
    const char kData[] = {
        0x7f, 0xff, 0xff, 0xff,  // Exclusive (no) and Dependency (0x7fffffff)
        0x00,                    // Weight: 1 (after adding 1)
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_EQ(QuicHttpStreamIdMask(), structure_.stream_dependency);
      EXPECT_EQ(1, structure_.weight);
      EXPECT_FALSE(structure_.is_exclusive);
    }
  }
}

TEST_F(QuicHttpPriorityFieldsDecoderTest, DecodesRandomized) {
  TestDecodingRandomizedStructures();
}

//------------------------------------------------------------------------------

class QuicHttpRstStreamFieldsDecoderTest
    : public StructureDecoderTest<QuicHttpRstStreamFields> {};

TEST_F(QuicHttpRstStreamFieldsDecoderTest, DecodesLiteral) {
  {
    const char kData[] = {
        0x00, 0x00, 0x00, 0x01,  // Error: PROTOCOL_ERROR
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_TRUE(structure_.IsSupportedErrorCode());
      EXPECT_EQ(QuicHttpErrorCode::PROTOCOL_ERROR, structure_.error_code);
    }
  }
  {
    const char kData[] = {
        0xff, 0xff, 0xff, 0xff,  // Error: max uint32 (Unknown error code)
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_FALSE(structure_.IsSupportedErrorCode());
      EXPECT_EQ(static_cast<QuicHttpErrorCode>(0xffffffff),
                structure_.error_code);
    }
  }
}

TEST_F(QuicHttpRstStreamFieldsDecoderTest, DecodesRandomized) {
  TestDecodingRandomizedStructures();
}

//------------------------------------------------------------------------------

class QuicHttpSettingFieldsDecoderTest
    : public StructureDecoderTest<QuicHttpSettingFields> {};

TEST_F(QuicHttpSettingFieldsDecoderTest, DecodesLiteral) {
  {
    const char kData[] = {
        0x00, 0x01,              // Setting: HEADER_TABLE_SIZE
        0x00, 0x00, 0x40, 0x00,  // Value: 16K
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_TRUE(structure_.IsSupportedParameter());
      EXPECT_EQ(QuicHttpSettingsParameter::HEADER_TABLE_SIZE,
                structure_.parameter);
      EXPECT_EQ(1 << 14, structure_.value);
    }
  }
  {
    const char kData[] = {
        0x00, 0x00,              // Setting: Unknown (0)
        0xff, 0xff, 0xff, 0xff,  // Value: max uint32
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_FALSE(structure_.IsSupportedParameter());
      EXPECT_EQ(static_cast<QuicHttpSettingsParameter>(0),
                structure_.parameter);
    }
  }
}

TEST_F(QuicHttpSettingFieldsDecoderTest, DecodesRandomized) {
  TestDecodingRandomizedStructures();
}

//------------------------------------------------------------------------------

class QuicHttpPushPromiseFieldsDecoderTest
    : public StructureDecoderTest<QuicHttpPushPromiseFields> {};

TEST_F(QuicHttpPushPromiseFieldsDecoderTest, DecodesLiteral) {
  {
    const char kData[] = {
        0x00, 0x01, 0x8a, 0x92,  // Promised Stream ID: 101010
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_EQ(101010, structure_.promised_stream_id);
    }
  }
  {
    // Promised stream id has R-bit (reserved for future use) set, which
    // should be cleared by the decoder.
    const char kData[] = {
        0xff, 0xff, 0xff, 0xff,  // Promised Stream ID: max uint31 and R-bit
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_EQ(QuicHttpStreamIdMask(), structure_.promised_stream_id);
    }
  }
}

TEST_F(QuicHttpPushPromiseFieldsDecoderTest, DecodesRandomized) {
  TestDecodingRandomizedStructures();
}

//------------------------------------------------------------------------------

class QuicHttpPingFieldsDecoderTest
    : public StructureDecoderTest<QuicHttpPingFields> {};

TEST_F(QuicHttpPingFieldsDecoderTest, DecodesLiteral) {
  {
    // Each byte is different, so can detect if order changed.
    const char kData[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_EQ(QuicStringPiece(kData, 8),
                ToStringPiece(structure_.opaque_bytes));
    }
  }
  {
    // All zeros, detect problems handling NULs.
    const char kData[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_EQ(QuicStringPiece(kData, 8),
                ToStringPiece(structure_.opaque_bytes));
    }
  }
  {
    const char kData[] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_EQ(QuicStringPiece(kData, 8),
                ToStringPiece(structure_.opaque_bytes));
    }
  }
}

TEST_F(QuicHttpPingFieldsDecoderTest, DecodesRandomized) {
  TestDecodingRandomizedStructures();
}

//------------------------------------------------------------------------------

class QuicHttpGoAwayFieldsDecoderTest
    : public StructureDecoderTest<QuicHttpGoAwayFields> {};

TEST_F(QuicHttpGoAwayFieldsDecoderTest, DecodesLiteral) {
  {
    const char kData[] = {
        0x00, 0x00, 0x00, 0x00,  // Last Stream ID: 0
        0x00, 0x00, 0x00, 0x00,  // Error: NO_ERROR (0)
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_EQ(0, structure_.last_stream_id);
      EXPECT_TRUE(structure_.IsSupportedErrorCode());
      EXPECT_EQ(QuicHttpErrorCode::HTTP2_NO_ERROR, structure_.error_code);
    }
  }
  {
    const char kData[] = {
        0x00, 0x00, 0x00, 0x01,  // Last Stream ID: 1
        0x00, 0x00, 0x00, 0x0d,  // Error: HTTP_1_1_REQUIRED
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_EQ(1, structure_.last_stream_id);
      EXPECT_TRUE(structure_.IsSupportedErrorCode());
      EXPECT_EQ(QuicHttpErrorCode::HTTP_1_1_REQUIRED, structure_.error_code);
    }
  }
  {
    const char kData[] = {
        0xff, 0xff, 0xff, 0xff,  // Last Stream ID: max uint31 and R-bit
        0xff, 0xff, 0xff, 0xff,  // Error: max uint32 (Unknown error code)
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_EQ(QuicHttpStreamIdMask(),
                structure_.last_stream_id);  // No high-bit.
      EXPECT_FALSE(structure_.IsSupportedErrorCode());
      EXPECT_EQ(static_cast<QuicHttpErrorCode>(0xffffffff),
                structure_.error_code);
    }
  }
}

TEST_F(QuicHttpGoAwayFieldsDecoderTest, DecodesRandomized) {
  TestDecodingRandomizedStructures();
}

//------------------------------------------------------------------------------

class QuicHttpWindowUpdateFieldsDecoderTest
    : public StructureDecoderTest<QuicHttpWindowUpdateFields> {};

TEST_F(QuicHttpWindowUpdateFieldsDecoderTest, DecodesLiteral) {
  {
    const char kData[] = {
        0x00, 0x01, 0x00, 0x00,  // Window Size Increment: 2 ^ 16
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_EQ(1 << 16, structure_.window_size_increment);
    }
  }
  {
    // Increment must be non-zero, but we need to be able to decode the invalid
    // zero to detect it.
    const char kData[] = {
        0x00, 0x00, 0x00, 0x00,  // Window Size Increment: 0
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_EQ(0, structure_.window_size_increment);
    }
  }
  {
    // Increment has R-bit (reserved for future use) set, which
    // should be cleared by the decoder.
    // clang-format off
    const char kData[] = {
        // Window Size Increment: max uint31 and R-bit
        0xff, 0xff, 0xff, 0xff,
    };
    // clang-format on
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_EQ(QuicHttpStreamIdMask(), structure_.window_size_increment);
    }
  }
}

TEST_F(QuicHttpWindowUpdateFieldsDecoderTest, DecodesRandomized) {
  TestDecodingRandomizedStructures();
}

//------------------------------------------------------------------------------

class QuicHttpAltSvcFieldsDecoderTest
    : public StructureDecoderTest<QuicHttpAltSvcFields> {};

TEST_F(QuicHttpAltSvcFieldsDecoderTest, DecodesLiteral) {
  {
    const char kData[] = {
        0x00, 0x00,  // Origin Length: 0
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_EQ(0, structure_.origin_length);
    }
  }
  {
    const char kData[] = {
        0x00, 0x14,  // Origin Length: 20
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_EQ(20, structure_.origin_length);
    }
  }
  {
    const char kData[] = {
        0xff, 0xff,  // Origin Length: uint16 max
    };
    DecodeLeadingStructure(kData);
    if (!HasFailure()) {
      EXPECT_EQ(65535, structure_.origin_length);
    }
  }
}

TEST_F(QuicHttpAltSvcFieldsDecoderTest, DecodesRandomized) {
  TestDecodingRandomizedStructures();
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
