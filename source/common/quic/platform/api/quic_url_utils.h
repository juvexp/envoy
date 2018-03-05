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

#ifndef GFE_QUIC_PLATFORM_API_QUIC_URL_UTILS_H_
#define GFE_QUIC_PLATFORM_API_QUIC_URL_UTILS_H_

#include "base/macros.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"
#include "common/quic/platform/impl/quic_url_utils_impl.h"

namespace gfe_quic {

class QUIC_EXPORT_PRIVATE QuicUrlUtils {
 public:
  // Returns hostname, or empty string if missing.
  static QuicString HostName(QuicStringPiece url);

  // Returns false if any of these conditions occur: (1) Host name too long; (2)
  // Invalid characters in host name, path or params; (3) Invalid port number
  // (e.g. greater than 65535).
  static bool IsValidUrl(QuicStringPiece url);

  // Returns a canonical, valid URL for a PUSH_PROMISE with the specified
  // ":scheme", ":authority", and ":path" header fields, or an empty
  // string if the resulting URL is not valid or supported.
  static QuicString GetPushPromiseUrl(QuicStringPiece scheme,
                                      QuicStringPiece authority,
                                      QuicStringPiece path);

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicUrlUtils);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_PLATFORM_API_QUIC_URL_UTILS_H_
