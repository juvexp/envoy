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

#ifndef GFE_SPDY_PLATFORM_API_SPDY_MEM_SLICE_H_
#define GFE_SPDY_PLATFORM_API_SPDY_MEM_SLICE_H_

#include <utility>

#include "common/quic/spdy_utils/platform/api/spdy_export.h"
#include "common/quic/spdy_utils/platform/impl/spdy_mem_slice_impl.h"

namespace gfe_spdy {

// SpdyMemSlice is an internally reference counted data buffer used as the
// source buffers for write operations. SpdyMemSlice implicitly maintains a
// reference count and will free the underlying data buffer when the reference
// count reaches zero.
class SPDY_EXPORT_PRIVATE SpdyMemSlice {
 public:
  // Constructs an empty SpdyMemSlice with no underlying data and 0 reference
  // count.
  SpdyMemSlice() = default;

  // Constructs a SpdyMemSlice with reference count 1 to a newly allocated data
  // buffer of |length| bytes.
  explicit SpdyMemSlice(size_t length) : impl_(length) {}

  // Constructs a SpdyMemSlice from |impl|. It takes the reference away from
  // |impl|.
  explicit SpdyMemSlice(SpdyMemSliceImpl impl) : impl_(std::move(impl)) {}

  SpdyMemSlice(const SpdyMemSlice& other) = delete;
  SpdyMemSlice& operator=(const SpdyMemSlice& other) = delete;

  // Move constructors. |other| will not hold a reference to the data buffer
  // after this call completes.
  SpdyMemSlice(SpdyMemSlice&& other) = default;
  SpdyMemSlice& operator=(SpdyMemSlice&& other) = default;

  ~SpdyMemSlice() = default;

  // Returns a char pointer to underlying data buffer.
  const char* data() const { return impl_.data(); }
  // Returns the length of underlying data buffer.
  size_t length() const { return impl_.length(); }

 private:
  SpdyMemSliceImpl impl_;
};

}  // namespace gfe_spdy

#endif  // GFE_SPDY_PLATFORM_API_SPDY_MEM_SLICE_H_
