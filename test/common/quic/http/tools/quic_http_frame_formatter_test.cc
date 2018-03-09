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

#include "test/common/quic/http/tools/quic_http_frame_formatter.h"

#include "base/logging.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace gfe_quic {
namespace test {
namespace {

TEST(IdentifyCppLineTypeTest, StartOfCharArray) {
  EXPECT_EQ(kStartOfCharArray,
            IdentifyCppLineType("const unsigned char F[]={// Comment"));
  EXPECT_EQ(kStartOfCharArray,
            IdentifyCppLineType("  const  unsigned  char  F  [ ]  =  { //X"));
  EXPECT_EQ(kStartOfCharArray,
            IdentifyCppLineType("const unsigned char FOO[]={"));
  EXPECT_EQ(kStartOfCharArray,
            IdentifyCppLineType("  const  unsigned  char  FOO  [ ]  =  { "));
  EXPECT_EQ(kStartOfCharArray, IdentifyCppLineType("const\tchar FOO[]={"));
  EXPECT_EQ(kStartOfCharArray,
            IdentifyCppLineType("const char kFrameData[] = {"));
  EXPECT_EQ(kStartOfCharArray,
            IdentifyCppLineType("  const unsigned char kH2Input[] = {"));
}

TEST(IdentifyCppLineTypeTest, CharLiteral) {
  EXPECT_EQ(kCharLiteral, IdentifyCppLineType("   '\\n'  "));
  EXPECT_EQ(kCharLiteral, IdentifyCppLineType("   '\\t'  "));
  EXPECT_EQ(kCharLiteral, IdentifyCppLineType("   'x'"));
  EXPECT_EQ(kCharLiteral, IdentifyCppLineType("   ' '"));
}

TEST(IdentifyCppLineTypeTest, WhitespaceOrLineComment) {
  EXPECT_EQ(kWhitespaceOrLineComment, IdentifyCppLineType("   \n"));
  EXPECT_EQ(kWhitespaceOrLineComment, IdentifyCppLineType("   //"));
  EXPECT_EQ(kWhitespaceOrLineComment, IdentifyCppLineType("  "));
  EXPECT_EQ(kWhitespaceOrLineComment, IdentifyCppLineType("\n"));
  EXPECT_EQ(kWhitespaceOrLineComment, IdentifyCppLineType("//"));
  EXPECT_EQ(kWhitespaceOrLineComment, IdentifyCppLineType(""));
}

TEST(IdentifyCppLineTypeTest, ClangFormatOff) {
  EXPECT_EQ(kClangFormatOff, IdentifyCppLineType("// clang-format off"));
}

TEST(IdentifyCppLineTypeTest, ClangFormatOn) {
  EXPECT_EQ(kClangFormatOn, IdentifyCppLineType("// clang-format on"));
}

TEST(IdentifyCppLineTypeTest, FrameFormatOff) {
  EXPECT_EQ(kFrameFormatOff, IdentifyCppLineType("// frame-format off"));
}

TEST(IdentifyCppLineTypeTest, FrameFormatOn) {
  EXPECT_EQ(kFrameFormatOn, IdentifyCppLineType("// frame-format on"));
}

TEST(IdentifyCppLineTypeTest, EndOfCharArray) {
  EXPECT_EQ(kEndOfCharArray, IdentifyCppLineType("   }  ;   "));
  EXPECT_EQ(kEndOfCharArray, IdentifyCppLineType("};"));
}

TEST(IdentifyCppLineTypeTest, Other) {
  EXPECT_EQ(kOther, IdentifyCppLineType("const unsigned char FOO[]="));
  EXPECT_EQ(kOther, IdentifyCppLineType("   const unsigned char    []   ={"));
  EXPECT_EQ(kOther, IdentifyCppLineType("   const char    []   ={"));
}

TEST(ParseCharLiteralsInLineTest, HexAndCharLiterals) {
  bool comma_at_end = false;
  auto status_or_parsed = ParseCharLiteralsInLine(
      R"#( 0x01 , 32, 'a', // comment  )#", &comma_at_end);
  EXPECT_OK(status_or_parsed.status());
  EXPECT_EQ("\x01 a", status_or_parsed.ConsumeValueOrDie());
  EXPECT_TRUE(comma_at_end);
}

TEST(ParseCharLiteralsInLineTest, ClassicEscapedCharLiterals) {
  bool comma_at_end = true;
  auto status_or_parsed = ParseCharLiteralsInLine(
      R"#('\'','\"','\?','\\','\a','\b','\f','\n','\r','\t','\v',)#"
      R"#('\377','\X7f','\0')#",
      &comma_at_end);
  EXPECT_OK(status_or_parsed.status());
  QuicString actual(status_or_parsed.ValueOrDie());
  QuicString expected = R"#('"?\)#"
                        "\a\b\f\n\r\t\v\xFF\177";
  expected.push_back(0);
  EXPECT_EQ(expected, actual);
  EXPECT_EQ(expected.size(), actual.size());
  EXPECT_FALSE(comma_at_end);
}

// Not a test, just useful for debugging.
TEST(FormatFrameHeaderTest, LogFormattedFrame) {
  const char kFrameData[] = {
      0x00, 0x00, 0x14,  // 20 byte payload
      0x01,              // Type: HEADERS
      0x8c,              // Flags: QUIC_HTTP_END_HEADERS | QUIC_HTTP_PADDED
      0x80, 0x00, 0x00, 0x01,  // Stream 1 with reserved bit
      0x01,                    // 1 byte of trailing padding
      0x00,                    // Unindexed, literal name & value
      0x03, 0x62, 0x61, 0x72,  // Name len and name (3, "bar")
      0x03, 0x66, 0x6f, 0x6f,  // Value len and value (3, "foo")
      0x00,                    // Unindexed, literal name & value
      0x03, 0x66, 0x6f, 0x6f,  // Name len and name (3, "foo")
      0x03, 0x62, 0x61, 0x72,  // Value len and value (3, "bar")
      0x00,                    // Trailing padding.
  };
  QuicStringPiece encoded_frame_data(kFrameData, sizeof kFrameData);
  LOG(INFO) << "formatted:\n\n"
            << FormatFrameHeader(encoded_frame_data, "<prefix>") << "\n\n";
  LOG(INFO) << "formatted again:\n\n"
            << FormatFrameHeader(encoded_frame_data, "  ") << "\n\n";
}

// Not a test, just useful for debugging.
TEST(FormatFramesInFileContents, FormatAndLog) {
  QuicString file_contents = R"#(
  // clang-format off
  const char kFrameData[] = {
      0x00, 0x00, 0x14,        // 20 byte payload
      0x01,                    // Type: HEADERS
      0x8c,                    // Flags: QUIC_HTTP_END_HEADERS | QUIC_HTTP_PADDED
      0x80, 0x00, 0x00, 0x01,  // Stream 1 with reserved bit
      0x01,                    // 1 byte of trailing padding
      0x00,                    // Unindexed, literal name & value
      0x03, 0x62, 0x61, 0x72,  // Name len and name (3, "bar")
      0x03, 0x66, 0x6f, 0x6f,  // Value len and value (3, "foo")
      0x00,                    // Unindexed, literal name & value
      0x03, 0x66, 0x6f, 0x6f,  // Name len and name (3, "foo")
      0x03, 0x62, 0x61, 0x72,  // Value len and value (3, "bar")
      0x00,                    // Trailing padding.
  };
  // clang-format on



  // SYN_STREAM and SYN_REPLY don't exist in HTTP2, so instead we send
  // HEADERS frames with QUIC_HTTP_PRIORITY(SYN_STREAM only) and QUIC_HTTP_END_HEADERS set.
  const unsigned char kH2Input[] = {
    0x00, 0x00, 0x05, 0x01,  // HEADERS: QUIC_HTTP_PRIORITY | QUIC_HTTP_END_HEADERS
    0x24, 0x00, 0x00, 0x00,  // Stream 1
    0x01, 0x00, 0x00, 0x00,  // Priority 0
    0x00, 0x82,              // :method: GET

    0x00, 0x00, 0x01, 0x01,  // HEADERS: QUIC_HTTP_END_HEADERS
    0x04, 0x00, 0x00, 0x00,  // Stream 1
    0x01, 0x8c,              // :status: 200

    0x00, 0x00, 0x0c, 0x00,  // DATA on Stream #1
    0x00, 0x00, 0x00, 0x00,
    0x01, 0xde, 0xad, 0xbe,
    0xef, 0xde, 0xad, 0xbe,
    0xef, 0xde, 0xad, 0xbe,
    0xef,

    0x00, 0x00, 0x04, 0x00,  // DATA on Stream #1, with FIN
    0x01, 0x00, 0x00, 0x00,
    0x01, 0xde, 0xad, 0xbe,
    0xef,
  };



// Tests handling of QUIC_HTTP_PRIORITY frame with incorrect size.
TEST_P(SpdyFramerTest, ReadIncorrectlySizedPriority) {
  if (!IsQuicHttp()) {
    return;
  }

  // QUIC_HTTP_PRIORITY frame of size 4, which isn't correct.
  const unsigned char kFrameData[] = {
    0x00, 0x00, 0x04, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x01,
  };

  TestSpdyVisitor visitor(spdy_version_);
  visitor.SimulateInFramer(kFrameData, sizeof(kFrameData));

  EXPECT_EQ(SpdyFramer::SPDY_ERROR, visitor.framer_.state());
  EXPECT_EQ(SpdyFramer::SPDY_INVALID_CONTROL_FRAME_SIZE,
            visitor.framer_.spdy_framer_error())
      << SpdyFramer::SpdyFramerErrorToString(visitor.framer_.spdy_framer_error());
}


)#";

  util::StatusOr<QuicString> status_or_string =
      FormatFramesInFileContents(file_contents);
  ASSERT_OK(status_or_string.status());
  LOG(INFO) << status_or_string.ConsumeValueOrDie();
}

}  // namespace

}  // namespace test
}  // namespace gfe_quic
