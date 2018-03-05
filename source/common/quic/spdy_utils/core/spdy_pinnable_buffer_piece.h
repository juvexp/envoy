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

#ifndef GFE_SPDY_CORE_SPDY_PINNABLE_BUFFER_PIECE_H_
#define GFE_SPDY_CORE_SPDY_PINNABLE_BUFFER_PIECE_H_

#include <memory>

#include "common/quic/spdy_utils/platform/api/spdy_export.h"
#include "common/quic/spdy_utils/platform/api/spdy_string_piece.h"

namespace gfe_spdy {

class SpdyPrefixedBufferReader;

// Helper class of SpdyPrefixedBufferReader.
// Represents a piece of consumed buffer which may (or may not) own its
// underlying storage. Users may "pin" the buffer at a later time to ensure
// a SpdyPinnableBufferPiece owns and retains storage of the buffer.
struct SPDY_EXPORT_PRIVATE SpdyPinnableBufferPiece {
 public:
  SpdyPinnableBufferPiece();
  ~SpdyPinnableBufferPiece();

  const char* buffer() const { return buffer_; }

  explicit operator SpdyStringPiece() const {
    return SpdyStringPiece(buffer_, length_);
  }

  // Allocates and copies the buffer to internal storage.
  void Pin();

  bool IsPinned() const { return storage_ != nullptr; }

  // Swaps buffers, including internal storage, with |other|.
  void Swap(SpdyPinnableBufferPiece* other);

 private:
  friend class SpdyPrefixedBufferReader;

  const char* buffer_;
  size_t length_;
  // Null iff |buffer_| isn't pinned.
  std::unique_ptr<char[]> storage_;
};

}  // namespace gfe_spdy

#endif  // GFE_SPDY_CORE_SPDY_PINNABLE_BUFFER_PIECE_H_
