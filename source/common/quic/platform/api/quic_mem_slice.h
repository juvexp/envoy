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

#ifndef GFE_QUIC_PLATFORM_API_QUIC_MEM_SLICE_H_
#define GFE_QUIC_PLATFORM_API_QUIC_MEM_SLICE_H_

#include "common/quic/platform/impl/quic_mem_slice_impl.h"

namespace gfe_quic {

// QuicMemSlice is an internally reference counted data buffer used as the
// source buffers for write operations. QuicMemSlice implicitly maintains a
// reference count and will free the underlying data buffer when the reference
// count reaches zero.
class QuicMemSlice {
 public:
  // Constructs a empty QuicMemSlice with no underlying data and 0 reference
  // count.
  QuicMemSlice() = default;
  // Let |allocator| allocate a data buffer of |length|, then construct
  // QuicMemSlice with reference count 1 from the allocated data buffer.
  // Once all of the references to the allocated data buffer are released,
  // |allocator| is responsible to free the memory. |allocator| must
  // not be null, and |length| must not be 0. To construct an empty
  // QuicMemSlice, use the zero-argument constructor instead.
  QuicMemSlice(QuicBufferAllocator* allocator, size_t length)
      : impl_(allocator, length) {}

  // Constructs QuicMemSlice from |impl|. It takes the reference away from
  // |impl|.
  explicit QuicMemSlice(QuicMemSliceImpl impl) : impl_(std::move(impl)) {}

  QuicMemSlice(const QuicMemSlice& other) = delete;
  QuicMemSlice& operator=(const QuicMemSlice& other) = delete;

  // Move constructors. |other| will not hold a reference to the data buffer
  // after this call completes.
  QuicMemSlice(QuicMemSlice&& other) = default;
  QuicMemSlice& operator=(QuicMemSlice&& other) = default;

  ~QuicMemSlice() = default;

  // Release the underlying reference. Further access the memory will result in
  // undefined behavior.
  void Reset() { impl_.Reset(); }

  // Returns a const char pointer to underlying data buffer.
  const char* data() const { return impl_.data(); }
  // Returns the length of underlying data buffer.
  size_t length() const { return impl_.length(); }

  bool empty() const { return impl_.empty(); }

 private:
  QuicMemSliceImpl impl_;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_PLATFORM_API_QUIC_MEM_SLICE_H_
