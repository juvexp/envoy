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

#ifndef GFE_SPDY_CORE_HPACK_HPACK_CONSTANTS_H_
#define GFE_SPDY_CORE_HPACK_HPACK_CONSTANTS_H_

#include <cstddef>
#include <cstdint>

#include "common/quic/spdy_utils/platform/api/spdy_export.h"

// All section references below are to
// http://tools.ietf.org/html/draft-ietf-httpbis-header-compression-08

namespace gfe_spdy {

// An HpackPrefix signifies |bits| stored in the top |bit_size| bits
// of an octet.
struct HpackPrefix {
  uint8_t bits;
  size_t bit_size;
};

// Represents a symbol and its Huffman code (stored in most-significant bits).
struct HpackHuffmanSymbol {
  uint32_t code;
  uint8_t length;
  uint16_t id;
};

// An entry in the static table. Must be a POD in order to avoid static
// initializers, i.e. no user-defined constructors or destructors.
struct HpackStaticEntry {
  const char* const name;
  const size_t name_len;
  const char* const value;
  const size_t value_len;
};

class HpackHuffmanTable;
class HpackStaticTable;

// Defined in RFC 7540, 6.5.2.
const uint32_t kDefaultHeaderTableSizeSetting = 4096;

// RFC 7541, 5.2: Flag for a string literal that is stored unmodified (i.e.,
// without Huffman encoding).
const HpackPrefix kStringLiteralIdentityEncoded = {0x0, 1};

// RFC 7541, 5.2: Flag for a Huffman-coded string literal.
const HpackPrefix kStringLiteralHuffmanEncoded = {0x1, 1};

// RFC 7541, 6.1: Opcode for an indexed header field.
const HpackPrefix kIndexedOpcode = {0b1, 1};

// RFC 7541, 6.2.1: Opcode for a literal header field with incremental indexing.
const HpackPrefix kLiteralIncrementalIndexOpcode = {0b01, 2};

// RFC 7541, 6.2.2: Opcode for a literal header field without indexing.
const HpackPrefix kLiteralNoIndexOpcode = {0b0000, 4};

// RFC 7541, 6.2.3: Opcode for a literal header field which is never indexed.
// Currently unused.
// const HpackPrefix kLiteralNeverIndexOpcode = {0b0001, 4};

// RFC 7541, 6.3: Opcode for maximum header table size update. Begins a
// varint-encoded table size with a 5-bit prefix.
const HpackPrefix kHeaderTableSizeUpdateOpcode = {0b001, 3};

// Symbol code table from RFC 7541, "Appendix C. Huffman Code".
extern const HpackHuffmanSymbol kHpackHuffmanCode[257];

// Static table from RFC 7541, "Appendix B. Static Table Definition".
extern const HpackStaticEntry kHpackStaticTable[61];

// Returns a HpackHuffmanTable instance initialized with |kHpackHuffmanCode|.
// The instance is read-only, has static lifetime, and is safe to share amoung
// threads. This function is thread-safe.
SPDY_EXPORT_PRIVATE const HpackHuffmanTable& ObtainHpackHuffmanTable();

// Returns a HpackStaticTable instance initialized with |kHpackStaticTable|.
// The instance is read-only, has static lifetime, and is safe to share amoung
// threads. This function is thread-safe.
SPDY_EXPORT_PRIVATE const HpackStaticTable& ObtainHpackStaticTable();

// Pseudo-headers start with a colon.  (HTTP2 8.1.2.1., HPACK 3.1.)
const char kPseudoHeaderPrefix = ':';

}  // namespace gfe_spdy

#endif  // GFE_SPDY_CORE_HPACK_HPACK_CONSTANTS_H_
