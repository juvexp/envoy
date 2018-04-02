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

#ifndef GFE_SPDY_CORE_HPACK_HPACK_ENCODER_H_
#define GFE_SPDY_CORE_HPACK_HPACK_ENCODER_H_

#include <map>
#include <memory>

#include "base/macros.h"
#include "common/quic/spdy_utils/core/hpack/hpack_header_table.h"
#include "common/quic/spdy_utils/core/hpack/hpack_output_stream.h"
#include "common/quic/spdy_utils/core/spdy_protocol.h"
#include "common/quic/spdy_utils/platform/api/spdy_export.h"
#include "common/quic/spdy_utils/platform/api/spdy_string.h"
#include "common/quic/spdy_utils/platform/api/spdy_string_piece.h"

// An HpackEncoder encodes header sets as outlined in
// http://tools.ietf.org/html/rfc7541

namespace gfe_spdy {

class HpackHuffmanTable;

namespace test {
class HpackEncoderPeer;
}  // namespace test

class SPDY_EXPORT_PRIVATE HpackEncoder {
 public:
  using Representation = std::pair<SpdyStringPiece, SpdyStringPiece>;
  using Representations = std::vector<Representation>;

  // Callers may provide a HeaderListener to be informed of header name-value
  // pairs processed by this encoder.
  using HeaderListener = std::function<void(SpdyStringPiece, SpdyStringPiece)>;

  // An indexing policy should return true if the provided header name-value
  // pair should be inserted into the HPACK dynamic table.
  using IndexingPolicy = std::function<bool(SpdyStringPiece, SpdyStringPiece)>;

  // |table| is an initialized HPACK Huffman table, having an
  // externally-managed lifetime which spans beyond HpackEncoder.
  explicit HpackEncoder(const HpackHuffmanTable& table);
  ~HpackEncoder();

  // Encodes the given header set into the given string. Returns
  // whether or not the encoding was successful.
  bool EncodeHeaderSet(const SpdyHeaderBlock& header_set, SpdyString* output);

  class SPDY_EXPORT_PRIVATE ProgressiveEncoder {
   public:
    virtual ~ProgressiveEncoder() {}

    // Returns true iff more remains to encode.
    virtual bool HasNext() const = 0;

    // Encodes up to max_encoded_bytes of the current header block into the
    // given output string.
    virtual void Next(size_t max_encoded_bytes, SpdyString* output) = 0;
  };

  // Returns a ProgressiveEncoder which must be outlived by both the given
  // SpdyHeaderBlock and this object.
  std::unique_ptr<ProgressiveEncoder> EncodeHeaderSet(
      const SpdyHeaderBlock& header_set);

  // Called upon a change to SETTINGS_HEADER_TABLE_SIZE. Specifically, this
  // is to be called after receiving (and sending an acknowledgement for) a
  // SETTINGS_HEADER_TABLE_SIZE update from the remote decoding endpoint.
  void ApplyHeaderTableSizeSetting(size_t size_setting);

  size_t CurrentHeaderTableSizeSetting() const {
    return header_table_.settings_size_bound();
  }

  // This HpackEncoder will use |policy| to determine whether to insert header
  // name-value pairs into the dynamic table.
  void SetIndexingPolicy(IndexingPolicy policy) { should_index_ = policy; }

  // |listener| will be invoked for each header name-value pair processed by
  // this encoder.
  void SetHeaderListener(HeaderListener listener) { listener_ = listener; }

  void SetHeaderTableDebugVisitor(
      std::unique_ptr<HpackHeaderTable::DebugVisitorInterface> visitor) {
    header_table_.set_debug_visitor(std::move(visitor));
  }

  void DisableCompression() { enable_compression_ = false; }

 private:
  friend class test::HpackEncoderPeer;

  class RepresentationIterator;
  class Encoderator;

  // Encodes a sequence of header name-value pairs as a single header block.
  void EncodeRepresentations(RepresentationIterator* iter, SpdyString* output);

  // Emits a static/dynamic indexed representation (Section 7.1).
  void EmitIndex(const HpackEntry* entry);

  // Emits a literal representation (Section 7.2).
  void EmitIndexedLiteral(const Representation& representation);
  void EmitNonIndexedLiteral(const Representation& representation);
  void EmitLiteral(const Representation& representation);

  // Emits a Huffman or identity string (whichever is smaller).
  void EmitString(SpdyStringPiece str);

  // Emits the current dynamic table size if the table size was recently
  // updated and we have not yet emitted it (Section 6.3).
  void MaybeEmitTableSize();

  // Crumbles a cookie header into ";" delimited crumbs.
  static void CookieToCrumbs(const Representation& cookie,
                             Representations* crumbs_out);

  // Crumbles other header field values at \0 delimiters.
  static void DecomposeRepresentation(const Representation& header_field,
                                      Representations* out);

  // Gathers headers without crumbling. Used when compression is not enabled.
  static void GatherRepresentation(const Representation& header_field,
                                   Representations* out);

  HpackHeaderTable header_table_;
  HpackOutputStream output_stream_;

  const HpackHuffmanTable& huffman_table_;
  size_t min_table_size_setting_received_;
  HeaderListener listener_;
  IndexingPolicy should_index_;
  bool enable_compression_;
  bool should_emit_table_size_;

  DISALLOW_COPY_AND_ASSIGN(HpackEncoder);
};

}  // namespace gfe_spdy

#endif  // GFE_SPDY_CORE_HPACK_HPACK_ENCODER_H_
