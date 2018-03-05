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

#ifndef GFE_SPDY_CORE_ARRAY_OUTPUT_BUFFER_H_
#define GFE_SPDY_CORE_ARRAY_OUTPUT_BUFFER_H_

#include <cstddef>

#include "common/quic/spdy_utils/core/zero_copy_output_buffer.h"

namespace gfe_spdy {

class ArrayOutputBuffer : public ZeroCopyOutputBuffer {
 public:
  // |buffer| is pointed to the output to write to, and |size| is the capacity
  // of the output.
  ArrayOutputBuffer(char* buffer, int64_t size)
      : current_(buffer), begin_(buffer), capacity_(size) {}
  ~ArrayOutputBuffer() override {}

  ArrayOutputBuffer(const ArrayOutputBuffer&) = delete;
  ArrayOutputBuffer& operator=(const ArrayOutputBuffer&) = delete;

  void Next(char** data, int* size) override;
  void AdvanceWritePtr(int64_t count) override;
  int64_t BytesFree() const override;

  size_t Size() const { return current_ - begin_; }
  char* Begin() const { return begin_; }

  // Resets the buffer to its original state.
  void Reset() {
    capacity_ += Size();
    current_ = begin_;
  }

 private:
  char* current_ = nullptr;
  char* begin_ = nullptr;
  int64_t capacity_ = 0;
};

}  // namespace gfe_spdy

#endif  // GFE_SPDY_CORE_ARRAY_OUTPUT_BUFFER_H_
