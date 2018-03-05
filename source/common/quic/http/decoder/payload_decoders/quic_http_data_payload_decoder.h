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

#ifndef GFE_QUIC_HTTP_DECODER_PAYLOAD_DECODERS_QUIC_HTTP_DATA_PAYLOAD_DECODER_H_
#define GFE_QUIC_HTTP_DECODER_PAYLOAD_DECODERS_QUIC_HTTP_DATA_PAYLOAD_DECODER_H_

// Decodes the payload of a DATA frame.
// See http://g3doc/gfe/quic/http/decoder/payload_decoders/README.md for info
// about payload decoders.

#include "common/quic/http/decoder/quic_http_decode_buffer.h"
#include "common/quic/http/decoder/quic_http_decode_status.h"
#include "common/quic/http/decoder/quic_http_frame_decoder_state.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {
namespace test {
class QuicHttpDataQuicHttpPayloadDecoderPeer;
}  // namespace test

class QUIC_EXPORT_PRIVATE QuicHttpDataQuicHttpPayloadDecoder {
 public:
  // States during decoding of a DATA frame.
  enum class PayloadState {
    // The frame is padded and we need to read the PAD_LENGTH field (1 byte),
    // and then call OnPadLength
    kReadPadLength,

    // Report the non-padding portion of the payload to the listener's
    // OnDataPayload method.
    kReadPayload,

    // The decoder has finished with the non-padding portion of the payload,
    // and is now ready to skip the trailing padding, if the frame has any.
    kSkipPadding,
  };

  // Starts decoding a DATA frame's payload, and completes it if
  // the entire payload is in the provided decode buffer.
  QuicHttpDecodeStatus StartDecodingPayload(QuicHttpFrameDecoderState* state,
                                            QuicHttpDecodeBuffer* db);

  // Resumes decoding a DATA frame's payload that has been split across
  // decode buffers.
  QuicHttpDecodeStatus ResumeDecodingPayload(QuicHttpFrameDecoderState* state,
                                             QuicHttpDecodeBuffer* db);

 private:
  friend class test::QuicHttpDataQuicHttpPayloadDecoderPeer;

  PayloadState payload_state_;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_DECODER_PAYLOAD_DECODERS_QUIC_HTTP_DATA_PAYLOAD_DECODER_H_
