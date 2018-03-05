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

#ifndef GFE_QUIC_HTTP_DECODER_QUIC_HTTP_DECODE_STRUCTURES_H_
#define GFE_QUIC_HTTP_DECODER_QUIC_HTTP_DECODE_STRUCTURES_H_

// Provides functions for decoding the fixed size structures in the HTTP/2 spec.

#include "common/quic/http/decoder/quic_http_decode_buffer.h"
#include "common/quic/http/quic_http_structures.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

// DoDecode(STRUCTURE* out, QuicHttpDecodeBuffer* b) decodes the structure from
// start to end, advancing the cursor by STRUCTURE::EncodedSize(). The decode
// buffer must be large enough (i.e. b->Remaining() >=
// STRUCTURE::EncodedSize()).

QUIC_EXPORT_PRIVATE void DoDecode(QuicHttpFrameHeader* out,
                                  QuicHttpDecodeBuffer* b);
QUIC_EXPORT_PRIVATE void DoDecode(QuicHttpPriorityFields* out,
                                  QuicHttpDecodeBuffer* b);
QUIC_EXPORT_PRIVATE void DoDecode(QuicHttpRstStreamFields* out,
                                  QuicHttpDecodeBuffer* b);
QUIC_EXPORT_PRIVATE void DoDecode(QuicHttpSettingFields* out,
                                  QuicHttpDecodeBuffer* b);
QUIC_EXPORT_PRIVATE void DoDecode(QuicHttpPushPromiseFields* out,
                                  QuicHttpDecodeBuffer* b);
QUIC_EXPORT_PRIVATE void DoDecode(QuicHttpPingFields* out,
                                  QuicHttpDecodeBuffer* b);
QUIC_EXPORT_PRIVATE void DoDecode(QuicHttpGoAwayFields* out,
                                  QuicHttpDecodeBuffer* b);
QUIC_EXPORT_PRIVATE void DoDecode(QuicHttpWindowUpdateFields* out,
                                  QuicHttpDecodeBuffer* b);
QUIC_EXPORT_PRIVATE void DoDecode(QuicHttpAltSvcFields* out,
                                  QuicHttpDecodeBuffer* b);

}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_DECODER_QUIC_HTTP_DECODE_STRUCTURES_H_
