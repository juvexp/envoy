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

#ifndef GFE_QUIC_CORE_QUIC_HEADER_LIST_H_
#define GFE_QUIC_CORE_QUIC_HEADER_LIST_H_

#include <algorithm>
#include <functional>
#include <utility>

#include "common/quic/platform/api/quic_bug_tracker.h"
#include "common/quic/platform/api/quic_containers.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"
#include "common/quic/spdy_utils/core/spdy_header_block.h"
#include "common/quic/spdy_utils/core/spdy_headers_handler_interface.h"

namespace gfe_quic {

// A simple class that accumulates header pairs
class QUIC_EXPORT_PRIVATE QuicHeaderList
    : public gfe_spdy::SpdyHeadersHandlerInterface {
 public:
  typedef QuicDeque<std::pair<QuicString, QuicString>> ListType;
  typedef ListType::const_iterator const_iterator;

  QuicHeaderList();
  QuicHeaderList(QuicHeaderList&& other);
  QuicHeaderList(const QuicHeaderList& other);
  QuicHeaderList& operator=(QuicHeaderList&& other);
  QuicHeaderList& operator=(const QuicHeaderList& other);
  ~QuicHeaderList() override;

  // From SpdyHeadersHandlerInteface.
  void OnHeaderBlockStart() override;
  void OnHeader(QuicStringPiece name, QuicStringPiece value) override;
  void OnHeaderBlockEnd(size_t uncompressed_header_bytes,
                        size_t compressed_header_bytes) override;

  void Clear();

  const_iterator begin() const { return header_list_.begin(); }
  const_iterator end() const { return header_list_.end(); }

  bool empty() const { return header_list_.empty(); }
  size_t uncompressed_header_bytes() const {
    return uncompressed_header_bytes_;
  }
  size_t compressed_header_bytes() const { return compressed_header_bytes_; }

  void set_max_header_list_size(size_t max_header_list_size) {
    max_header_list_size_ = max_header_list_size;
  }

  size_t max_header_list_size() const { return max_header_list_size_; }

  QuicString DebugString() const;

 private:
  QuicDeque<std::pair<QuicString, QuicString>> header_list_;

  // The limit on the size of the header list (defined by spec as name + value +
  // overhead for each header field). Headers over this limit will not be
  // buffered, and the list will be cleared upon OnHeaderBlockEnd.
  size_t max_header_list_size_;

  // Defined per the spec as the size of all header fields with an additional
  // overhead for each field.
  size_t current_header_list_size_;

  // TODO Are these fields necessary?
  size_t uncompressed_header_bytes_;
  size_t compressed_header_bytes_;
};

inline bool operator==(const QuicHeaderList& l1, const QuicHeaderList& l2) {
  auto pred = [](const std::pair<QuicString, QuicString>& p1,
                 const std::pair<QuicString, QuicString>& p2) {
    return p1.first == p2.first && p1.second == p2.second;
  };
  return std::equal(l1.begin(), l1.end(), l2.begin(), pred);
}

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_HEADER_LIST_H_
