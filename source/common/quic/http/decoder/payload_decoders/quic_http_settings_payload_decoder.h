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

#ifndef GFE_QUIC_HTTP_DECODER_PAYLOAD_DECODERS_QUIC_HTTP_SETTINGS_PAYLOAD_DECODER_H_
#define GFE_QUIC_HTTP_DECODER_PAYLOAD_DECODERS_QUIC_HTTP_SETTINGS_PAYLOAD_DECODER_H_

// Decodes the payload of a SETTINGS frame; for the RFC, see:
//     http://httpwg.org/specs/rfc7540.html#SETTINGS
//
// For info about payload decoders, see:
//     http://g3doc/gfe/quic/http/decoder/payload_decoders/README.md

#include "common/quic/http/decoder/quic_http_decode_buffer.h"
#include "common/quic/http/decoder/quic_http_decode_status.h"
#include "common/quic/http/decoder/quic_http_frame_decoder_state.h"
#include "common/quic/http/quic_http_structures.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {
namespace test {
class QuicHttpQuicHttpSettingsQuicHttpPayloadDecoderPeer;
}  // namespace test

class QUIC_EXPORT_PRIVATE QuicHttpQuicHttpSettingsQuicHttpPayloadDecoder {
 public:
  // Starts the decoding of a SETTINGS frame's payload, and completes it if
  // the entire payload is in the provided decode buffer.
  QuicHttpDecodeStatus StartDecodingPayload(QuicHttpFrameDecoderState* state,
                                            QuicHttpDecodeBuffer* db);

  // Resumes decoding a SETTINGS frame that has been split across decode
  // buffers.
  QuicHttpDecodeStatus ResumeDecodingPayload(QuicHttpFrameDecoderState* state,
                                             QuicHttpDecodeBuffer* db);

 private:
  friend class test::QuicHttpQuicHttpSettingsQuicHttpPayloadDecoderPeer;

  // Decodes as many settings as are available in the decode buffer, starting at
  // the first byte of one setting; if a single setting is split across buffers,
  // ResumeDecodingPayload will handle starting from where the previous call
  // left off, and then will call StartDecodingSettings.
  QuicHttpDecodeStatus StartDecodingSettings(QuicHttpFrameDecoderState* state,
                                             QuicHttpDecodeBuffer* db);

  // Decoding a single SETTING returned a status other than kDecodeDone; this
  // method just brings together the DCHECKs to reduce duplication.
  QuicHttpDecodeStatus HandleNotDone(QuicHttpFrameDecoderState* state,
                                     QuicHttpDecodeBuffer* db,
                                     QuicHttpDecodeStatus status);

  QuicHttpSettingFields setting_fields_;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_DECODER_PAYLOAD_DECODERS_QUIC_HTTP_SETTINGS_PAYLOAD_DECODER_H_
