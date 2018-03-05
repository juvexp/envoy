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

#ifndef GFE_SPDY_CORE_HPACK_HPACK_STATIC_TABLE_H_
#define GFE_SPDY_CORE_HPACK_HPACK_STATIC_TABLE_H_

#include "common/quic/spdy_utils/core/hpack/hpack_header_table.h"
#include "common/quic/spdy_utils/platform/api/spdy_export.h"

namespace gfe_spdy {

struct HpackStaticEntry;

// HpackStaticTable provides |static_entries_| and |static_index_| for HPACK
// encoding and decoding contexts.  Once initialized, an instance is read only
// and may be accessed only through its const interface.  Such an instance may
// be shared accross multiple HPACK contexts.
class SPDY_EXPORT_PRIVATE HpackStaticTable {
 public:
  HpackStaticTable();
  ~HpackStaticTable();

  // Prepares HpackStaticTable by filling up static_entries_ and static_index_
  // from an array of struct HpackStaticEntry.  Must be called exactly once.
  void Initialize(const HpackStaticEntry* static_entry_table,
                  size_t static_entry_count);

  // Returns whether Initialize() has been called.
  bool IsInitialized() const;

  // Accessors.
  const HpackHeaderTable::EntryTable& GetStaticEntries() const {
    return static_entries_;
  }
  const HpackHeaderTable::UnorderedEntrySet& GetStaticIndex() const {
    return static_index_;
  }
  const HpackHeaderTable::NameToEntryMap& GetStaticNameIndex() const {
    return static_name_index_;
  }

 private:
  HpackHeaderTable::EntryTable static_entries_;
  HpackHeaderTable::UnorderedEntrySet static_index_;
  HpackHeaderTable::NameToEntryMap static_name_index_;
};

}  // namespace gfe_spdy

#endif  // GFE_SPDY_CORE_HPACK_HPACK_STATIC_TABLE_H_
