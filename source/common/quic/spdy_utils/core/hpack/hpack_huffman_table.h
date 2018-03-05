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

#ifndef GFE_SPDY_CORE_HPACK_HPACK_HUFFMAN_TABLE_H_
#define GFE_SPDY_CORE_HPACK_HPACK_HUFFMAN_TABLE_H_

#include <cstddef>
#include <cstdint>
#include <vector>

#include "common/quic/spdy_utils/core/hpack/hpack_constants.h"
#include "common/quic/spdy_utils/platform/api/spdy_export.h"
#include "common/quic/spdy_utils/platform/api/spdy_string.h"
#include "common/quic/spdy_utils/platform/api/spdy_string_piece.h"

namespace gfe_spdy {

namespace test {
class HpackHuffmanTablePeer;
}  // namespace test

class HpackOutputStream;

// HpackHuffmanTable encodes string literals using a constructed canonical
// Huffman code. Once initialized, an instance is read only and may be accessed
// only through its const interface.
class SPDY_EXPORT_PRIVATE HpackHuffmanTable {
 public:
  friend class test::HpackHuffmanTablePeer;

  typedef HpackHuffmanSymbol Symbol;

  HpackHuffmanTable();
  ~HpackHuffmanTable();

  // Prepares HpackHuffmanTable to encode the canonical Huffman code as
  // determined by the given symbols. Must be called exactly once.
  // Returns false if the input symbols define an invalid coding, and true
  // otherwise. Symbols must be presented in ascending ID order with no gaps,
  // and |symbol_count| must fit in a uint16_t.
  bool Initialize(const Symbol* input_symbols, size_t symbol_count);

  // Returns whether Initialize() has been successfully called.
  bool IsInitialized() const;

  // Encodes the input string to the output stream using the table's Huffman
  // context.
  void EncodeString(SpdyStringPiece in, HpackOutputStream* out) const;

  // Returns the encoded size of the input string.
  size_t EncodedSize(SpdyStringPiece in) const;

 private:
  // Expects symbols ordered on ID ascending.
  void BuildEncodeTable(const std::vector<Symbol>& symbols);

  // Symbol code and code length, in ascending symbol ID order.
  // Codes are stored in the most-significant bits of the word.
  std::vector<uint32_t> code_by_id_;
  std::vector<uint8_t> length_by_id_;

  // The first 8 bits of the longest code. Applied when generating padding bits.
  uint8_t pad_bits_;

  // If initialization fails, preserve the symbol ID which failed validation
  // for examination in tests.
  uint16_t failed_symbol_id_;
};

}  // namespace gfe_spdy

#endif  // GFE_SPDY_CORE_HPACK_HPACK_HUFFMAN_TABLE_H_
