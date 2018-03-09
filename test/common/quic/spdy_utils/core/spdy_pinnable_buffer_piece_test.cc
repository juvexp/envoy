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

#include "common/quic/spdy_utils/core/spdy_pinnable_buffer_piece.h"

#include "common/quic/spdy_utils/core/spdy_prefixed_buffer_reader.h"
#include "common/quic/spdy_utils/platform/api/spdy_string.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace gfe_spdy {

namespace test {

class SpdyPinnableBufferPieceTest : public ::testing::Test {
 protected:
  SpdyPrefixedBufferReader Build(const SpdyString& prefix,
                                 const SpdyString& suffix) {
    prefix_ = prefix;
    suffix_ = suffix;
    return SpdyPrefixedBufferReader(prefix_.data(), prefix_.length(),
                                    suffix_.data(), suffix_.length());
  }
  SpdyString prefix_, suffix_;
};

TEST_F(SpdyPinnableBufferPieceTest, Pin) {
  SpdyPrefixedBufferReader reader = Build("foobar", "");
  SpdyPinnableBufferPiece piece;
  EXPECT_TRUE(reader.ReadN(6, &piece));

  // Piece points to underlying prefix storage.
  EXPECT_EQ(SpdyStringPiece("foobar"), SpdyStringPiece(piece));
  EXPECT_FALSE(piece.IsPinned());
  EXPECT_EQ(prefix_.data(), piece.buffer());

  piece.Pin();

  // Piece now points to allocated storage.
  EXPECT_EQ(SpdyStringPiece("foobar"), SpdyStringPiece(piece));
  EXPECT_TRUE(piece.IsPinned());
  EXPECT_NE(prefix_.data(), piece.buffer());

  // Pinning again has no effect.
  const char* buffer = piece.buffer();
  piece.Pin();
  EXPECT_EQ(buffer, piece.buffer());
}

TEST_F(SpdyPinnableBufferPieceTest, Swap) {
  SpdyPrefixedBufferReader reader = Build("foobar", "");
  SpdyPinnableBufferPiece piece1, piece2;
  EXPECT_TRUE(reader.ReadN(4, &piece1));
  EXPECT_TRUE(reader.ReadN(2, &piece2));

  piece1.Pin();

  EXPECT_EQ(SpdyStringPiece("foob"), SpdyStringPiece(piece1));
  EXPECT_TRUE(piece1.IsPinned());
  EXPECT_EQ(SpdyStringPiece("ar"), SpdyStringPiece(piece2));
  EXPECT_FALSE(piece2.IsPinned());

  piece1.Swap(&piece2);

  EXPECT_EQ(SpdyStringPiece("ar"), SpdyStringPiece(piece1));
  EXPECT_FALSE(piece1.IsPinned());
  EXPECT_EQ(SpdyStringPiece("foob"), SpdyStringPiece(piece2));
  EXPECT_TRUE(piece2.IsPinned());

  SpdyPinnableBufferPiece empty;
  piece2.Swap(&empty);

  EXPECT_EQ(SpdyStringPiece(""), SpdyStringPiece(piece2));
  EXPECT_FALSE(piece2.IsPinned());
}

}  // namespace test

}  // namespace gfe_spdy
