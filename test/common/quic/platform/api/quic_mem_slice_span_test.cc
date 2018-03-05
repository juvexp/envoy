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

#include "common/quic/platform/api/quic_mem_slice_span.h"

#include "common/quic/core/quic_simple_buffer_allocator.h"
#include "common/quic/core/quic_stream_send_buffer.h"
#include "common/quic/platform/api/quic_flags.h"
#include "test/common/quic/platform/api/quic_test.h"
#include "test/common/quic/platform/api/quic_test_mem_slice_vector.h"

namespace gfe_quic {
namespace test {
namespace {

class QuicMemSliceSpanImplTest : public QuicTest {
 public:
  QuicMemSliceSpanImplTest() {
    for (size_t i = 0; i < 10; ++i) {
      buffers_.push_back(make_pair(data_, 1024));
    }
  }

  char data_[1024];
  std::vector<std::pair<char*, size_t>> buffers_;
};

TEST_F(QuicMemSliceSpanImplTest, SaveDataInSendBuffer) {
  SimpleBufferAllocator allocator;
  QuicStreamSendBuffer send_buffer(&allocator);
  QuicTestMemSliceVector vector(buffers_);

  EXPECT_EQ(10 * 1024u, vector.span().SaveMemSlicesInSendBuffer(&send_buffer));
  EXPECT_EQ(10u, send_buffer.size());
}

TEST_F(QuicMemSliceSpanImplTest, SaveEmptyMemSliceInSendBuffer) {
  SimpleBufferAllocator allocator;
  QuicStreamSendBuffer send_buffer(&allocator);
  buffers_.push_back(std::make_pair(nullptr, 0));
  QuicTestMemSliceVector vector(buffers_);
  EXPECT_EQ(10 * 1024u, vector.span().SaveMemSlicesInSendBuffer(&send_buffer));
  // Verify the empty slice does not get saved.
  EXPECT_EQ(10u, send_buffer.size());
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
