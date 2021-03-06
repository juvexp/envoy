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

#ifndef GFE_QUIC_HTTP_DECODER_PAYLOAD_DECODERS_QUIC_HTTP_ALTSVC_PAYLOAD_DECODER_H_
#define GFE_QUIC_HTTP_DECODER_PAYLOAD_DECODERS_QUIC_HTTP_ALTSVC_PAYLOAD_DECODER_H_

// Decodes the payload of a ALTSVC frame.
// See http://g3doc/gfe/quic/http/decoder/payload_decoders/README.md for info
// about payload decoders.

#include "common/quic/http/decoder/quic_http_decode_buffer.h"
#include "common/quic/http/decoder/quic_http_decode_status.h"
#include "common/quic/http/decoder/quic_http_frame_decoder_state.h"
#include "common/quic/http/quic_http_structures.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {
namespace test {
class QuicHttpAltSvcQuicHttpPayloadDecoderPeer;
}  // namespace test

class QUIC_EXPORT_PRIVATE QuicHttpAltSvcQuicHttpPayloadDecoder {
 public:
  // States during decoding of a ALTSVC frame.
  enum class PayloadState {
    // Start decoding the fixed size structure at the start of an ALTSVC
    // frame (QuicHttpAltSvcFields).
    kStartDecodingStruct,

    // Handle the QuicHttpDecodeStatus returned from starting or resuming the
    // decoding of QuicHttpAltSvcFields. If complete, calls OnAltSvcStart.
    kMaybeDecodedStruct,

    // Reports the value of the strings (origin and value) of an ALTSVC frame
    // to the listener.
    kDecodingStrings,

    // The initial decode buffer wasn't large enough for the
    // QuicHttpAltSvcFields,
    // so this state resumes the decoding when ResumeDecodingPayload is called
    // later with a new QuicHttpDecodeBuffer.
    kResumeDecodingStruct,
  };

  // Starts the decoding of a ALTSVC frame's payload, and completes it if the
  // entire payload is in the provided decode buffer.
  QuicHttpDecodeStatus StartDecodingPayload(QuicHttpFrameDecoderState* state,
                                            QuicHttpDecodeBuffer* db);

  // Resumes decoding a ALTSVC frame's payload that has been split across
  // decode buffers.
  QuicHttpDecodeStatus ResumeDecodingPayload(QuicHttpFrameDecoderState* state,
                                             QuicHttpDecodeBuffer* db);

 private:
  friend class test::QuicHttpAltSvcQuicHttpPayloadDecoderPeer;

  // Implements state kDecodingStrings.
  QuicHttpDecodeStatus DecodeStrings(QuicHttpFrameDecoderState* state,
                                     QuicHttpDecodeBuffer* db);

  QuicHttpAltSvcFields altsvc_fields_;
  PayloadState payload_state_;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_DECODER_PAYLOAD_DECODERS_QUIC_HTTP_ALTSVC_PAYLOAD_DECODER_H_
