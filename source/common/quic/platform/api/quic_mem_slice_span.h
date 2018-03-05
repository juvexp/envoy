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

#ifndef GFE_QUIC_PLATFORM_API_QUIC_MEM_SLICE_SPAN_H_
#define GFE_QUIC_PLATFORM_API_QUIC_MEM_SLICE_SPAN_H_

#include "common/quic/platform/impl/quic_mem_slice_span_impl.h"

namespace gfe_quic {

// QuicMemSliceSpan is effectively wrapper around an array of data structures
// used as QuicMemSlice. So it could implemented with:
// QuicMemSlice* slices_;
// size_t num_slices_;
// But for efficiency reasons, the actual implementation is an array of
// platform-specific objects. This could avoid the translation from
// platform-specific object to QuicMemSlice.
// QuicMemSliceSpan does not own the underling data buffers.
class QuicMemSliceSpan {
 public:
  explicit QuicMemSliceSpan(QuicMemSliceSpanImpl impl) : impl_(impl) {}

  QuicMemSliceSpan(const QuicMemSliceSpan& other) = default;
  QuicMemSliceSpan& operator=(const QuicMemSliceSpan& other) = default;
  QuicMemSliceSpan(QuicMemSliceSpan&& other) = default;
  QuicMemSliceSpan& operator=(QuicMemSliceSpan&& other) = default;

  ~QuicMemSliceSpan() = default;

  // Save data buffers to |send_buffer| and returns the amount of saved data.
  // |send_buffer| will hold a reference to all data buffer.
  QuicByteCount SaveMemSlicesInSendBuffer(QuicStreamSendBuffer* send_buffer) {
    return impl_.SaveMemSlicesInSendBuffer(send_buffer);
  }

  bool empty() const { return impl_.empty(); }

 private:
  QuicMemSliceSpanImpl impl_;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_PLATFORM_API_QUIC_MEM_SLICE_SPAN_H_
