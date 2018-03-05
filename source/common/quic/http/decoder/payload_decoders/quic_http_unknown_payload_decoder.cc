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

#include "common/quic/http/decoder/payload_decoders/quic_http_unknown_payload_decoder.h"

#include <stddef.h>

#include "base/logging.h"
#include "common/quic/http/decoder/quic_http_decode_buffer.h"
#include "common/quic/http/decoder/quic_http_frame_decoder_listener.h"
#include "common/quic/http/quic_http_constants.h"
#include "common/quic/http/quic_http_structures.h"

namespace gfe_quic {

QuicHttpDecodeStatus
QuicHttpUnknownQuicHttpPayloadDecoder::StartDecodingPayload(
    QuicHttpFrameDecoderState* state,
    QuicHttpDecodeBuffer* db) {
  const QuicHttpFrameHeader& frame_header = state->frame_header();

  DVLOG(2) << "QuicHttpUnknownQuicHttpPayloadDecoder::StartDecodingPayload: "
           << frame_header;
  DCHECK(!IsSupportedQuicHttpFrameType(frame_header.type)) << frame_header;
  DCHECK_LE(db->Remaining(), frame_header.payload_length);

  state->InitializeRemainders();
  state->listener()->OnUnknownStart(frame_header);
  return ResumeDecodingPayload(state, db);
}

QuicHttpDecodeStatus
QuicHttpUnknownQuicHttpPayloadDecoder::ResumeDecodingPayload(
    QuicHttpFrameDecoderState* state,
    QuicHttpDecodeBuffer* db) {
  DVLOG(2) << "QuicHttpUnknownQuicHttpPayloadDecoder::ResumeDecodingPayload "
           << "remaining_payload=" << state->remaining_payload()
           << "; db->Remaining=" << db->Remaining();
  DCHECK(!IsSupportedQuicHttpFrameType(state->frame_header().type))
      << state->frame_header();
  DCHECK_LE(state->remaining_payload(), state->frame_header().payload_length);
  DCHECK_LE(db->Remaining(), state->remaining_payload());

  size_t avail = db->Remaining();
  if (avail > 0) {
    state->listener()->OnUnknownPayload(db->cursor(), avail);
    db->AdvanceCursor(avail);
    state->ConsumePayload(avail);
  }
  if (state->remaining_payload() == 0) {
    state->listener()->OnUnknownEnd();
    return QuicHttpDecodeStatus::kDecodeDone;
  }
  return QuicHttpDecodeStatus::kDecodeInProgress;
}

}  // namespace gfe_quic
