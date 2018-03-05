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

#include "common/quic/http/decoder/payload_decoders/quic_http_priority_payload_decoder.h"

#include "base/logging.h"
#include "common/quic/http/decoder/quic_http_decode_buffer.h"
#include "common/quic/http/decoder/quic_http_frame_decoder_listener.h"
#include "common/quic/http/quic_http_constants.h"
#include "common/quic/http/quic_http_structures.h"

namespace gfe_quic {

QuicHttpDecodeStatus
QuicHttpPriorityQuicHttpPayloadDecoder::StartDecodingPayload(
    QuicHttpFrameDecoderState* state,
    QuicHttpDecodeBuffer* db) {
  DVLOG(2) << "QuicHttpPriorityQuicHttpPayloadDecoder::StartDecodingPayload: "
           << state->frame_header();
  DCHECK_EQ(QuicHttpFrameType::QUIC_HTTP_PRIORITY, state->frame_header().type);
  DCHECK_LE(db->Remaining(), state->frame_header().payload_length);
  // QUIC_HTTP_PRIORITY frames have no flags.
  DCHECK_EQ(0, state->frame_header().flags);
  state->InitializeRemainders();
  return HandleStatus(
      state, state->StartDecodingStructureInPayload(&priority_fields_, db));
}

QuicHttpDecodeStatus
QuicHttpPriorityQuicHttpPayloadDecoder::ResumeDecodingPayload(
    QuicHttpFrameDecoderState* state,
    QuicHttpDecodeBuffer* db) {
  DVLOG(2) << "QuicHttpPriorityQuicHttpPayloadDecoder::ResumeDecodingPayload"
           << "  remaining_payload=" << state->remaining_payload()
           << "  db->Remaining=" << db->Remaining();
  DCHECK_EQ(QuicHttpFrameType::QUIC_HTTP_PRIORITY, state->frame_header().type);
  DCHECK_LE(db->Remaining(), state->frame_header().payload_length);
  return HandleStatus(
      state, state->ResumeDecodingStructureInPayload(&priority_fields_, db));
}

QuicHttpDecodeStatus QuicHttpPriorityQuicHttpPayloadDecoder::HandleStatus(
    QuicHttpFrameDecoderState* state,
    QuicHttpDecodeStatus status) {
  if (status == QuicHttpDecodeStatus::kDecodeDone) {
    if (state->remaining_payload() == 0) {
      state->listener()->OnPriorityFrame(state->frame_header(),
                                         priority_fields_);
      return QuicHttpDecodeStatus::kDecodeDone;
    }
    // Payload is too long.
    return state->ReportFrameSizeError();
  }
  // Not done decoding the structure. Either we've got more payload to decode,
  // or we've run out because the payload is too short, in which case
  // OnFrameSizeError will have already been called.
  DCHECK((status == QuicHttpDecodeStatus::kDecodeInProgress &&
          state->remaining_payload() > 0) ||
         (status == QuicHttpDecodeStatus::kDecodeError &&
          state->remaining_payload() == 0))
      << "\n status=" << status
      << "; remaining_payload=" << state->remaining_payload();
  return status;
}

}  // namespace gfe_quic
