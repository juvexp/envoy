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

#ifndef GFE_QUIC_HTTP_DECODER_PAYLOAD_DECODERS_QUIC_HTTP_GOAWAY_PAYLOAD_DECODER_H_
#define GFE_QUIC_HTTP_DECODER_PAYLOAD_DECODERS_QUIC_HTTP_GOAWAY_PAYLOAD_DECODER_H_

// Decodes the payload of a GOAWAY frame.
// See http://g3doc/gfe/quic/http/decoder/payload_decoders/README.md for info
// about payload decoders.

// TODO: Sweep through all payload decoders, changing the names of
// the PayloadState enums so that they are really states, and not actions.

#include "common/quic/http/decoder/quic_http_decode_buffer.h"
#include "common/quic/http/decoder/quic_http_decode_status.h"
#include "common/quic/http/decoder/quic_http_frame_decoder_state.h"
#include "common/quic/http/quic_http_structures.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {
namespace test {
class QuicHttpGoAwayQuicHttpPayloadDecoderPeer;
}  // namespace test

class QUIC_EXPORT_PRIVATE QuicHttpGoAwayQuicHttpPayloadDecoder {
 public:
  // States during decoding of a GOAWAY frame.
  enum class PayloadState {
    // At the start of the GOAWAY frame payload, ready to start decoding the
    // fixed size fields into goaway_fields_.
    kStartDecodingFixedFields,

    // Handle the QuicHttpDecodeStatus returned from starting or resuming the
    // decoding of QuicHttpGoAwayFields into goaway_fields_. If complete,
    // calls OnGoAwayStart.
    kHandleFixedFieldsStatus,

    // Report the Opaque Data portion of the payload to the listener's
    // OnGoAwayOpaqueData method, and call OnGoAwayEnd when the end of the
    // payload is reached.
    kReadOpaqueData,

    // The fixed size fields weren't all available when the decoder first
    // tried to decode them (state kStartDecodingFixedFields); this state
    // resumes the decoding when ResumeDecodingPayload is called later.
    kResumeDecodingFixedFields,
  };

  // Starts the decoding of a GOAWAY frame's payload, and completes it if
  // the entire payload is in the provided decode buffer.
  QuicHttpDecodeStatus StartDecodingPayload(QuicHttpFrameDecoderState* state,
                                            QuicHttpDecodeBuffer* db);

  // Resumes decoding a GOAWAY frame's payload that has been split across
  // decode buffers.
  QuicHttpDecodeStatus ResumeDecodingPayload(QuicHttpFrameDecoderState* state,
                                             QuicHttpDecodeBuffer* db);

 private:
  friend class test::QuicHttpGoAwayQuicHttpPayloadDecoderPeer;

  QuicHttpGoAwayFields goaway_fields_;
  PayloadState payload_state_;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_DECODER_PAYLOAD_DECODERS_QUIC_HTTP_GOAWAY_PAYLOAD_DECODER_H_
