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

#include "common/quic/http/decoder/payload_decoders/quic_http_continuation_payload_decoder.h"

#include <stddef.h>

#include <cstdint>

#include "base/logging.h"
#include "common/quic/http/decoder/quic_http_decode_buffer.h"
#include "common/quic/http/decoder/quic_http_frame_decoder_listener.h"
#include "common/quic/http/quic_http_constants.h"
#include "common/quic/http/quic_http_structures.h"

namespace gfe_quic {

QuicHttpDecodeStatus
QuicHttpContinuationQuicHttpPayloadDecoder::StartDecodingPayload(
    QuicHttpFrameDecoderState* state,
    QuicHttpDecodeBuffer* db) {
  const QuicHttpFrameHeader& frame_header = state->frame_header();
  const uint32_t total_length = frame_header.payload_length;

  DVLOG(2)
      << "QuicHttpContinuationQuicHttpPayloadDecoder::StartDecodingPayload: "
      << frame_header;
  DCHECK_EQ(QuicHttpFrameType::CONTINUATION, frame_header.type);
  DCHECK_LE(db->Remaining(), total_length);
  DCHECK_EQ(0,
            frame_header.flags & ~(QuicHttpFrameFlag::QUIC_HTTP_END_HEADERS));

  state->InitializeRemainders();
  state->listener()->OnContinuationStart(frame_header);
  return ResumeDecodingPayload(state, db);
}

QuicHttpDecodeStatus
QuicHttpContinuationQuicHttpPayloadDecoder::ResumeDecodingPayload(
    QuicHttpFrameDecoderState* state,
    QuicHttpDecodeBuffer* db) {
  DVLOG(2)
      << "QuicHttpContinuationQuicHttpPayloadDecoder::ResumeDecodingPayload"
      << "  remaining_payload=" << state->remaining_payload()
      << "  db->Remaining=" << db->Remaining();
  DCHECK_EQ(QuicHttpFrameType::CONTINUATION, state->frame_header().type);
  DCHECK_LE(state->remaining_payload(), state->frame_header().payload_length);
  DCHECK_LE(db->Remaining(), state->remaining_payload());

  size_t avail = db->Remaining();
  DCHECK_LE(avail, state->remaining_payload());
  if (avail > 0) {
    state->listener()->OnHpackFragment(db->cursor(), avail);
    db->AdvanceCursor(avail);
    state->ConsumePayload(avail);
  }
  if (state->remaining_payload() == 0) {
    state->listener()->OnContinuationEnd();
    return QuicHttpDecodeStatus::kDecodeDone;
  }
  return QuicHttpDecodeStatus::kDecodeInProgress;
}

}  // namespace gfe_quic
