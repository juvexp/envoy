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

#include "common/quic/spdy_utils/core/spdy_frame_builder.h"

#include <memory>

#include "common/quic/spdy_utils/core/array_output_buffer.h"
#include "common/quic/spdy_utils/core/spdy_framer.h"
#include "common/quic/spdy_utils/core/spdy_protocol.h"
#include "gtest/gtest.h"

namespace gfe_spdy {

namespace {

const int64_t kSize = 64 * 1024;
char output_buffer[kSize] = "";

}  // namespace

// Verifies that SpdyFrameBuilder::GetWritableBuffer() can be used to build a
// SpdySerializedFrame.
TEST(SpdyFrameBuilderTest, GetWritableBuffer) {
  const size_t kBuilderSize = 10;
  SpdyFrameBuilder builder(kBuilderSize);
  char* writable_buffer = builder.GetWritableBuffer(kBuilderSize);
  memset(writable_buffer, ~1, kBuilderSize);
  EXPECT_TRUE(builder.Seek(kBuilderSize));
  SpdySerializedFrame frame(builder.take());
  char expected[kBuilderSize];
  memset(expected, ~1, kBuilderSize);
  EXPECT_EQ(SpdyStringPiece(expected, kBuilderSize),
            SpdyStringPiece(frame.data(), kBuilderSize));
}

// Verifies that SpdyFrameBuilder::GetWritableBuffer() can be used to build a
// SpdySerializedFrame to the output buffer.
TEST(SpdyFrameBuilderTest, GetWritableOutput) {
  ArrayOutputBuffer output(output_buffer, kSize);
  const size_t kBuilderSize = 10;
  SpdyFrameBuilder builder(kBuilderSize, &output);
  size_t actual_size = 0;
  char* writable_buffer = builder.GetWritableOutput(kBuilderSize, &actual_size);
  memset(writable_buffer, ~1, kBuilderSize);
  EXPECT_TRUE(builder.Seek(kBuilderSize));
  SpdySerializedFrame frame(output.Begin(), kBuilderSize, false);
  char expected[kBuilderSize];
  memset(expected, ~1, kBuilderSize);
  EXPECT_EQ(SpdyStringPiece(expected, kBuilderSize),
            SpdyStringPiece(frame.data(), kBuilderSize));
}

// Verifies the case that the buffer's capacity is too small.
TEST(SpdyFrameBuilderTest, GetWritableOutputNegative) {
  size_t small_cap = 1;
  ArrayOutputBuffer output(output_buffer, small_cap);
  const size_t kBuilderSize = 10;
  SpdyFrameBuilder builder(kBuilderSize, &output);
  size_t actual_size = 0;
  char* writable_buffer = builder.GetWritableOutput(kBuilderSize, &actual_size);
  builder.GetWritableOutput(kBuilderSize, &actual_size);
  EXPECT_EQ(0, actual_size);
  EXPECT_EQ(nullptr, writable_buffer);
}

}  // namespace gfe_spdy
