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

#ifndef GFE_QUIC_CORE_QUIC_BUFFER_ALLOCATOR_H_
#define GFE_QUIC_CORE_QUIC_BUFFER_ALLOCATOR_H_

#include <stddef.h>

#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

// Abstract base class for classes which allocate and delete buffers.
class QUIC_EXPORT_PRIVATE QuicBufferAllocator {
 public:
  virtual ~QuicBufferAllocator();

  // Returns or allocates a new buffer of |size|. Never returns null.
  virtual char* New(size_t size) = 0;

  // Returns or allocates a new buffer of |size| if |flag_enable| is true.
  // Otherwise, returns a buffer that is compatible with this class directly
  // with operator new. Never returns null.
  virtual char* New(size_t size, bool flag_enable) = 0;

  // Releases a buffer.
  virtual void Delete(char* buffer) = 0;

  // Marks the allocator as being idle. Serves as a hint to notify the allocator
  // that it should release any resources it's still holding on to.
  virtual void MarkAllocatorIdle() {}
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_BUFFER_ALLOCATOR_H_
