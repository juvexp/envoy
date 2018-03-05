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

#include "common/quic/http/decoder/quic_http_decode_status.h"

#include "base/logging.h"
#include "common/quic/platform/api/quic_bug_tracker.h"

namespace gfe_quic {

std::ostream& operator<<(std::ostream& out, QuicHttpDecodeStatus v) {
  switch (v) {
    case QuicHttpDecodeStatus::kDecodeDone:
      return out << "DecodeDone";
    case QuicHttpDecodeStatus::kDecodeInProgress:
      return out << "DecodeInProgress";
    case QuicHttpDecodeStatus::kDecodeError:
      return out << "DecodeError";
  }
  // Since the value doesn't come over the wire, only a programming bug should
  // result in reaching this point.
  int unknown = static_cast<int>(v);
  QUIC_BUG << "Unknown QuicHttpDecodeStatus " << unknown;
  return out << "QuicHttpDecodeStatus(" << unknown << ")";
}

}  // namespace gfe_quic
