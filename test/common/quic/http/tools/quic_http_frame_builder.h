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

#ifndef GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_FRAME_BUILDER_H_
#define GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_FRAME_BUILDER_H_

// QuicHttpFrameBuilder builds wire-format HTTP/2 frames (or fragments thereof)
// from components.
//
// For now, this is only intended for use in tests, and thus has EXPECT* in the
// code. If desired to use it in an encoder, it will need optimization work,
// especially w.r.t memory mgmt, and the EXPECT* will need to be removed or
// replaced with DCHECKs.

#include <stddef.h>  // for size_t

#include <cstdint>

#include "common/quic/http/quic_http_constants.h"
#include "common/quic/http/quic_http_structures.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {
namespace test {

class QuicHttpFrameBuilder {
 public:
  QuicHttpFrameBuilder(QuicHttpFrameType type,
                       uint8_t flags,
                       uint32_t stream_id);
  explicit QuicHttpFrameBuilder(const QuicHttpFrameHeader& v);
  QuicHttpFrameBuilder() {}
  ~QuicHttpFrameBuilder() {}

  size_t size() const { return buffer_.size(); }
  const QuicString& buffer() const { return buffer_; }

  //----------------------------------------------------------------------------
  // Methods for appending to the end of the buffer.

  // Append a sequence of bytes from various sources.
  void Append(QuicStringPiece s);
  void AppendBytes(const void* data, uint32_t num_bytes);

  // Append an array of type T[N] to the string. Intended for tests with arrays
  // initialized from literals, such as:
  //    const char kData[] = {0x12, 0x23, ...};
  //    builder.AppendBytes(kData);
  template <typename T, size_t N>
  void AppendBytes(T (&buf)[N]) {
    AppendBytes(buf, N * sizeof(buf[0]));
  }

  // Support for appending padding. Does not read or write the Pad Length field.
  void AppendZeroes(size_t num_zero_bytes);

  // Append various sizes of unsigned integers.
  void AppendUInt8(uint8_t value);
  void AppendUInt16(uint16_t value);
  void AppendUInt24(uint32_t value);
  void AppendUInt31(uint32_t value);
  void AppendUInt32(uint32_t value);

  // Append various enums.
  void Append(QuicHttpErrorCode error_code);
  void Append(QuicHttpFrameType type);
  void Append(QuicHttpSettingsParameter parameter);

  // Append various structures.
  void Append(const QuicHttpFrameHeader& v);
  void Append(const QuicHttpPriorityFields& v);
  void Append(const QuicHttpRstStreamFields& v);
  void Append(const QuicHttpSettingFields& v);
  void Append(const QuicHttpPushPromiseFields& v);
  void Append(const QuicHttpPingFields& v);
  void Append(const QuicHttpGoAwayFields& v);
  void Append(const QuicHttpWindowUpdateFields& v);
  void Append(const QuicHttpAltSvcFields& v);

  // Methods for changing existing buffer contents (mostly focused on updating
  // the payload length).

  void WriteAt(QuicStringPiece s, size_t offset);
  void WriteBytesAt(const void* data, uint32_t num_bytes, size_t offset);
  void WriteUInt24At(uint32_t value, size_t offset);

  // Set the payload length to the specified size.
  void SetPayloadLength(uint32_t payload_length);

  // Sets the payload length to the size of the buffer minus the size of
  // the frame header.
  size_t SetPayloadLength();

 private:
  QuicString buffer_;
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_FRAME_BUILDER_H_
