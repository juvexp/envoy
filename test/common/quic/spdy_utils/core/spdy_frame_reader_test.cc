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

// Copyright 2012 Google Inc. All Rights Reserved.
// Author: hkhalil@google.com (Hasan Khalil)

#include "common/quic/spdy_utils/core/spdy_frame_reader.h"

// For ntohs() and ntohl()
#ifdef WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

#include "base/macros.h"
#include "testing/base/public/gunit.h"

namespace gfe_spdy {

TEST(SpdyFrameReaderTest, ReadUInt16) {
  // Frame data in network byte order.
  const uint16_t kFrameData[] = {
      htons(1), htons(1 << 15),
  };

  SpdyFrameReader frame_reader(reinterpret_cast<const char*>(kFrameData),
                               sizeof(kFrameData));
  EXPECT_FALSE(frame_reader.IsDoneReading());

  uint16_t uint16_val;
  EXPECT_TRUE(frame_reader.ReadUInt16(&uint16_val));
  EXPECT_FALSE(frame_reader.IsDoneReading());
  EXPECT_EQ(1, uint16_val);

  EXPECT_TRUE(frame_reader.ReadUInt16(&uint16_val));
  EXPECT_TRUE(frame_reader.IsDoneReading());
  EXPECT_EQ(1 << 15, uint16_val);
}

TEST(SpdyFrameReaderTest, ReadUInt32) {
  // Frame data in network byte order.
  const uint32_t kFrameData[] = {
      htonl(1), htonl(1 << 31),
  };

  SpdyFrameReader frame_reader(reinterpret_cast<const char*>(kFrameData),
                               arraysize(kFrameData) * sizeof(uint32_t));
  EXPECT_FALSE(frame_reader.IsDoneReading());

  uint32_t uint32_val;
  EXPECT_TRUE(frame_reader.ReadUInt32(&uint32_val));
  EXPECT_FALSE(frame_reader.IsDoneReading());
  EXPECT_EQ(1u, uint32_val);

  EXPECT_TRUE(frame_reader.ReadUInt32(&uint32_val));
  EXPECT_TRUE(frame_reader.IsDoneReading());
  EXPECT_EQ(1u << 31, uint32_val);
}

TEST(SpdyFrameReaderTest, ReadStringPiece16) {
  // Frame data in network byte order.
  const char kFrameData[] = {
      0x00, 0x02,  // uint16_t(2)
      0x48, 0x69,  // "Hi"
      0x00, 0x10,  // uint16_t(16)
      0x54, 0x65, 0x73, 0x74, 0x69, 0x6e, 0x67, 0x2c,
      0x20, 0x31, 0x2c, 0x20, 0x32, 0x2c, 0x20, 0x33,  // "Testing, 1, 2, 3"
  };

  SpdyFrameReader frame_reader(kFrameData, arraysize(kFrameData));
  EXPECT_FALSE(frame_reader.IsDoneReading());

  SpdyStringPiece stringpiece_val;
  EXPECT_TRUE(frame_reader.ReadStringPiece16(&stringpiece_val));
  EXPECT_FALSE(frame_reader.IsDoneReading());
  EXPECT_EQ(0, stringpiece_val.compare("Hi"));

  EXPECT_TRUE(frame_reader.ReadStringPiece16(&stringpiece_val));
  EXPECT_TRUE(frame_reader.IsDoneReading());
  EXPECT_EQ(0, stringpiece_val.compare("Testing, 1, 2, 3"));
}

TEST(SpdyFrameReaderTest, ReadStringPiece32) {
  // Frame data in network byte order.
  const char kFrameData[] = {
      0x00, 0x00, 0x00, 0x03,  // uint32_t(3)
      0x66, 0x6f, 0x6f,        // "foo"
      0x00, 0x00, 0x00, 0x10,  // uint32_t(16)
      0x54, 0x65, 0x73, 0x74, 0x69, 0x6e, 0x67, 0x2c,
      0x20, 0x34, 0x2c, 0x20, 0x35, 0x2c, 0x20, 0x36,  // "Testing, 4, 5, 6"
  };

  SpdyFrameReader frame_reader(kFrameData, arraysize(kFrameData));
  EXPECT_FALSE(frame_reader.IsDoneReading());

  SpdyStringPiece stringpiece_val;
  EXPECT_TRUE(frame_reader.ReadStringPiece32(&stringpiece_val));
  EXPECT_FALSE(frame_reader.IsDoneReading());
  EXPECT_EQ(0, stringpiece_val.compare("foo"));

  EXPECT_TRUE(frame_reader.ReadStringPiece32(&stringpiece_val));
  EXPECT_TRUE(frame_reader.IsDoneReading());
  EXPECT_EQ(0, stringpiece_val.compare("Testing, 4, 5, 6"));
}

TEST(SpdyFrameReaderTest, ReadUInt16WithBufferTooSmall) {
  // Frame data in network byte order.
  const char kFrameData[] = {
      0x00,  // part of a uint16_t
  };

  SpdyFrameReader frame_reader(kFrameData, arraysize(kFrameData));
  EXPECT_FALSE(frame_reader.IsDoneReading());

  uint16_t uint16_val;
  EXPECT_FALSE(frame_reader.ReadUInt16(&uint16_val));
}

TEST(SpdyFrameReaderTest, ReadUInt32WithBufferTooSmall) {
  // Frame data in network byte order.
  const char kFrameData[] = {
      0x00, 0x00, 0x00,  // part of a uint32_t
  };

  SpdyFrameReader frame_reader(kFrameData, arraysize(kFrameData));
  EXPECT_FALSE(frame_reader.IsDoneReading());

  uint32_t uint32_val;
  EXPECT_FALSE(frame_reader.ReadUInt32(&uint32_val));

  // Also make sure that trying to read a uint16_t, which technically could
  // work, fails immediately due to previously encountered failed read.
  uint16_t uint16_val;
  EXPECT_FALSE(frame_reader.ReadUInt16(&uint16_val));
}

// Tests ReadStringPiece16() with a buffer too small to fit the entire string.
TEST(SpdyFrameReaderTest, ReadStringPiece16WithBufferTooSmall) {
  // Frame data in network byte order.
  const char kFrameData[] = {
      0x00, 0x03,  // uint16_t(3)
      0x48, 0x69,  // "Hi"
  };

  SpdyFrameReader frame_reader(kFrameData, arraysize(kFrameData));
  EXPECT_FALSE(frame_reader.IsDoneReading());

  SpdyStringPiece stringpiece_val;
  EXPECT_FALSE(frame_reader.ReadStringPiece16(&stringpiece_val));

  // Also make sure that trying to read a uint16_t, which technically could
  // work, fails immediately due to previously encountered failed read.
  uint16_t uint16_val;
  EXPECT_FALSE(frame_reader.ReadUInt16(&uint16_val));
}

// Tests ReadStringPiece16() with a buffer too small even to fit the length.
TEST(SpdyFrameReaderTest, ReadStringPiece16WithBufferWayTooSmall) {
  // Frame data in network byte order.
  const char kFrameData[] = {
      0x00,  // part of a uint16_t
  };

  SpdyFrameReader frame_reader(kFrameData, arraysize(kFrameData));
  EXPECT_FALSE(frame_reader.IsDoneReading());

  SpdyStringPiece stringpiece_val;
  EXPECT_FALSE(frame_reader.ReadStringPiece16(&stringpiece_val));

  // Also make sure that trying to read a uint16_t, which technically could
  // work, fails immediately due to previously encountered failed read.
  uint16_t uint16_val;
  EXPECT_FALSE(frame_reader.ReadUInt16(&uint16_val));
}

// Tests ReadStringPiece32() with a buffer too small to fit the entire string.
TEST(SpdyFrameReaderTest, ReadStringPiece32WithBufferTooSmall) {
  // Frame data in network byte order.
  const char kFrameData[] = {
      0x00, 0x00, 0x00, 0x03,  // uint32_t(3)
      0x48, 0x69,              // "Hi"
  };

  SpdyFrameReader frame_reader(kFrameData, arraysize(kFrameData));
  EXPECT_FALSE(frame_reader.IsDoneReading());

  SpdyStringPiece stringpiece_val;
  EXPECT_FALSE(frame_reader.ReadStringPiece32(&stringpiece_val));

  // Also make sure that trying to read a uint16_t, which technically could
  // work, fails immediately due to previously encountered failed read.
  uint16_t uint16_val;
  EXPECT_FALSE(frame_reader.ReadUInt16(&uint16_val));
}

// Tests ReadStringPiece32() with a buffer too small even to fit the length.
TEST(SpdyFrameReaderTest, ReadStringPiece32WithBufferWayTooSmall) {
  // Frame data in network byte order.
  const char kFrameData[] = {
      0x00, 0x00, 0x00,  // part of a uint32_t
  };

  SpdyFrameReader frame_reader(kFrameData, arraysize(kFrameData));
  EXPECT_FALSE(frame_reader.IsDoneReading());

  SpdyStringPiece stringpiece_val;
  EXPECT_FALSE(frame_reader.ReadStringPiece32(&stringpiece_val));

  // Also make sure that trying to read a uint16_t, which technically could
  // work, fails immediately due to previously encountered failed read.
  uint16_t uint16_val;
  EXPECT_FALSE(frame_reader.ReadUInt16(&uint16_val));
}

TEST(SpdyFrameReaderTest, ReadBytes) {
  // Frame data in network byte order.
  const char kFrameData[] = {
      0x66, 0x6f, 0x6f,  // "foo"
      0x48, 0x69,        // "Hi"
  };

  SpdyFrameReader frame_reader(kFrameData, arraysize(kFrameData));
  EXPECT_FALSE(frame_reader.IsDoneReading());

  char dest1[3] = {};
  EXPECT_TRUE(frame_reader.ReadBytes(&dest1, arraysize(dest1)));
  EXPECT_FALSE(frame_reader.IsDoneReading());
  EXPECT_EQ("foo", SpdyStringPiece(dest1, arraysize(dest1)));

  char dest2[2] = {};
  EXPECT_TRUE(frame_reader.ReadBytes(&dest2, arraysize(dest2)));
  EXPECT_TRUE(frame_reader.IsDoneReading());
  EXPECT_EQ("Hi", SpdyStringPiece(dest2, arraysize(dest2)));
}

TEST(SpdyFrameReaderTest, ReadBytesWithBufferTooSmall) {
  // Frame data in network byte order.
  const char kFrameData[] = {
      0x01,
  };

  SpdyFrameReader frame_reader(kFrameData, arraysize(kFrameData));
  EXPECT_FALSE(frame_reader.IsDoneReading());

  char dest[arraysize(kFrameData) + 2] = {};
  EXPECT_FALSE(frame_reader.ReadBytes(&dest, arraysize(kFrameData) + 1));
  EXPECT_STREQ("", dest);
}

}  // namespace gfe_spdy
