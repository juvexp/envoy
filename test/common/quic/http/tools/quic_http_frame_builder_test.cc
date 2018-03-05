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

#include "test/common/quic/http/tools/quic_http_frame_builder.h"

// Tests of QuicHttpFrameBuilder.

#include "testing/base/public/gunit-spi.h"
#include "testing/base/public/gunit.h"

namespace gfe_quic {
namespace test {
namespace {

const char kHighBitSetMsg[] = "High-bit of uint32_t should be clear";

TEST(QuicHttpFrameBuilderTest, Constructors) {
  {
    QuicHttpFrameBuilder fb;
    EXPECT_EQ(0, fb.size());
  }
  {
    QuicHttpFrameBuilder fb(QuicHttpFrameType::DATA, 0, 123);
    EXPECT_EQ(9, fb.size());

    const char kData[] = {
        0x00, 0x00, 0x00,        // Payload length: 0 (unset)
        0x00,                    // Frame type: DATA
        0x00,                    // Flags: none
        0x00, 0x00, 0x00, 0x7b,  // Stream ID: 123
    };
    EXPECT_EQ(QuicStringPiece(kData, sizeof kData), fb.buffer());
  }
  {
    QuicHttpFrameHeader header;
    header.payload_length = (1 << 24) - 1;
    header.type = QuicHttpFrameType::HEADERS;
    header.flags = QuicHttpFrameFlag::QUIC_HTTP_END_HEADERS;
    header.stream_id = QuicHttpStreamIdMask();
    QuicHttpFrameBuilder fb(header);
    EXPECT_EQ(9, fb.size());

    const char kData[] = {
        0xff, 0xff, 0xff,        // Payload length: 2^24 - 1 (max uint24)
        0x01,                    // Frame type: HEADER
        0x04,                    // Flags: QUIC_HTTP_END_HEADERS
        0x7f, 0xff, 0xff, 0xff,  // Stream ID: stream id mask
    };
    EXPECT_EQ(QuicStringPiece(kData, sizeof kData), fb.buffer());
  }
}

TEST(QuicHttpFrameBuilderTest, SetPayloadLength) {
  QuicHttpFrameBuilder fb(QuicHttpFrameType::DATA, QUIC_HTTP_PADDED, 20000);
  EXPECT_EQ(9, fb.size());

  fb.AppendUInt8(50);  // Trailing payload length
  EXPECT_EQ(10, fb.size());

  fb.Append("ten bytes.");
  EXPECT_EQ(20, fb.size());

  fb.AppendZeroes(50);
  EXPECT_EQ(70, fb.size());

  fb.SetPayloadLength();
  EXPECT_EQ(70, fb.size());

  // clang-format off
  const char kData[] = {
      0x00, 0x00, 0x3d,              // Payload length: 61
      0x00,                          // Frame type: DATA
      0x08,                          // Flags: QUIC_HTTP_PADDED
      0x00, 0x00, 0x4e, 0x20,        // Stream ID: 20000
      0x32,                          // Padding Length: 50
      't', 'e', 'n', ' ', 'b',       // "ten b"
      'y', 't', 'e', 's', '.',       // "ytes."
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // Padding bytes
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // Padding bytes
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // Padding bytes
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // Padding bytes
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // Padding bytes
  };
  // clang-format on
  EXPECT_EQ(QuicStringPiece(kData, sizeof kData), fb.buffer());
}

TEST(QuicHttpFrameBuilderTest, Settings) {
  QuicHttpFrameBuilder fb(QuicHttpFrameType::SETTINGS, 0, 0);
  QuicHttpSettingFields sf;

  sf.parameter = QuicHttpSettingsParameter::HEADER_TABLE_SIZE;
  sf.value = 1 << 12;
  fb.Append(sf);

  sf.parameter = QuicHttpSettingsParameter::ENABLE_PUSH;
  sf.value = 0;
  fb.Append(sf);

  sf.parameter = QuicHttpSettingsParameter::MAX_CONCURRENT_STREAMS;
  sf.value = ~0;
  fb.Append(sf);

  sf.parameter = QuicHttpSettingsParameter::INITIAL_WINDOW_SIZE;
  sf.value = 1 << 16;
  fb.Append(sf);

  sf.parameter = QuicHttpSettingsParameter::MAX_FRAME_SIZE;
  sf.value = 1 << 14;
  fb.Append(sf);

  sf.parameter = QuicHttpSettingsParameter::MAX_HEADER_LIST_SIZE;
  sf.value = 1 << 10;
  fb.Append(sf);

  size_t payload_size = 6 * QuicHttpSettingFields::EncodedSize();
  EXPECT_EQ(QuicHttpFrameHeader::EncodedSize() + payload_size, fb.size());

  fb.SetPayloadLength(payload_size);

  // clang-format off
  const char kData[] = {
      0x00, 0x00, 0x24,        // Payload length: 36
      0x04,                    // Frame type: SETTINGS
      0x00,                    // Flags: none
      0x00, 0x00, 0x00, 0x00,  // Stream ID: 0
      0x00, 0x01,              // HEADER_TABLE_SIZE
      0x00, 0x00, 0x10, 0x00,  // 4096
      0x00, 0x02,              // ENABLE_PUSH
      0x00, 0x00, 0x00, 0x00,  // 0
      0x00, 0x03,              // MAX_CONCURRENT_STREAMS
      0xff, 0xff, 0xff, 0xff,  // 0xffffffff (max uint32)
      0x00, 0x04,              // INITIAL_WINDOW_SIZE
      0x00, 0x01, 0x00, 0x00,  // 4096
      0x00, 0x05,              // MAX_FRAME_SIZE
      0x00, 0x00, 0x40, 0x00,  // 4096
      0x00, 0x06,              // MAX_HEADER_LIST_SIZE
      0x00, 0x00, 0x04, 0x00,  // 1024
  };
  // clang-format on
  EXPECT_EQ(QuicStringPiece(kData, 9), fb.buffer().substr(0, 9));
  for (int n = 0; n < 6; ++n) {
    int offset = 9 + n * 6;
    EXPECT_EQ(QuicStringPiece(kData + offset, 6), fb.buffer().substr(offset, 6))
        << "Setting #" << n;
  }
  EXPECT_EQ(QuicStringPiece(kData, sizeof kData), fb.buffer());
}

TEST(QuicHttpFrameBuilderTest, EnhanceYourCalm) {
  const char kData[] = {0x00, 0x00, 0x00, 0x0b};
  const QuicStringPiece expected(kData, sizeof kData);
  {
    QuicHttpFrameBuilder fb;
    fb.Append(QuicHttpErrorCode::ENHANCE_YOUR_CALM);
    EXPECT_EQ(expected, fb.buffer());
  }
  {
    QuicHttpFrameBuilder fb;
    QuicHttpRstStreamFields rsp;
    rsp.error_code = QuicHttpErrorCode::ENHANCE_YOUR_CALM;
    fb.Append(rsp);
    EXPECT_EQ(expected, fb.buffer());
  }
}

TEST(QuicHttpFrameBuilderTest, PushPromise) {
  const char kData[] = {0x7f, 0xff, 0xff, 0xff};
  {
    QuicHttpFrameBuilder fb;
    fb.Append(QuicHttpPushPromiseFields{0x7fffffff});
    EXPECT_EQ(QuicStringPiece(kData, sizeof kData), fb.buffer());
  }
  {
    QuicHttpFrameBuilder fb;
    // Will generate an error if the high-bit of the stream id is set.
    EXPECT_NONFATAL_FAILURE(fb.Append(QuicHttpPushPromiseFields{0xffffffff}),
                            kHighBitSetMsg);
    EXPECT_EQ(QuicStringPiece(kData, sizeof kData), fb.buffer());
  }
}

TEST(QuicHttpFrameBuilderTest, Ping) {
  QuicHttpFrameBuilder fb;
  QuicHttpPingFields ping{"8 bytes"};
  fb.Append(ping);

  const char kData[] = {'8', ' ', 'b', 'y', 't', 'e', 's', '\0'};
  EXPECT_EQ(sizeof kData, QuicHttpPingFields::EncodedSize());
  EXPECT_EQ(QuicStringPiece(kData, sizeof kData), fb.buffer());
}

TEST(QuicHttpFrameBuilderTest, GoAway) {
  const char kData[] = {
      0x12, 0x34, 0x56, 0x78,  // Last Stream Id
      0x00, 0x00, 0x00, 0x01,  // Error code
  };
  EXPECT_EQ(sizeof kData, QuicHttpGoAwayFields::EncodedSize());
  {
    QuicHttpFrameBuilder fb;
    QuicHttpGoAwayFields ga(0x12345678, QuicHttpErrorCode::PROTOCOL_ERROR);
    fb.Append(ga);
    EXPECT_EQ(QuicStringPiece(kData, sizeof kData), fb.buffer());
  }
  {
    QuicHttpFrameBuilder fb;
    // Will generate a test failure if the high-bit of the stream id is set.
    QuicHttpGoAwayFields ga(0x92345678, QuicHttpErrorCode::PROTOCOL_ERROR);
    EXPECT_NONFATAL_FAILURE(fb.Append(ga), kHighBitSetMsg);
    EXPECT_EQ(QuicStringPiece(kData, sizeof kData), fb.buffer());
  }
}

TEST(QuicHttpFrameBuilderTest, WindowUpdate) {
  QuicHttpFrameBuilder fb;
  fb.Append(QuicHttpWindowUpdateFields{123456});

  // Will generate a test failure if the high-bit of the increment is set.
  EXPECT_NONFATAL_FAILURE(fb.Append(QuicHttpWindowUpdateFields{0x80000001}),
                          kHighBitSetMsg);

  // Will generate a test failure if the increment is zero.
  EXPECT_NONFATAL_FAILURE(fb.Append(QuicHttpWindowUpdateFields{0}), "non-zero");

  const char kData[] = {
      0x00, 0x01, 0xe2, 0x40,  // Valid Window Size Increment
      0x00, 0x00, 0x00, 0x01,  // High-bit cleared
      0x00, 0x00, 0x00, 0x00,  // Invalid Window Size Increment
  };
  EXPECT_EQ(sizeof kData, 3 * QuicHttpWindowUpdateFields::EncodedSize());
  EXPECT_EQ(QuicStringPiece(kData, sizeof kData), fb.buffer());
}

TEST(QuicHttpFrameBuilderTest, AltSvc) {
  QuicHttpFrameBuilder fb;
  fb.Append(QuicHttpAltSvcFields{99});
  fb.Append(QuicHttpAltSvcFields{0});  // No optional origin
  const char kData[] = {
      0x00, 0x63,  // Has origin.
      0x00, 0x00,  // Doesn't have origin.
  };
  EXPECT_EQ(sizeof kData, 2 * QuicHttpAltSvcFields::EncodedSize());
  EXPECT_EQ(QuicStringPiece(kData, sizeof kData), fb.buffer());
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
