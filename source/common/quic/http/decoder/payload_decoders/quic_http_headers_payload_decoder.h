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

#ifndef GFE_QUIC_HTTP_DECODER_PAYLOAD_DECODERS_QUIC_HTTP_HEADERS_PAYLOAD_DECODER_H_
#define GFE_QUIC_HTTP_DECODER_PAYLOAD_DECODERS_QUIC_HTTP_HEADERS_PAYLOAD_DECODER_H_

// Decodes the payload of a HEADERS frame.
// See http://g3doc/gfe/quic/http/decoder/payload_decoders/README.md for info
// about payload decoders.

#include "common/quic/http/decoder/quic_http_decode_buffer.h"
#include "common/quic/http/decoder/quic_http_decode_status.h"
#include "common/quic/http/decoder/quic_http_frame_decoder_state.h"
#include "common/quic/http/quic_http_structures.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {
namespace test {
class QuicHttpHeadersQuicHttpPayloadDecoderPeer;
}  // namespace test

class QUIC_EXPORT_PRIVATE QuicHttpHeadersQuicHttpPayloadDecoder {
 public:
  // States during decoding of a HEADERS frame, unless the fast path kicks
  // in, in which case the state machine will be bypassed.
  enum class PayloadState {
    // The QUIC_HTTP_PADDED flag is set, and we now need to read the Pad Length
    // field
    // (the first byte of the payload, after the common frame header).
    kReadPadLength,

    // The QUIC_HTTP_PRIORITY flag is set, and we now need to read the fixed
    // size priority
    // fields (E, Stream Dependency, Weight) into priority_fields_.  Calls on
    // OnHeadersPriority if completely decodes those fields.
    kStartDecodingPriorityFields,

    // The decoder passes the non-padding portion of the remaining payload
    // (i.e. the HPQUIC_HTTP_ACK block fragment) to the listener's
    // OnHpackFragment method.
    kReadPayload,

    // The decoder has finished with the HPQUIC_HTTP_ACK block fragment, and is
    // now
    // ready to skip the trailing padding, if the frame has any.
    kSkipPadding,

    // The fixed size fields weren't all available when the decoder first tried
    // to decode them (state kStartDecodingPriorityFields); this state resumes
    // the decoding when ResumeDecodingPayload is called later.
    kResumeDecodingPriorityFields,
  };

  // Starts the decoding of a HEADERS frame's payload, and completes it if
  // the entire payload is in the provided decode buffer.
  QuicHttpDecodeStatus StartDecodingPayload(QuicHttpFrameDecoderState* state,
                                            QuicHttpDecodeBuffer* db);

  // Resumes decoding a HEADERS frame's payload that has been split across
  // decode buffers.
  QuicHttpDecodeStatus ResumeDecodingPayload(QuicHttpFrameDecoderState* state,
                                             QuicHttpDecodeBuffer* db);

 private:
  friend class test::QuicHttpHeadersQuicHttpPayloadDecoderPeer;

  PayloadState payload_state_;
  QuicHttpPriorityFields priority_fields_;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_DECODER_PAYLOAD_DECODERS_QUIC_HTTP_HEADERS_PAYLOAD_DECODER_H_
