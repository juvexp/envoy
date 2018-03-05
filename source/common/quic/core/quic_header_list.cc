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

#include "common/quic/core/quic_header_list.h"

#include "common/quic/core/quic_packets.h"
#include "common/quic/platform/api/quic_flags.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/spdy_utils/core/spdy_protocol.h"

namespace gfe_quic {

QuicHeaderList::QuicHeaderList()
    : max_header_list_size_(kDefaultMaxUncompressedHeaderSize),
      current_header_list_size_(0),
      uncompressed_header_bytes_(0),
      compressed_header_bytes_(0) {}

QuicHeaderList::QuicHeaderList(QuicHeaderList&& other) = default;

QuicHeaderList::QuicHeaderList(const QuicHeaderList& other) = default;

QuicHeaderList& QuicHeaderList::operator=(const QuicHeaderList& other) =
    default;

QuicHeaderList& QuicHeaderList::operator=(QuicHeaderList&& other) = default;

QuicHeaderList::~QuicHeaderList() {}

void QuicHeaderList::OnHeaderBlockStart() {
  QUIC_BUG_IF(current_header_list_size_ != 0)
      << "OnHeaderBlockStart called more than once!";
}

void QuicHeaderList::OnHeader(QuicStringPiece name, QuicStringPiece value) {
  // Avoid infinite buffering of headers. No longer store headers
  // once the current headers are over the limit.
  if (current_header_list_size_ < max_header_list_size_) {
    current_header_list_size_ += name.size();
    current_header_list_size_ += value.size();
    current_header_list_size_ += gfe_spdy::kPerHeaderOverhead;
    header_list_.emplace_back(QuicString(name), QuicString(value));
  }
}

void QuicHeaderList::OnHeaderBlockEnd(size_t uncompressed_header_bytes,
                                      size_t compressed_header_bytes) {
  uncompressed_header_bytes_ = uncompressed_header_bytes;
  compressed_header_bytes_ = compressed_header_bytes;
  if (current_header_list_size_ > max_header_list_size_) {
    Clear();
  }
}

void QuicHeaderList::Clear() {
  header_list_.clear();
  current_header_list_size_ = 0;
  uncompressed_header_bytes_ = 0;
  compressed_header_bytes_ = 0;
}

QuicString QuicHeaderList::DebugString() const {
  QuicString s = "{ ";
  for (const auto& p : *this) {
    s.append(p.first + "=" + p.second + ", ");
  }
  s.append("}");
  return s;
}

}  // namespace gfe_quic
