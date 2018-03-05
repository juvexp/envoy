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

#include "common/quic/spdy_utils/core/hpack/hpack_static_table.h"

#include "base/logging.h"
#include "common/quic/spdy_utils/core/hpack/hpack_constants.h"
#include "common/quic/spdy_utils/core/hpack/hpack_entry.h"
#include "common/quic/spdy_utils/platform/api/spdy_string_piece.h"

namespace gfe_spdy {

HpackStaticTable::HpackStaticTable() = default;

HpackStaticTable::~HpackStaticTable() = default;

void HpackStaticTable::Initialize(const HpackStaticEntry* static_entry_table,
                                  size_t static_entry_count) {
  CHECK(!IsInitialized());

  int total_insertions = 0;
  for (const HpackStaticEntry* it = static_entry_table;
       it != static_entry_table + static_entry_count; ++it) {
    static_entries_.push_back(
        HpackEntry(SpdyStringPiece(it->name, it->name_len),
                   SpdyStringPiece(it->value, it->value_len),
                   true,  // is_static
                   total_insertions));
    HpackEntry* entry = &static_entries_.back();
    CHECK(static_index_.insert(entry).second);
    // Multiple static entries may have the same name, so inserts may fail.
    static_name_index_.insert(std::make_pair(entry->name(), entry));

    ++total_insertions;
  }
}

bool HpackStaticTable::IsInitialized() const {
  return !static_entries_.empty();
}

}  // namespace gfe_spdy
