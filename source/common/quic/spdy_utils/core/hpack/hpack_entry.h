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

#ifndef GFE_SPDY_CORE_HPACK_HPACK_ENTRY_H_
#define GFE_SPDY_CORE_HPACK_HPACK_ENTRY_H_

#include <cstdint>

#include "base/macros.h"
#include "common/quic/spdy_utils/platform/api/spdy_export.h"
#include "common/quic/spdy_utils/platform/api/spdy_string.h"
#include "common/quic/spdy_utils/platform/api/spdy_string_piece.h"

// All section references below are to
// http://tools.ietf.org/html/draft-ietf-httpbis-header-compression-08

namespace gfe_spdy {

// A structure for an entry in the static table (3.3.1)
// and the header table (3.3.2).
class SPDY_EXPORT_PRIVATE HpackEntry {
 public:
  // The constant amount added to name().size() and value().size() to
  // get the size of an HpackEntry as defined in 5.1.
  static const size_t kSizeOverhead;

  // Creates an entry. Preconditions:
  // - |is_static| captures whether this entry is a member of the static
  //   or dynamic header table.
  // - |insertion_index| is this entry's index in the total set of entries ever
  //   inserted into the header table (including static entries).
  //
  // The combination of |is_static| and |insertion_index| allows an
  // HpackEntryTable to determine the index of an HpackEntry in O(1) time.
  // Copies |name| and |value|.
  HpackEntry(SpdyStringPiece name,
             SpdyStringPiece value,
             bool is_static,
             size_t insertion_index);

  // Create a 'lookup' entry (only) suitable for querying a HpackEntrySet. The
  // instance InsertionIndex() always returns 0 and IsLookup() returns true.
  // The memory backing |name| and |value| must outlive this object.
  HpackEntry(SpdyStringPiece name, SpdyStringPiece value);

  HpackEntry(const HpackEntry& other);
  HpackEntry& operator=(const HpackEntry& other);

  // Creates an entry with empty name and value. Only defined so that
  // entries can be stored in STL containers.
  HpackEntry();

  ~HpackEntry();

  SpdyStringPiece name() const { return name_ref_; }
  SpdyStringPiece value() const { return value_ref_; }

  // Returns whether this entry is a member of the static (as opposed to
  // dynamic) table.
  bool IsStatic() const { return type_ == STATIC; }

  // Returns whether this entry is a lookup-only entry.
  bool IsLookup() const { return type_ == LOOKUP; }

  // Used to compute the entry's index in the header table.
  size_t InsertionIndex() const { return insertion_index_; }

  // Returns the size of an entry as defined in 5.1.
  static size_t Size(SpdyStringPiece name, SpdyStringPiece value);
  size_t Size() const;

  SpdyString GetDebugString() const;

  int64_t time_added() const { return time_added_; }
  void set_time_added(int64_t now) { time_added_ = now; }

  // Returns the estimate of dynamically allocated memory in bytes.
  size_t EstimateMemoryUsage() const;

 private:
  enum EntryType {
    LOOKUP,
    DYNAMIC,
    STATIC,
  };

  // These members are not used for LOOKUP entries.
  SpdyString name_;
  SpdyString value_;

  // These members are always valid. For DYNAMIC and STATIC entries, they
  // always point to |name_| and |value_|.
  SpdyStringPiece name_ref_;
  SpdyStringPiece value_ref_;

  // The entry's index in the total set of entries ever inserted into the header
  // table.
  size_t insertion_index_;

  EntryType type_;

  // For HpackHeaderTable::DebugVisitorInterface
  int64_t time_added_;
};

}  // namespace gfe_spdy

#endif  // GFE_SPDY_CORE_HPACK_HPACK_ENTRY_H_
