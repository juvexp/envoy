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

#include "common/quic/http/decoder/quic_http_structure_decoder.h"

// Tests decoding all of the fixed size HTTP/2 structures that are in the frame
// payload (i.e. those defined in gfe/quic/http/quic_http_structures.h, except
// QuicHttpFrameHeader) using QuicHttpStructureDecoder, which handles buffering
// of structures split across input buffer boundaries, and in turn uses DoDecode
// when it has all of a structure in a contiguous buffer.

#include <stddef.h>

#include <cstdint>

#include "base/commandlineflags.h"
#include "base/logging.h"
#include "common/quic/http/decoder/quic_http_decode_buffer.h"
#include "common/quic/http/decoder/quic_http_decode_status.h"
#include "test/common/quic/http/decoder/quic_http_structure_decoder_test_util.h"
#include "common/quic/http/quic_http_constants.h"
#include "common/quic/http/quic_http_structures.h"
#include "test/common/quic/http/quic_http_structures_test_util.h"
#include "test/common/quic/http/tools/quic_http_random_decoder_test.h"
#include "common/quic/platform/api/quic_reconstruct_object.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"
#include "common/quic/platform/api/quic_string_utils.h"
#include "common/quic/platform/api/quic_text_utils.h"
#include "gtest/gtest.h"

DEFINE_FLAG(uint64 /* allow-non-std-int */,
            random_decode_count,
            100,
            "Number of randomized structures of each type to decode.");

using ::testing::AssertionFailure;
using ::testing::AssertionResult;
using ::testing::AssertionSuccess;

