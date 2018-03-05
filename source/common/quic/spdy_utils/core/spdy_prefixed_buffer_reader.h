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

#ifndef GFE_SPDY_CORE_SPDY_PREFIXED_BUFFER_READER_H_
#define GFE_SPDY_CORE_SPDY_PREFIXED_BUFFER_READER_H_

#include "common/quic/spdy_utils/core/spdy_pinnable_buffer_piece.h"
#include "common/quic/spdy_utils/platform/api/spdy_export.h"

namespace gfe_spdy {

// Reader class which simplifies reading contiguously from
// from a disjoint buffer prefix & suffix.
class SPDY_EXPORT_PRIVATE SpdyPrefixedBufferReader {
 public:
  SpdyPrefixedBufferReader(const char* prefix,
                           size_t prefix_length,
                           const char* suffix,
                           size_t suffix_length);

  // Returns number of bytes available to be read.
  size_t Available();

  // Reads |count| bytes, copying into |*out|. Returns true on success,
  // false if not enough bytes were available.
  bool ReadN(size_t count, char* out);

  // Reads |count| bytes, returned in |*out|. Returns true on success,
  // false if not enough bytes were available.
  bool ReadN(size_t count, SpdyPinnableBufferPiece* out);

 private:
  const char* prefix_;
  const char* suffix_;

  size_t prefix_length_;
  size_t suffix_length_;
};

}  // namespace gfe_spdy

#endif  // GFE_SPDY_CORE_SPDY_PREFIXED_BUFFER_READER_H_
