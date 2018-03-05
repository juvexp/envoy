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

#include "common/quic/http/decoder/quic_http_decode_buffer.h"

namespace gfe_quic {

#ifndef NDEBUG
// These are part of validating during tests that there is at most one
// QuicHttpDecodeBufferSubset instance at a time for any DecodeBuffer instance.
void QuicHttpDecodeBuffer::set_subset_of_base(
    QuicHttpDecodeBuffer* base,
    const QuicHttpDecodeBufferSubset* subset) {
  DCHECK_EQ(this, subset);
  base->set_subset(subset);
}
void QuicHttpDecodeBuffer::clear_subset_of_base(
    QuicHttpDecodeBuffer* base,
    const QuicHttpDecodeBufferSubset* subset) {
  DCHECK_EQ(this, subset);
  base->clear_subset(subset);
}
void QuicHttpDecodeBuffer::set_subset(
    const QuicHttpDecodeBufferSubset* subset) {
  DCHECK(subset != nullptr);
  DCHECK_EQ(subset_, nullptr) << "There is already a subset";
  subset_ = subset;
}
void QuicHttpDecodeBuffer::clear_subset(
    const QuicHttpDecodeBufferSubset* subset) {
  DCHECK(subset != nullptr);
  DCHECK_EQ(subset_, subset);
  subset_ = nullptr;
}
void QuicHttpDecodeBufferSubset::DebugSetup() {
  start_base_offset_ = base_buffer_->Offset();
  max_base_offset_ = start_base_offset_ + FullSize();
  DCHECK_LE(max_base_offset_, base_buffer_->FullSize());

  // Ensure that there is only one QuicHttpDecodeBufferSubset at a time for a
  // base.
  set_subset_of_base(base_buffer_, this);
}
void QuicHttpDecodeBufferSubset::DebugTearDown() {
  // Ensure that the base hasn't been modified.
  DCHECK_EQ(start_base_offset_, base_buffer_->Offset())
      << "The base buffer was modified";

  // Ensure that we haven't gone beyond the maximum allowed offset.
  size_t offset = Offset();
  DCHECK_LE(offset, FullSize());
  DCHECK_LE(start_base_offset_ + offset, max_base_offset_);
  DCHECK_LE(max_base_offset_, base_buffer_->FullSize());

  clear_subset_of_base(base_buffer_, this);
}
#endif

}  // namespace gfe_quic
