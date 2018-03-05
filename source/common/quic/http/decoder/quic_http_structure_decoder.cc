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

#include "common/quic/http/decoder/quic_http_structure_decoder.h"

#include <algorithm>

#include "common/quic/platform/api/quic_bug_tracker.h"

namespace gfe_quic {

// Below we have some defensive coding: if we somehow run off the end, don't
// overwrite lots of memory. Note that most of this decoder is not defensive
// against bugs in the decoder, only against malicious encoders, but since
// we're copying memory into a buffer here, let's make sure we don't allow a
// small mistake to grow larger. The decoder will get stuck if we hit the
// QUIC_BUG conditions, but shouldn't corrupt memory.

uint32_t QuicHttpStructureDecoder::IncompleteStart(QuicHttpDecodeBuffer* db,
                                                   uint32_t target_size) {
  if (target_size > sizeof buffer_) {
    QUIC_BUG << "target_size too large for buffer: " << target_size;
    return 0;
  }
  const uint32_t num_to_copy = db->MinLengthRemaining(target_size);
  memcpy(buffer_, db->cursor(), num_to_copy);
  offset_ = num_to_copy;
  db->AdvanceCursor(num_to_copy);
  return num_to_copy;
}

QuicHttpDecodeStatus QuicHttpStructureDecoder::IncompleteStart(
    QuicHttpDecodeBuffer* db,
    uint32_t* remaining_payload,
    uint32_t target_size) {
  DVLOG(1) << "IncompleteStart@" << this
           << ": *remaining_payload=" << *remaining_payload
           << "; target_size=" << target_size
           << "; db->Remaining=" << db->Remaining();
  *remaining_payload -=
      IncompleteStart(db, std::min(target_size, *remaining_payload));
  if (*remaining_payload > 0 && db->Empty()) {
    return QuicHttpDecodeStatus::kDecodeInProgress;
  }
  DVLOG(1) << "IncompleteStart: kDecodeError";
  return QuicHttpDecodeStatus::kDecodeError;
}

bool QuicHttpStructureDecoder::ResumeFillingBuffer(QuicHttpDecodeBuffer* db,
                                                   uint32_t target_size) {
  DVLOG(2) << "ResumeFillingBuffer@" << this << ": target_size=" << target_size
           << "; offset_=" << offset_ << "; db->Remaining=" << db->Remaining();
  if (target_size < offset_) {
    QUIC_BUG << "Already filled buffer_! target_size=" << target_size
             << "    offset_=" << offset_;
    return false;
  }
  const uint32_t needed = target_size - offset_;
  const uint32_t num_to_copy = db->MinLengthRemaining(needed);
  DVLOG(2) << "ResumeFillingBuffer num_to_copy=" << num_to_copy;
  memcpy(&buffer_[offset_], db->cursor(), num_to_copy);
  db->AdvanceCursor(num_to_copy);
  offset_ += num_to_copy;
  return needed == num_to_copy;
}

bool QuicHttpStructureDecoder::ResumeFillingBuffer(QuicHttpDecodeBuffer* db,
                                                   uint32_t* remaining_payload,
                                                   uint32_t target_size) {
  DVLOG(2) << "ResumeFillingBuffer@" << this << ": target_size=" << target_size
           << "; offset_=" << offset_
           << "; *remaining_payload=" << *remaining_payload
           << "; db->Remaining=" << db->Remaining();
  if (target_size < offset_) {
    QUIC_BUG << "Already filled buffer_! target_size=" << target_size
             << "    offset_=" << offset_;
    return false;
  }
  const uint32_t needed = target_size - offset_;
  const uint32_t num_to_copy =
      db->MinLengthRemaining(std::min(needed, *remaining_payload));
  DVLOG(2) << "ResumeFillingBuffer num_to_copy=" << num_to_copy;
  memcpy(&buffer_[offset_], db->cursor(), num_to_copy);
  db->AdvanceCursor(num_to_copy);
  offset_ += num_to_copy;
  *remaining_payload -= num_to_copy;
  return needed == num_to_copy;
}

}  // namespace gfe_quic
