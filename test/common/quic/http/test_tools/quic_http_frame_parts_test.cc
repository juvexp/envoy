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

#include "test/common/quic/http/test_tools/quic_http_frame_parts.h"

#include "testing/base/public/gunit-spi.h"
#include "testing/base/public/gunit.h"

namespace gfe_quic {
namespace test {
namespace {

class QuicHttpFramePartsTest : public ::testing::Test {};

TEST_F(QuicHttpFramePartsTest, UnpaddedDataFrame) {
  const QuicHttpFrameHeader h1(10, QuicHttpFrameType::DATA, 0, 123);
  QuicHttpFrameParts fp(h1);
  EXPECT_EQ(h1, fp.frame_header);
  EXPECT_EQ("", fp.payload);
  EXPECT_FALSE(fp.opt_payload_length);
  EXPECT_FALSE(fp.opt_pad_length);
  EXPECT_EQ("", fp.padding);
  EXPECT_FALSE(fp.got_start_callback);
  EXPECT_FALSE(fp.got_end_callback);

  fp.OnDataStart(h1);
  EXPECT_TRUE(fp.got_start_callback);
  EXPECT_FALSE(fp.got_end_callback);
  ASSERT_TRUE(fp.opt_payload_length);
  EXPECT_EQ(10, fp.opt_payload_length.value());

  QuicString s("abcdefghij");
  fp.OnDataPayload(s.data(), s.size());
  EXPECT_EQ(s, fp.payload);
  EXPECT_TRUE(fp.got_start_callback);
  EXPECT_FALSE(fp.got_end_callback);

  fp.OnDataEnd();
  EXPECT_EQ(s, fp.payload);
  EXPECT_TRUE(fp.got_start_callback);
  EXPECT_TRUE(fp.got_end_callback);
}

TEST_F(QuicHttpFramePartsTest, PaddedDataFrame) {
  // Padded frame...
  const QuicHttpFrameHeader header(10, QuicHttpFrameType::DATA,
                                   QuicHttpFrameFlag::QUIC_HTTP_PADDED |
                                       QuicHttpFrameFlag::QUIC_HTTP_END_STREAM,
                                   123);
  QuicHttpFrameParts fp(header);
  fp.OnDataStart(header);
  EXPECT_TRUE(fp.got_start_callback);
  EXPECT_FALSE(fp.got_end_callback);
  ASSERT_TRUE(fp.opt_payload_length);
  EXPECT_EQ(10, fp.opt_payload_length.value());

  fp.OnPadLength(1);
  ASSERT_TRUE(fp.opt_pad_length);
  EXPECT_EQ(1, fp.opt_pad_length.value());
  ASSERT_TRUE(fp.opt_payload_length);
  EXPECT_EQ(8, fp.opt_payload_length.value());

  QuicString t("abcdefgh");
  fp.OnDataPayload(t.data(), t.size());
  EXPECT_EQ(t, fp.payload);

  QuicString padding;
  padding.push_back(0);
  fp.OnPadding(padding.data(), padding.size());

  fp.OnDataEnd();

  EXPECT_EQ(t, fp.payload);
  EXPECT_EQ(padding, fp.padding);

  EXPECT_TRUE(fp.got_start_callback);
  EXPECT_TRUE(fp.got_end_callback);
}

// OnDataStart complains if header.type is not DATA.
TEST_F(QuicHttpFramePartsTest, OnDataStartWrongType) {
  EXPECT_FATAL_FAILURE(
      []() {
        const QuicHttpFrameHeader header(10, QuicHttpFrameType::DATA, 0, 123);
        QuicHttpFrameParts fp(
            QuicHttpFrameHeader(0, QuicHttpFrameType::HEADERS, 0, 1));
        fp.OnDataStart(header);
      }(),
      "DATA");
}

// OnDataStart complains if called twice.
TEST_F(QuicHttpFramePartsTest, OnDataStartTwice) {
  EXPECT_FATAL_FAILURE(
      []() {
        const QuicHttpFrameHeader header(9, QuicHttpFrameType::DATA, 0, 1);
        QuicHttpFrameParts fp(header);
        fp.OnDataStart(header);
        fp.OnDataStart(header);
      }(),
      "got_start_callback");
}

// OnDataPayload complains if OnDataStart has not been called.
TEST_F(QuicHttpFramePartsTest, OnDataPayloadBeforeStart) {
  EXPECT_FATAL_FAILURE(
      []() {
        QuicHttpFrameParts fp(
            QuicHttpFrameHeader(9, QuicHttpFrameType::DATA, 0, 1));
        fp.OnDataPayload(nullptr, 0);
      }(),
      "got_start_callback");
}

// OnDataPayload complains if too much data is provided.
TEST_F(QuicHttpFramePartsTest, OnDataPayloadTooLarge) {
  EXPECT_FATAL_FAILURE(
      []() {
        const QuicHttpFrameHeader header(3, QuicHttpFrameType::DATA, 0, 1);
        QuicHttpFrameParts fp(header);
        fp.OnDataStart(header);
        fp.OnDataPayload("ab", 2);
        fp.OnDataPayload("de", 2);
      }(),
      "String too large");
}

// OnDataEnd complains if OnDataStart has not been called.
TEST_F(QuicHttpFramePartsTest, OnDataEndBeforeStart) {
  EXPECT_FATAL_FAILURE(
      []() {
        QuicHttpFrameParts fp(
            QuicHttpFrameHeader(9, QuicHttpFrameType::DATA, 0, 1));
        fp.OnDataEnd();
      }(),
      "got_start_callback");
}

// OnPadLength complains if frame is not padded.
TEST_F(QuicHttpFramePartsTest, OnPadLengthWithUnpaddedDataFrame) {
  EXPECT_FATAL_FAILURE(
      []() {
        const QuicHttpFrameHeader header(10, QuicHttpFrameType::DATA, 0, 123);
        QuicHttpFrameParts fp(header);
        fp.OnDataStart(header);
        fp.OnPadLength(0);
      }(),
      "FrameIsPadded");
}

// OnPadding complains if frame is not padded.
TEST_F(QuicHttpFramePartsTest, OnPaddingWithUnpaddedDataFrame) {
  EXPECT_FATAL_FAILURE(
      []() {
        const QuicHttpFrameHeader header(10, QuicHttpFrameType::DATA, 0, 123);
        QuicHttpFrameParts fp(header);
        fp.OnDataStart(header);
        fp.OnPadding(nullptr, 0);
      }(),
      "FrameIsPadded");
}

// OnPadding complains if OnPadLength has not been called.
TEST_F(QuicHttpFramePartsTest, OnPaddingBeforePadLength) {
  EXPECT_FATAL_FAILURE(
      []() {
        const QuicHttpFrameHeader header(
            9, QuicHttpFrameType::DATA, QuicHttpFrameFlag::QUIC_HTTP_PADDED, 1);
        QuicHttpFrameParts fp(header);
        fp.OnDataStart(header);
        //         fp.OnPadLength(0);
        fp.OnPadding(nullptr, 0);
      }(),
      "opt_pad_length");
}

TEST_F(QuicHttpFramePartsTest, HeaderQuicHttpFrameParts) {
  const QuicHttpFrameHeader h1(10, QuicHttpFrameType::HEADERS, 0, 123);
  QuicHttpFrameParts fp(h1);
  EXPECT_EQ(h1, fp.frame_header);
  EXPECT_EQ("", fp.payload);
  EXPECT_FALSE(fp.opt_payload_length);
  EXPECT_FALSE(fp.opt_pad_length);
  EXPECT_EQ("", fp.padding);
  EXPECT_FALSE(fp.got_start_callback);
  EXPECT_FALSE(fp.got_end_callback);

  fp.OnHeadersStart(h1);
  EXPECT_TRUE(fp.got_start_callback);
  EXPECT_FALSE(fp.got_end_callback);
  ASSERT_TRUE(fp.opt_payload_length);
  EXPECT_EQ(10, fp.opt_payload_length.value());

  QuicString s("abcdefghij");
  fp.OnHpackFragment(s.data(), s.size());
  EXPECT_EQ(s, fp.payload);
  EXPECT_TRUE(fp.got_start_callback);
  EXPECT_FALSE(fp.got_end_callback);

  fp.OnHeadersEnd();
  EXPECT_EQ(s, fp.payload);
  EXPECT_TRUE(fp.got_start_callback);
  EXPECT_TRUE(fp.got_end_callback);
}

TEST_F(QuicHttpFramePartsTest, DISABLED_PriorityQuicHttpFrameParts) {
  // TODO: Implement this, or ensure that coverage is achieved by
  // other means.
}

TEST_F(QuicHttpFramePartsTest, DISABLED_RstStreamQuicHttpFrameParts) {
  // TODO: Implement this, or ensure that coverage is achieved by
  // other means.
}

TEST_F(QuicHttpFramePartsTest, DISABLED_SettingsQuicHttpFrameParts) {
  // TODO: Implement this, or ensure that coverage is achieved by
  // other means.
}

TEST_F(QuicHttpFramePartsTest, DISABLED_PushPromiseQuicHttpFrameParts) {
  // TODO: Implement this, or ensure that coverage is achieved by
  // other means.
}

TEST_F(QuicHttpFramePartsTest, DISABLED_PingQuicHttpFrameParts) {
  // TODO: Implement this, or ensure that coverage is achieved by
  // other means.
}

TEST_F(QuicHttpFramePartsTest, DISABLED_GoAwayQuicHttpFrameParts) {
  // TODO: Implement this, or ensure that coverage is achieved by
  // other means.
}

TEST_F(QuicHttpFramePartsTest, DISABLED_WindowUpdateQuicHttpFrameParts) {
  // TODO: Implement this, or ensure that coverage is achieved by
  // other means.
}

TEST_F(QuicHttpFramePartsTest, DISABLED_AltSvcQuicHttpFrameParts) {
  // TODO: Implement this, or ensure that coverage is achieved by
  // other means.
}

TEST_F(QuicHttpFramePartsTest, DISABLED_UnknownQuicHttpFrameParts) {
  // TODO: Implement this, or ensure that coverage is achieved by
  // other means.
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
