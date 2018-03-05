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

#ifndef GFE_SPDY_CORE_SPDY_HEADERS_HANDLER_INTERFACE_H_
#define GFE_SPDY_CORE_SPDY_HEADERS_HANDLER_INTERFACE_H_

#include "common/quic/spdy_utils/platform/api/spdy_export.h"
#include "common/quic/spdy_utils/platform/api/spdy_string_piece.h"

namespace gfe_spdy {

// This interface defines how an object that accepts header data should behave.
// It is used by both SpdyHeadersBlockParser and HpackDecoder.
class SPDY_EXPORT_PRIVATE SpdyHeadersHandlerInterface {
 public:
  virtual ~SpdyHeadersHandlerInterface() {}

  // A callback method which notifies when the parser starts handling a new
  // header block. Will only be called once per block, even if it extends into
  // CONTINUATION frames.
  virtual void OnHeaderBlockStart() = 0;

  // A callback method which notifies on a header key value pair. Multiple
  // values for a given key will be emitted as multiple calls to OnHeader.
  virtual void OnHeader(SpdyStringPiece key, SpdyStringPiece value) = 0;

  // A callback method which notifies when the parser finishes handling a
  // header block (i.e. the containing frame has the END_HEADERS flag set).
  // Also indicates the total number of bytes in this block.
  virtual void OnHeaderBlockEnd(size_t uncompressed_header_bytes,
                                size_t compressed_header_bytes) = 0;
};

}  // namespace gfe_spdy

#endif  // GFE_SPDY_CORE_SPDY_HEADERS_HANDLER_INTERFACE_H_
