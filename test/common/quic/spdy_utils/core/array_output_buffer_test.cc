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

#include "common/quic/spdy_utils/core/array_output_buffer.h"

#include "testing/base/public/gmock.h"
#include "testing/base/public/gunit.h"

namespace gfe_spdy {
namespace test {

// This test verifies that ArrayOutputBuffer is initialized properly.
TEST(ArrayOutputBufferTest, InitializedFromArray) {
  char array[100];
  ArrayOutputBuffer buffer(array, sizeof(array));
  EXPECT_EQ(sizeof(array), buffer.BytesFree());
  EXPECT_EQ(0, buffer.Size());
  EXPECT_EQ(array, buffer.Begin());
}

// This test verifies that Reset() causes an ArrayOutputBuffer's capacity and
// size to be reset to the initial state.
TEST(ArrayOutputBufferTest, WriteAndReset) {
  char array[100];
  ArrayOutputBuffer buffer(array, sizeof(array));

  // Let's write some bytes.
  char* dst;
  int size;
  buffer.Next(&dst, &size);
  ASSERT_GT(size, 1);
  ASSERT_NE(nullptr, dst);
  const int64_t written = size / 2;
  memset(dst, 'x', written);
  buffer.AdvanceWritePtr(written);

  // The buffer should be partially used.
  EXPECT_EQ(size - written, buffer.BytesFree());
  EXPECT_EQ(written, buffer.Size());

  buffer.Reset();

  // After a reset, the buffer should regain its full capacity.
  EXPECT_EQ(sizeof(array), buffer.BytesFree());
  EXPECT_EQ(0, buffer.Size());
}

}  // namespace test
}  // namespace gfe_spdy
