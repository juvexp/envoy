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

#include "common/quic/spdy_utils/platform/api/spdy_mem_slice.h"

#include <utility>

#include "testing/base/public/gunit.h"

namespace gfe_spdy {
namespace test {
namespace {

class SpdyMemSliceTest : public ::testing::Test {
 public:
  SpdyMemSliceTest() {
    slice_ = SpdyMemSlice(1024);
    orig_data_ = slice_.data();
    orig_length_ = slice_.length();
  }

  SpdyMemSlice slice_;
  const char* orig_data_;
  size_t orig_length_;
};

TEST_F(SpdyMemSliceTest, MoveConstruct) {
  SpdyMemSlice moved(std::move(slice_));
  EXPECT_EQ(moved.data(), orig_data_);
  EXPECT_EQ(moved.length(), orig_length_);
  EXPECT_EQ(nullptr, slice_.data());
  EXPECT_EQ(0, slice_.length());
}

TEST_F(SpdyMemSliceTest, MoveAssign) {
  SpdyMemSlice moved;
  moved = std::move(slice_);
  EXPECT_EQ(moved.data(), orig_data_);
  EXPECT_EQ(moved.length(), orig_length_);
  EXPECT_EQ(nullptr, slice_.data());
  EXPECT_EQ(0, slice_.length());
}

}  // namespace
}  // namespace test
}  // namespace gfe_spdy
