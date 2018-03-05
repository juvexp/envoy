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

#include "common/quic/spdy_utils/core/spdy_prefixed_buffer_reader.h"

#include <new>

#include "base/logging.h"

namespace gfe_spdy {

SpdyPrefixedBufferReader::SpdyPrefixedBufferReader(const char* prefix,
                                                   size_t prefix_length,
                                                   const char* suffix,
                                                   size_t suffix_length)
    : prefix_(prefix),
      suffix_(suffix),
      prefix_length_(prefix_length),
      suffix_length_(suffix_length) {}

size_t SpdyPrefixedBufferReader::Available() {
  return prefix_length_ + suffix_length_;
}

bool SpdyPrefixedBufferReader::ReadN(size_t count, char* out) {
  if (Available() < count) {
    return false;
  }

  if (prefix_length_ >= count) {
    // Read is fully satisfied by the prefix.
    std::copy(prefix_, prefix_ + count, out);
    prefix_ += count;
    prefix_length_ -= count;
    return true;
  } else if (prefix_length_ != 0) {
    // Read is partially satisfied by the prefix.
    out = std::copy(prefix_, prefix_ + prefix_length_, out);
    count -= prefix_length_;
    prefix_length_ = 0;
    // Fallthrough to suffix read.
  }
  DCHECK(suffix_length_ >= count);
  // Read is satisfied by the suffix.
  std::copy(suffix_, suffix_ + count, out);
  suffix_ += count;
  suffix_length_ -= count;
  return true;
}

bool SpdyPrefixedBufferReader::ReadN(size_t count,
                                     SpdyPinnableBufferPiece* out) {
  if (Available() < count) {
    return false;
  }

  out->storage_.reset();
  out->length_ = count;

  if (prefix_length_ >= count) {
    // Read is fully satisfied by the prefix.
    out->buffer_ = prefix_;
    prefix_ += count;
    prefix_length_ -= count;
    return true;
  } else if (prefix_length_ != 0) {
    // Read is only partially satisfied by the prefix. We need to allocate
    // contiguous storage as the read spans the prefix & suffix.
    out->storage_.reset(new char[count]);
    out->buffer_ = out->storage_.get();
    ReadN(count, out->storage_.get());
    return true;
  } else {
    DCHECK(suffix_length_ >= count);
    // Read is fully satisfied by the suffix.
    out->buffer_ = suffix_;
    suffix_ += count;
    suffix_length_ -= count;
    return true;
  }
}

}  // namespace gfe_spdy
