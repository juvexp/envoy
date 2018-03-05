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

#ifndef GFE_SPDY_CORE_ZERO_COPY_OUTPUT_BUFFER_H_
#define GFE_SPDY_CORE_ZERO_COPY_OUTPUT_BUFFER_H_

#include <cstdint>

namespace gfe_spdy {

class ZeroCopyOutputBuffer {
 public:
  virtual ~ZeroCopyOutputBuffer() {}

  // Returns the next available segment of memory to write. Will always return
  // the same segment until AdvanceWritePtr is called.
  virtual void Next(char** data, int* size) = 0;

  // After writing to a buffer returned from Next(), the caller should call
  // this method to indicate how many bytes were written.
  virtual void AdvanceWritePtr(int64_t count) = 0;

  // Returns the available capacity of the buffer.
  virtual int64_t BytesFree() const = 0;
};

}  // namespace gfe_spdy

#endif  // GFE_SPDY_CORE_ZERO_COPY_OUTPUT_BUFFER_H_
