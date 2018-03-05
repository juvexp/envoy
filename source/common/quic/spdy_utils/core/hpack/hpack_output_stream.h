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

#ifndef GFE_SPDY_CORE_HPACK_HPACK_OUTPUT_STREAM_H_
#define GFE_SPDY_CORE_HPACK_HPACK_OUTPUT_STREAM_H_

#include <cstdint>
#include <map>

#include "base/macros.h"
#include "common/quic/spdy_utils/core/hpack/hpack_constants.h"
#include "common/quic/spdy_utils/platform/api/spdy_export.h"
#include "common/quic/spdy_utils/platform/api/spdy_string.h"
#include "common/quic/spdy_utils/platform/api/spdy_string_piece.h"

// All section references below are to
// http://tools.ietf.org/html/draft-ietf-httpbis-header-compression-08

namespace gfe_spdy {

// An HpackOutputStream handles all the low-level details of encoding
// header fields.
class SPDY_EXPORT_PRIVATE HpackOutputStream {
 public:
  HpackOutputStream();
  ~HpackOutputStream();

  // Appends the lower |bit_size| bits of |bits| to the internal buffer.
  //
  // |bit_size| must be > 0 and <= 8. |bits| must not have any bits
  // set other than the lower |bit_size| bits.
  void AppendBits(uint8_t bits, size_t bit_size);

  // Simply forwards to AppendBits(prefix.bits, prefix.bit-size).
  void AppendPrefix(HpackPrefix prefix);

  // Directly appends |buffer|.
  void AppendBytes(SpdyStringPiece buffer);

  // Appends the given integer using the representation described in
  // 6.1. If the internal buffer ends on a byte boundary, the prefix
  // length N is taken to be 8; otherwise, it is taken to be the
  // number of bits to the next byte boundary.
  //
  // It is guaranteed that the internal buffer will end on a byte
  // boundary after this function is called.
  void AppendUint32(uint32_t I);

  // Swaps the internal buffer with |output|, then resets state.
  void TakeString(SpdyString* output);

  // Gives up to |max_size| bytes of the internal buffer to |output|. Resets
  // internal state with the overflow.
  void BoundedTakeString(size_t max_size, SpdyString* output);

  // Size in bytes of stream's internal buffer.
  size_t size() const { return buffer_.size(); }

 private:
  // The internal bit buffer.
  SpdyString buffer_;

  // If 0, the buffer ends on a byte boundary. If non-zero, the buffer
  // ends on the nth most significant bit. Guaranteed to be < 8.
  size_t bit_offset_;

  DISALLOW_COPY_AND_ASSIGN(HpackOutputStream);
};

}  // namespace gfe_spdy

#endif  // GFE_SPDY_CORE_HPACK_HPACK_OUTPUT_STREAM_H_
