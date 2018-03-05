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

#ifndef GFE_QUIC_PLATFORM_API_QUIC_HOSTNAME_UTILS_H_
#define GFE_QUIC_PLATFORM_API_QUIC_HOSTNAME_UTILS_H_

#include "base/macros.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string_piece.h"
#include "common/quic/platform/impl/quic_hostname_utils_impl.h"

namespace gfe_quic {

class QUIC_EXPORT_PRIVATE QuicHostnameUtils {
 public:
  // Returns true if the sni is valid, false otherwise.
  //  (1) disallow IP addresses;
  //  (2) check that the hostname contains valid characters only; and
  //  (3) contains at least one dot.
  static bool IsValidSNI(QuicStringPiece sni);

  // Convert hostname to lowercase and remove the trailing '.'.
  // WARNING: mutates |hostname| in place and returns |hostname|.
  static char* NormalizeHostname(char* hostname);

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicHostnameUtils);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_PLATFORM_API_QUIC_HOSTNAME_UTILS_H_
