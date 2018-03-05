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

#include "common/quic/core/quic_simple_buffer_allocator.h"

#include "common/quic/core/quic_packets.h"
#include "test/common/quic/platform/api/quic_test.h"

namespace gfe_quic {
namespace {

class SimpleBufferAllocatorTest : public QuicTest {};

TEST_F(SimpleBufferAllocatorTest, NewDelete) {
  SimpleBufferAllocator alloc;
  char* buf = alloc.New(4);
  EXPECT_NE(nullptr, buf);
  alloc.Delete(buf);
}

TEST_F(SimpleBufferAllocatorTest, DeleteNull) {
  SimpleBufferAllocator alloc;
  alloc.Delete(nullptr);
}

}  // namespace
}  // namespace gfe_quic
