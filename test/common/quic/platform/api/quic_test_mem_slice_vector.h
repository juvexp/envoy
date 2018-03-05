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

#ifndef GFE_QUIC_PLATFORM_API_QUIC_TEST_MEM_SLICE_VECTOR_H_
#define GFE_QUIC_PLATFORM_API_QUIC_TEST_MEM_SLICE_VECTOR_H_

#include <utility>

#include "common/quic/platform/api/quic_mem_slice_span.h"
#include "common/quic/platform/impl/quic_test_mem_slice_vector_impl.h"

namespace gfe_quic {
namespace test {
// QuicTestMemSliceVector is a test only class which creates a vector of
// platform-specific data structure (used as QuicMemSlice) from an array of data
// buffers. QuicTestMemSliceVector does not own the underlying data buffer.
// Tests using QuicTestMemSliceVector need to make sure the actual data buffers
// outlive QuicTestMemSliceVector, and QuicTestMemSliceVector outlive the
// returned QuicMemSliceSpan.
class QuicTestMemSliceVector {
 public:
  explicit QuicTestMemSliceVector(std::vector<std::pair<char*, size_t>> buffers)
      : impl_(std::move(buffers)) {}

  QuicMemSliceSpan span() { return QuicMemSliceSpan(impl_.span()); }

 private:
  QuicTestMemSliceVectorImpl impl_;
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_PLATFORM_API_QUIC_TEST_MEM_SLICE_VECTOR_H_
