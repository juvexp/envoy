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

// Copyright 2015 Google Inc. All Rights Reserved.
// Author: bnc@google.com (Bence Béky)

// This file contains data structures and utility functions used for serializing
// and parsing alternative service header values, common to HTTP/1.1 header
// fields and HTTP/2 and QUIC ALTSVC frames.  See specification at
// https://httpwg.github.io/http-extensions/alt-svc.html.

#ifndef GFE_SPDY_CORE_SPDY_ALT_SVC_WIRE_FORMAT_H_
#define GFE_SPDY_CORE_SPDY_ALT_SVC_WIRE_FORMAT_H_

#include <cstdint>
#include <vector>

#include "common/quic/spdy_utils/platform/api/spdy_export.h"
#include "common/quic/spdy_utils/platform/api/spdy_string.h"
#include "common/quic/spdy_utils/platform/api/spdy_string_piece.h"
#include "third_party/absl/container/inlined_vector.h"
#include "util/gtl/inlined_vector.h"

namespace gfe_spdy {

namespace test {
class SpdyAltSvcWireFormatPeer;
}  // namespace test

class SPDY_EXPORT_PRIVATE SpdyAltSvcWireFormat {
 public:
  using VersionVector = absl::InlinedVector<uint32_t, 8>;

  struct SPDY_EXPORT_PRIVATE AlternativeService {
    SpdyString protocol_id;
    SpdyString host;
    // Default is 0: invalid port.
    uint16_t port = 0;
    // Default is one day.
    uint32_t max_age = 86400;
    // Default is empty: unspecified version.
    VersionVector version;

    AlternativeService();
    AlternativeService(const SpdyString& protocol_id,
                       const SpdyString& host,
                       uint16_t port,
                       uint32_t max_age,
                       VersionVector version);
    AlternativeService(const AlternativeService& other);
    ~AlternativeService();

    bool operator==(const AlternativeService& other) const {
      return protocol_id == other.protocol_id && host == other.host &&
             port == other.port && version == other.version &&
             max_age == other.max_age;
    }
  };
  // An empty vector means alternative services should be cleared for given
  // origin.  Note that the wire format for this is the string "clear", not an
  // empty value (which is invalid).
  typedef std::vector<AlternativeService> AlternativeServiceVector;

  friend class test::SpdyAltSvcWireFormatPeer;
  static bool ParseHeaderFieldValue(SpdyStringPiece value,
                                    AlternativeServiceVector* altsvc_vector);
  static SpdyString SerializeHeaderFieldValue(
      const AlternativeServiceVector& altsvc_vector);

 private:
  static void SkipWhiteSpace(SpdyStringPiece::const_iterator* c,
                             SpdyStringPiece::const_iterator end);
  static bool PercentDecode(SpdyStringPiece::const_iterator c,
                            SpdyStringPiece::const_iterator end,
                            SpdyString* output);
  static bool ParseAltAuthority(SpdyStringPiece::const_iterator c,
                                SpdyStringPiece::const_iterator end,
                                SpdyString* host,
                                uint16_t* port);
  static bool ParsePositiveInteger16(SpdyStringPiece::const_iterator c,
                                     SpdyStringPiece::const_iterator end,
                                     uint16_t* value);
  static bool ParsePositiveInteger32(SpdyStringPiece::const_iterator c,
                                     SpdyStringPiece::const_iterator end,
                                     uint32_t* value);
};

}  // namespace gfe_spdy

#endif  // GFE_SPDY_CORE_SPDY_ALT_SVC_WIRE_FORMAT_H_
