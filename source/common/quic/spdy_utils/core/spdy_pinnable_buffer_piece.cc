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

#include "common/quic/spdy_utils/core/spdy_pinnable_buffer_piece.h"

namespace gfe_spdy {

SpdyPinnableBufferPiece::SpdyPinnableBufferPiece()
    : buffer_(nullptr), length_(0) {}

SpdyPinnableBufferPiece::~SpdyPinnableBufferPiece() = default;

void SpdyPinnableBufferPiece::Pin() {
  if (!storage_ && buffer_ != nullptr && length_ != 0) {
    storage_.reset(new char[length_]);
    std::copy(buffer_, buffer_ + length_, storage_.get());
    buffer_ = storage_.get();
  }
}

void SpdyPinnableBufferPiece::Swap(SpdyPinnableBufferPiece* other) {
  size_t length = length_;
  length_ = other->length_;
  other->length_ = length;

  const char* buffer = buffer_;
  buffer_ = other->buffer_;
  other->buffer_ = buffer;

  storage_.swap(other->storage_);
}

}  // namespace gfe_spdy
