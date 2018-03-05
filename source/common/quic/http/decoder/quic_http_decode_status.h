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

#ifndef GFE_QUIC_HTTP_DECODER_QUIC_HTTP_DECODE_STATUS_H_
#define GFE_QUIC_HTTP_DECODER_QUIC_HTTP_DECODE_STATUS_H_

// Enum QuicHttpDecodeStatus is used to report the status of decoding of many
// types of HTTP/2 and HPQUIC_HTTP_ACK objects.

#include <ostream>

#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

enum class QuicHttpDecodeStatus {
  // Decoding is done.
  kDecodeDone,

  // Decoder needs more input to be able to make progress.
  kDecodeInProgress,

  // Decoding failed (e.g. HPQUIC_HTTP_ACK variable length integer is too large,
  // or
  // an HTTP/2 frame has padding declared to be larger than the payload).
  kDecodeError,
};
QUIC_EXPORT_PRIVATE std::ostream& operator<<(std::ostream& out,
                                             QuicHttpDecodeStatus v);

}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_DECODER_QUIC_HTTP_DECODE_STATUS_H_
