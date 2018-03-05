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

#include "common/quic/platform/api/quic_mem_slice.h"

#include "common/quic/core/quic_simple_buffer_allocator.h"
#include "test/common/quic/platform/api/quic_test.h"

namespace gfe_quic {
namespace test {
namespace {

class QuicMemSliceTest : public QuicTest {
 public:
  QuicMemSliceTest() {
    size_t length = 1024;
    slice_ = QuicMemSlice(&allocator_, length);
    orig_data_ = slice_.data();
    orig_length_ = slice_.length();
  }

  SimpleBufferAllocator allocator_;
  QuicMemSlice slice_;
  const char* orig_data_;
  size_t orig_length_;
};

TEST_F(QuicMemSliceTest, MoveConstruct) {
  QuicMemSlice moved(std::move(slice_));
  EXPECT_EQ(moved.data(), orig_data_);
  EXPECT_EQ(moved.length(), orig_length_);
  EXPECT_EQ(nullptr, slice_.data());
  EXPECT_EQ(0, slice_.length());
  EXPECT_TRUE(slice_.empty());
}

TEST_F(QuicMemSliceTest, MoveAssign) {
  QuicMemSlice moved;
  moved = std::move(slice_);
  EXPECT_EQ(moved.data(), orig_data_);
  EXPECT_EQ(moved.length(), orig_length_);
  EXPECT_EQ(nullptr, slice_.data());
  EXPECT_EQ(0, slice_.length());
  EXPECT_TRUE(slice_.empty());
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