namespace gfe_quic {
namespace test {
namespace {

// A template providing the base for all of the type-specific tests.
template <class Structure>
class QuicHttpStructureDecoderRemainingPayloadTest
    : public QuicHttpRandomDecoderTest {
 protected:
  QuicHttpStructureDecoderRemainingPayloadTest() {
    // IF the test adds more data after the encoded structure, stop as
    // soon as the structure is decoded.
    stop_decode_on_done_ = true;
  }

  void RandomizeStructure(Structure* ptr) {
    gfe_quic::test::Randomize(ptr, RandomPtr());
  }

  QuicHttpDecodeStatus StartDecoding(QuicHttpDecodeBuffer* b) override {
    // Overwrite the current contents of |structure_|, in to which we'll
    // decode the buffer, so that we can be confident that we really decoded
    // the structure every time.
    QuicDefaultReconstructObject(&structure_, RandomPtr());

    // Do the same with the decoder so that we can be sure the decoder
    // is not depending on or affected by the state from the previous
    // decoding calls.
    QuicHttpStructureDecoderPeer::Randomize(&structure_decoder_, RandomPtr());

    const uint32_t avail = b->Remaining();

    VLOG(2) << "StartDecoding, avail=" << b->Remaining()
            << "; needed=" << Structure::EncodedSize();

    remaining_payload_ = remaining_payload_at_start_;
    QuicHttpDecodeStatus status =
        structure_decoder_.Start(&structure_, b, &remaining_payload_);

    const uint32_t consumed_payload =
        remaining_payload_at_start_ - remaining_payload_;
    const uint32_t consumed_input = avail - b->Remaining();
    EXPECT_EQ(consumed_payload, consumed_input);

    if (status == QuicHttpDecodeStatus::kDecodeDone) {
      EXPECT_EQ(consumed_payload, Structure::EncodedSize());
      ++fast_decode_count_;
    } else if (status == QuicHttpDecodeStatus::kDecodeInProgress) {
      EXPECT_LT(avail, Structure::EncodedSize());
      EXPECT_EQ(0, b->Remaining());
      EXPECT_EQ(avail, structure_decoder_.offset());
      EXPECT_GT(remaining_payload_, 0);
      ++incomplete_start_count_;
    } else {
      EXPECT_EQ(status, QuicHttpDecodeStatus::kDecodeError);
      EXPECT_EQ(remaining_payload_, 0);
      ++error_start_count_;
    }
    return status;
  }

  QuicHttpDecodeStatus ResumeDecoding(QuicHttpDecodeBuffer* b) override {
    const uint32_t old_offset = structure_decoder_.offset();
    EXPECT_LT(old_offset, Structure::EncodedSize());
    const uint32_t needed = Structure::EncodedSize() - old_offset;
    const uint32_t avail = b->Remaining();

    VLOG(2) << "ResumeDecoding, avail=" << b->Remaining()
            << "; needed=" << needed;

    const uint32_t old_remaining_payload = remaining_payload_;
    if (structure_decoder_.Resume(&structure_, b, &remaining_payload_)) {
      EXPECT_EQ(structure_decoder_.offset(), Structure::EncodedSize());
      EXPECT_GT(old_remaining_payload, remaining_payload_);
      const uint32_t consumed_payload =
          old_remaining_payload - remaining_payload_;
      const uint32_t consumed_input = avail - b->Remaining();
      EXPECT_EQ(needed, consumed_payload);
      EXPECT_EQ(needed, consumed_input);
      ++slow_decode_count_;
      return QuicHttpDecodeStatus::kDecodeDone;
    } else if (remaining_payload_ == 0) {
      EXPECT_GT(needed, old_remaining_payload);
      ++error_resume_count_;
      return QuicHttpDecodeStatus::kDecodeError;
    } else {
      // There is more payload to come, whether or not it is sufficient to
      // meet the needs of decoding a Structure.
      EXPECT_GE(old_remaining_payload, remaining_payload_);
      EXPECT_EQ(0, b->Remaining());
      const uint32_t consumed = old_remaining_payload - remaining_payload_;
      const uint32_t copied = structure_decoder_.offset() - old_offset;
      EXPECT_EQ(copied, avail);
      EXPECT_EQ(copied, consumed);
      EXPECT_LT(avail, needed);
      EXPECT_LT(structure_decoder_.offset(), Structure::EncodedSize());
      ++incomplete_resume_count_;
      return QuicHttpDecodeStatus::kDecodeInProgress;
    }
  }

  // Fully decodes the structure at the start of data, and confirms it matches
  // *expected (if provided) if initial_remaining_payload is big enough. Note
  // that data is expected to be big enough to include an instance of the
  // Structure, and possibly more than that.
  AssertionResult DecodeLeadingStructure(const Structure* expected,
                                         QuicStringPiece data,
                                         int initial_remaining_payload) {
    VLOG(2) << "DecodeLeadingStructure: " << data.size()
            << " bytes of data: " << QuicTextUtils::HexDump(data)
            << "\n   initial_remaining_payload=" << initial_remaining_payload
            << "\n   needed=" << Structure::EncodedSize();
    if (expected != nullptr) {
      VLOG(2) << "DecodeLeadingStructure: expected: " << *expected;
    }

    remaining_payload_at_start_ = initial_remaining_payload;

    VERIFY_LE(Structure::EncodedSize(), data.size());

    QuicHttpDecodeBuffer original(data.substr(0, initial_remaining_payload));

    // The validator is called after each of the several times that the input
    // QuicHttpDecodeBuffer is decoded, each with a different segmentation of
    // the input. Create an appropriate validator based on whether the payload
    // is big enough or not.

    Validator validator;
    if (initial_remaining_payload >= Structure::EncodedSize()) {
      // We expect complete decoding.
      NoArgValidator inner_validator = [expected, this]() -> AssertionResult {
        // remaining_payload_at_start_ should have been large enough.
        VERIFY_LE(Structure::EncodedSize(), remaining_payload_at_start_);

        // Remaining_payload_ should have dropped by Structure::EncodedSize().
        VERIFY_EQ(remaining_payload_ + Structure::EncodedSize(),
                  remaining_payload_at_start_);

        // Validate that structure_ matches the expected value, if provided.
        if (expected != nullptr) {
          VERIFY_EQ(*expected, structure_);
        }
        return AssertionSuccess();
      };

      // First validate that decoding is done and that we've advanced the cursor
      // the expected amount (i.e. the size of the encoded structure).
      validator =
          ValidateDoneAndOffset(Structure::EncodedSize(), inner_validator);
    } else {
      // Not enough payload left, so decoding should fail.

      // We shouldn't have an expected value if we can't decode the structure.
      VERIFY_LT(initial_remaining_payload, Structure::EncodedSize());

      validator = [this](const QuicHttpDecodeBuffer& db,
                         QuicHttpDecodeStatus status) -> AssertionResult {
        VERIFY_EQ(status, QuicHttpDecodeStatus::kDecodeError);
        VERIFY_GT(Structure::EncodedSize(), remaining_payload_at_start_);
        VERIFY_EQ(0, remaining_payload_);
        // Should have filled the structure_decoder_'s buffer with all of the
        // available data.
        VERIFY_EQ(remaining_payload_at_start_, structure_decoder_.offset());
        return AssertionSuccess();
      };
    }

    // Decode several times, with several segmentations of the input buffer.

    fast_decode_count_ = 0;
    slow_decode_count_ = 0;
    error_start_count_ = 0;
    error_resume_count_ = 0;
    incomplete_start_count_ = 0;
    incomplete_resume_count_ = 0;

    VERIFY_SUCCESS(DecodeAndValidateSeveralWays(
        &original, false /*return_non_zero_on_first*/, validator));

    // Check that both the fast and slow decode or error paths have been tested,
    // and if successful that the expected structure has been decoded.
    if (initial_remaining_payload >= Structure::EncodedSize()) {
      // Should have advanced the original QuicHttpDecodeBuffer to the end of
      // the encoded structure.
      VERIFY_EQ(Structure::EncodedSize(), original.Offset());

      // Should have done both a fast decode, where StartDecoding was passed
      // the entire input, and also a slow decode, where StartDecoding didn't
      // get the whole thing, including an empty buffer.
      VERIFY_LT(0, fast_decode_count_);
      VERIFY_LT(0, slow_decode_count_);
      VERIFY_LE(0, incomplete_start_count_);

      // There should have been no errors during decoding.
      VERIFY_EQ(0, error_start_count_);
      VERIFY_EQ(0, error_resume_count_);

      // If the input is at least two bytes long, then at least one segmentation
      // of the input will result in ResumeDecoding being called twice, so we
      // should have counted at least one incomplete resume.
      if (Structure::EncodedSize() >= 2) {
        VERIFY_LE(0, incomplete_resume_count_);
      }

      // Double check that the decoded structure has the expected value.
      if (expected != nullptr) {
        VERIFY_EQ(*expected, structure_);
      }
    } else {
      // We didn't have enough remaining payload, so we should have buffered
      // all that was available in the decoder, and reached the end of the
      // payload.
      VERIFY_EQ(initial_remaining_payload, structure_decoder_.offset());
      VERIFY_EQ(remaining_payload_, 0);

      // Should not have succeeded in decoding fast or slow.
      VERIFY_EQ(0, fast_decode_count_);
      VERIFY_EQ(0, slow_decode_count_);

      // Should determined there was an error during StartDecoding
      // at least once.
      VERIFY_LT(0, error_start_count_);

      // If the input is at least one byte long, then at least one segmentation
      // will result in an empty buffer being passed to StartDecoding.
      if (initial_remaining_payload >= 1) {
        VERIFY_LT(0, incomplete_start_count_);
        VERIFY_LT(0, error_resume_count_);

        // And if the input is at least 2 bytes long, then at least one
        // segmentation of the input will result in ResumeDecoding being called
        // twice, so we should have counted at least one incomplete resume.
        if (initial_remaining_payload >= 2) {
          VERIFY_LT(0, incomplete_resume_count_);
        }
      }
    }
    return AssertionSuccess();
  }

  // Fully decodes the Structure at the start of data, and confirms it matches
  // *expected (if provided).
  AssertionResult DecodeLeadingStructure(const Structure* expected,
                                         QuicStringPiece data) {
    VLOG(2) << "DecodeLeadingStructure: " << QuicTextUtils::HexDump(data);
    // Decode with various sizes of remaining payload, from none,
    // to insufficient, to enough, and finally more than enough.
    for (int initial_remaining_payload = 0;
         initial_remaining_payload <= Structure::EncodedSize() + 1;
         ++initial_remaining_payload) {
      VERIFY_SUCCESS(
          DecodeLeadingStructure(expected, data, initial_remaining_payload));
    }
    return AssertionSuccess();
  }

  // We use unsigned here because MSVC considers chars signed by default, so
  // char literals that are 0x80 or greater cause compiler errors if we don't
  // declare our literal arrays to be of type unsigned char[].
  template <size_t N>
  AssertionResult DecodeLeadingStructure(const unsigned char (&data)[N]) {
    QuicStringPiece s(reinterpret_cast<const char*>(data), N);
    VLOG(2) << "DecodeLeadingStructure: " << QuicTextUtils::HexDump(s);
    VERIFY_AND_RETURN_SUCCESS(DecodeLeadingStructure(nullptr, s));
  }

  // Encode the structure |in_s| into bytes, then decode the bytes
  // and validate that the decoder produced the same field values.
  AssertionResult EncodeThenDecode(const Structure& in_s) {
    QuicString bytes = SerializeStructure(in_s);
    EXPECT_EQ(Structure::EncodedSize(), bytes.size());
    VERIFY_AND_RETURN_SUCCESS(DecodeLeadingStructure(&in_s, bytes));
  }

  // Fill a Structure with random values, encode and decode. And repeat.
  AssertionResult TestDecodingRandomizedStructures(size_t count) {
    for (size_t i = 0; i < count; ++i) {
      Structure input;
      RandomizeStructure(&input);
      VERIFY_SUCCESS(EncodeThenDecode(input));
    }
    return AssertionSuccess();
  }

  uint32_t decode_offset_ = 0;
  Structure structure_;
  QuicHttpStructureDecoder structure_decoder_;
  size_t fast_decode_count_ = 0;
  size_t slow_decode_count_ = 0;
  size_t incomplete_start_count_ = 0;
  size_t incomplete_resume_count_ = 0;
  size_t error_start_count_ = 0;
  size_t error_resume_count_ = 0;

 private:
  uint32_t remaining_payload_;
  uint32_t remaining_payload_at_start_;
};

//------------------------------------------------------------------------------

class QuicHttpPriorityFieldsDecoderTest
    : public QuicHttpStructureDecoderRemainingPayloadTest<
          QuicHttpPriorityFields> {};

TEST_F(QuicHttpPriorityFieldsDecoderTest, DecodesLiteral) {
  {
    const unsigned char kData[] = {
        0x80, 0x00, 0x00, 0x05,  // Exclusive (yes) and Dependency (5)
        0xff,                    // Weight: 256 (after adding 1)
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_EQ(5, structure_.stream_dependency);
    EXPECT_EQ(256, structure_.weight);
    EXPECT_EQ(true, structure_.is_exclusive);
  }
  {
    const unsigned char kData[] = {
        0x7f, 0xff, 0xff, 0xff,  // Exclusive (no) and Dependency (0x7fffffff)
        0x00,                    // Weight: 1 (after adding 1)
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_EQ(QuicHttpStreamIdMask(), structure_.stream_dependency);
    EXPECT_EQ(1, structure_.weight);
    EXPECT_EQ(false, structure_.is_exclusive);
  }
}

TEST_F(QuicHttpPriorityFieldsDecoderTest, DecodesRandomized) {
  uint64_t count = base::GetFlag(FLAGS_random_decode_count);
  EXPECT_LT(count, 1000 * 1000) << "That should be plenty!";
  EXPECT_TRUE(TestDecodingRandomizedStructures(count));
}

//------------------------------------------------------------------------------

class QuicHttpRstStreamFieldsDecoderTest
    : public QuicHttpStructureDecoderRemainingPayloadTest<
          QuicHttpRstStreamFields> {};

TEST_F(QuicHttpRstStreamFieldsDecoderTest, DecodesLiteral) {
  {
    const unsigned char kData[] = {
        0x00, 0x00, 0x00, 0x01,  // Error: PROTOCOL_ERROR
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_TRUE(structure_.IsSupportedErrorCode());
    EXPECT_EQ(QuicHttpErrorCode::PROTOCOL_ERROR, structure_.error_code);
  }
  {
    const unsigned char kData[] = {
        0xff, 0xff, 0xff, 0xff,  // Error: max uint32 (Unknown error code)
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_FALSE(structure_.IsSupportedErrorCode());
    EXPECT_EQ(static_cast<QuicHttpErrorCode>(0xffffffff),
              structure_.error_code);
  }
}

TEST_F(QuicHttpRstStreamFieldsDecoderTest, DecodesRandomized) {
  uint64_t count = base::GetFlag(FLAGS_random_decode_count);
  EXPECT_LT(count, 1000 * 1000) << "That should be plenty!";
  EXPECT_TRUE(TestDecodingRandomizedStructures(count));
}

//------------------------------------------------------------------------------

class QuicHttpSettingFieldsDecoderTest
    : public QuicHttpStructureDecoderRemainingPayloadTest<
          QuicHttpSettingFields> {};

TEST_F(QuicHttpSettingFieldsDecoderTest, DecodesLiteral) {
  {
    const unsigned char kData[] = {
        0x00, 0x01,              // Setting: HEADER_TABLE_SIZE
        0x00, 0x00, 0x40, 0x00,  // Value: 16K
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_TRUE(structure_.IsSupportedParameter());
    EXPECT_EQ(QuicHttpSettingsParameter::HEADER_TABLE_SIZE,
              structure_.parameter);
    EXPECT_EQ(1 << 14, structure_.value);
  }
  {
    const unsigned char kData[] = {
        0x00, 0x00,              // Setting: Unknown (0)
        0xff, 0xff, 0xff, 0xff,  // Value: max uint32
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_FALSE(structure_.IsSupportedParameter());
    EXPECT_EQ(static_cast<QuicHttpSettingsParameter>(0), structure_.parameter);
  }
}

TEST_F(QuicHttpSettingFieldsDecoderTest, DecodesRandomized) {
  uint64_t count = base::GetFlag(FLAGS_random_decode_count);
  EXPECT_LT(count, 1000 * 1000) << "That should be plenty!";
  EXPECT_TRUE(TestDecodingRandomizedStructures(count));
}

//------------------------------------------------------------------------------

class QuicHttpPushPromiseFieldsDecoderTest
    : public QuicHttpStructureDecoderRemainingPayloadTest<
          QuicHttpPushPromiseFields> {};

TEST_F(QuicHttpPushPromiseFieldsDecoderTest, DecodesLiteral) {
  {
    const unsigned char kData[] = {
        0x00, 0x01, 0x8a, 0x92,  // Promised Stream ID: 101010
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_EQ(101010, structure_.promised_stream_id);
  }
  {
    // Promised stream id has R-bit (reserved for future use) set, which
    // should be cleared by the decoder.
    const unsigned char kData[] = {
        0xff, 0xff, 0xff, 0xff,  // Promised Stream ID: max uint31 and R-bit
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_EQ(QuicHttpStreamIdMask(), structure_.promised_stream_id);
  }
}

TEST_F(QuicHttpPushPromiseFieldsDecoderTest, DecodesRandomized) {
  uint64_t count = base::GetFlag(FLAGS_random_decode_count);
  EXPECT_LT(count, 1000 * 1000) << "That should be plenty!";
  EXPECT_TRUE(TestDecodingRandomizedStructures(count));
}

//------------------------------------------------------------------------------

class QuicHttpPingFieldsDecoderTest
    : public QuicHttpStructureDecoderRemainingPayloadTest<QuicHttpPingFields> {
};

TEST_F(QuicHttpPingFieldsDecoderTest, DecodesLiteral) {
  {
    // Each byte is different, so can detect if order changed.
    const unsigned char kData[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_EQ(ToStringPiece(kData), ToStringPiece(structure_.opaque_bytes));
  }
  {
    // All zeros, detect problems handling NULs.
    const unsigned char kData[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_EQ(ToStringPiece(kData), ToStringPiece(structure_.opaque_bytes));
  }
  {
    const unsigned char kData[] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_EQ(ToStringPiece(kData), ToStringPiece(structure_.opaque_bytes));
  }
}

TEST_F(QuicHttpPingFieldsDecoderTest, DecodesRandomized) {
  uint64_t count = base::GetFlag(FLAGS_random_decode_count);
  EXPECT_LT(count, 1000 * 1000) << "That should be plenty!";
  EXPECT_TRUE(TestDecodingRandomizedStructures(count));
}

//------------------------------------------------------------------------------

class QuicHttpGoAwayFieldsDecoderTest
    : public QuicHttpStructureDecoderRemainingPayloadTest<
          QuicHttpGoAwayFields> {};

TEST_F(QuicHttpGoAwayFieldsDecoderTest, DecodesLiteral) {
  {
    const unsigned char kData[] = {
        0x00, 0x00, 0x00, 0x00,  // Last Stream ID: 0
        0x00, 0x00, 0x00, 0x00,  // Error: INTERNAL_ERROR
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_EQ(0, structure_.last_stream_id);
    EXPECT_TRUE(structure_.IsSupportedErrorCode());
    EXPECT_EQ(QuicHttpErrorCode::HTTP2_NO_ERROR, structure_.error_code);
  }
  {
    const unsigned char kData[] = {
        0x00, 0x00, 0x00, 0x01,  // Last Stream ID: 1
        0x00, 0x00, 0x00, 0x0d,  // Error: HTTP_1_1_REQUIRED
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_EQ(1, structure_.last_stream_id);
    EXPECT_TRUE(structure_.IsSupportedErrorCode());
    EXPECT_EQ(QuicHttpErrorCode::HTTP_1_1_REQUIRED, structure_.error_code);
  }
  {
    const unsigned char kData[] = {
        0xff, 0xff, 0xff, 0xff,  // Last Stream ID: max uint31 and R-bit
        0xff, 0xff, 0xff, 0xff,  // Error: max uint32 (Unknown error code)
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_EQ(QuicHttpStreamIdMask(),
              structure_.last_stream_id);  // No high-bit.
    EXPECT_FALSE(structure_.IsSupportedErrorCode());
    EXPECT_EQ(static_cast<QuicHttpErrorCode>(0xffffffff),
              structure_.error_code);
  }
}

TEST_F(QuicHttpGoAwayFieldsDecoderTest, DecodesRandomized) {
  uint64_t count = base::GetFlag(FLAGS_random_decode_count);
  EXPECT_LT(count, 1000 * 1000) << "That should be plenty!";
  EXPECT_TRUE(TestDecodingRandomizedStructures(count));
}

//------------------------------------------------------------------------------

class QuicHttpWindowUpdateFieldsDecoderTest
    : public QuicHttpStructureDecoderRemainingPayloadTest<
          QuicHttpWindowUpdateFields> {};

TEST_F(QuicHttpWindowUpdateFieldsDecoderTest, DecodesLiteral) {
  {
    const unsigned char kData[] = {
        0x00, 0x01, 0x00, 0x00,  // Window Size Increment: 2 ^ 16
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_EQ(1 << 16, structure_.window_size_increment);
  }
  {
    // Increment must be non-zero, but we need to be able to decode the invalid
    // zero to detect it.
    const unsigned char kData[] = {
        0x00, 0x00, 0x00, 0x00,  // Window Size Increment: 0
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_EQ(0, structure_.window_size_increment);
  }
  {
    // Increment has R-bit (reserved for future use) set, which
    // should be cleared by the decoder.
    const unsigned char kData[] = {
        0xff, 0xff, 0xff, 0xff,  // Window Size Increment: max uint31 and R-bit
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_EQ(QuicHttpStreamIdMask(), structure_.window_size_increment);
  }
}

TEST_F(QuicHttpWindowUpdateFieldsDecoderTest, DecodesRandomized) {
  uint64_t count = base::GetFlag(FLAGS_random_decode_count);
  EXPECT_LT(count, 1000 * 1000) << "That should be plenty!";
  EXPECT_TRUE(TestDecodingRandomizedStructures(count));
}

//------------------------------------------------------------------------------

class QuicHttpAltSvcFieldsDecoderTest
    : public QuicHttpStructureDecoderRemainingPayloadTest<
          QuicHttpAltSvcFields> {};

TEST_F(QuicHttpAltSvcFieldsDecoderTest, DecodesLiteral) {
  {
    const unsigned char kData[] = {
        0x00, 0x00,  // Origin Length: 0
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_EQ(0, structure_.origin_length);
  }
  {
    const unsigned char kData[] = {
        0x00, 0x14,  // Origin Length: 20
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_EQ(20, structure_.origin_length);
  }
  {
    const unsigned char kData[] = {
        0xff, 0xff,  // Origin Length: 20
    };
    EXPECT_TRUE(DecodeLeadingStructure(kData));
    EXPECT_EQ(65535, structure_.origin_length);
  }
}

TEST_F(QuicHttpAltSvcFieldsDecoderTest, DecodesRandomized) {
  uint64_t count = base::GetFlag(FLAGS_random_decode_count);
  EXPECT_LT(count, 1000 * 1000) << "That should be plenty!";
  EXPECT_TRUE(TestDecodingRandomizedStructures(count));
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
