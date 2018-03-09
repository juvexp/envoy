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

#include <set>

#include "common/quic/spdy_utils/core/hpack/hpack_constants.h"
#include "common/quic/spdy_utils/platform/api/spdy_string_piece.h"
#include "gtest/gtest.h"

namespace gfe_spdy {

namespace test {

namespace {

class HpackStaticTableTest : public ::testing::Test {
 protected:
  HpackStaticTableTest() : table_() {}

  HpackStaticTable table_;
};

// Check that an initialized instance has the right number of entries.
TEST_F(HpackStaticTableTest, Initialize) {
  EXPECT_FALSE(table_.IsInitialized());
  table_.Initialize(kHpackStaticTable, arraysize(kHpackStaticTable));
  EXPECT_TRUE(table_.IsInitialized());

  HpackHeaderTable::EntryTable static_entries = table_.GetStaticEntries();
  EXPECT_EQ(arraysize(kHpackStaticTable), static_entries.size());

  HpackHeaderTable::UnorderedEntrySet static_index = table_.GetStaticIndex();
  EXPECT_EQ(arraysize(kHpackStaticTable), static_index.size());

  HpackHeaderTable::NameToEntryMap static_name_index =
      table_.GetStaticNameIndex();
  std::set<SpdyStringPiece> names;
  for (auto entry : static_index) {
    names.insert(entry->name());
  }
  EXPECT_EQ(names.size(), static_name_index.size());
}

// Test that ObtainHpackStaticTable returns the same instance every time.
TEST_F(HpackStaticTableTest, IsSingleton) {
  const HpackStaticTable* static_table_one = &ObtainHpackStaticTable();
  const HpackStaticTable* static_table_two = &ObtainHpackStaticTable();
  EXPECT_EQ(static_table_one, static_table_two);
}

}  // namespace

}  // namespace test

}  // namespace gfe_spdy
