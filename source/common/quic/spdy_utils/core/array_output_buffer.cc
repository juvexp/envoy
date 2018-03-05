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

#include "common/quic/spdy_utils/core/array_output_buffer.h"

namespace gfe_spdy {

void ArrayOutputBuffer::Next(char** data, int* size) {
  *data = current_;
  *size = capacity_ > 0 ? capacity_ : 0;
}

void ArrayOutputBuffer::AdvanceWritePtr(int64_t count) {
  current_ += count;
  capacity_ -= count;
}

int64_t ArrayOutputBuffer::BytesFree() const {
  return capacity_;
}

}  // namespace gfe_spdy
