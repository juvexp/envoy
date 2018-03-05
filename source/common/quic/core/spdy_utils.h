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

#ifndef GFE_QUIC_CORE_SPDY_UTILS_H_
#define GFE_QUIC_CORE_SPDY_UTILS_H_

#include <cstddef>
#include <cstdint>

#include "base/macros.h"
#include "common/quic/core/quic_header_list.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/spdy_utils/core/spdy_framer.h"

namespace gfe_quic {

class QUIC_EXPORT_PRIVATE SpdyUtils {
 public:
  // Populate |content length| with the value of the content-length header.
  // Returns true on success, false if parsing fails or content-length header is
  // missing.
  static bool ExtractContentLengthFromHeaders(
      int64_t* content_length,
      gfe_spdy::SpdyHeaderBlock* headers);

  // Copies a list of headers to a SpdyHeaderBlock.
  static bool CopyAndValidateHeaders(const QuicHeaderList& header_list,
                                     int64_t* content_length,
                                     gfe_spdy::SpdyHeaderBlock* headers);

  // Copies a list of headers to a SpdyHeaderBlock.
  static bool CopyAndValidateTrailers(const QuicHeaderList& header_list,
                                      size_t* final_byte_offset,
                                      gfe_spdy::SpdyHeaderBlock* trailers);

  // Returns a canonicalized URL composed from the :scheme, :authority, and
  // :path headers of a PUSH_PROMISE. Returns empty string if the headers do not
  // conform to HTTP/2 spec or if the ":method" header contains a forbidden
  // method for PUSH_PROMISE.
  static QuicString GetPromisedUrlFromHeaders(
      const gfe_spdy::SpdyHeaderBlock& headers);

  // Returns hostname, or empty string if missing.
  static QuicString GetPromisedHostNameFromHeaders(
      const gfe_spdy::SpdyHeaderBlock& headers);

  // Returns true if result of |GetPromisedUrlFromHeaders()| is non-empty
  // and is a well-formed URL.
  static bool PromisedUrlIsValid(const gfe_spdy::SpdyHeaderBlock& headers);

  // Populates the fields of |headers| to make a GET request of |url|,
  // which must be fully-qualified.
  static bool PopulateHeaderBlockFromUrl(const QuicString url,
                                         gfe_spdy::SpdyHeaderBlock* headers);

 private:
  DISALLOW_COPY_AND_ASSIGN(SpdyUtils);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_SPDY_UTILS_H_
