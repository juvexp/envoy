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

#include "common/quic/http/decoder/payload_decoders/quic_http_settings_payload_decoder.h"

#include <cstdint>

#include "base/logging.h"
#include "common/quic/http/decoder/quic_http_decode_buffer.h"
#include "common/quic/http/decoder/quic_http_frame_decoder_listener.h"
#include "common/quic/http/quic_http_constants.h"
#include "common/quic/http/quic_http_structures.h"

namespace gfe_quic {

QuicHttpDecodeStatus
QuicHttpQuicHttpSettingsQuicHttpPayloadDecoder::StartDecodingPayload(
    QuicHttpFrameDecoderState* state,
    QuicHttpDecodeBuffer* db) {
  const QuicHttpFrameHeader& frame_header = state->frame_header();
  const uint32_t total_length = frame_header.payload_length;

  DVLOG(2) << "QuicHttpQuicHttpSettingsQuicHttpPayloadDecoder::"
              "StartDecodingPayload: "
           << frame_header;
  DCHECK_EQ(QuicHttpFrameType::SETTINGS, frame_header.type);
  DCHECK_LE(db->Remaining(), total_length);
  DCHECK_EQ(0, frame_header.flags & ~(QuicHttpFrameFlag::QUIC_HTTP_ACK));

  if (frame_header.IsAck()) {
    if (total_length == 0) {
      state->listener()->OnSettingsAck(frame_header);
      return QuicHttpDecodeStatus::kDecodeDone;
    } else {
      state->InitializeRemainders();
      return state->ReportFrameSizeError();
    }
  } else {
    state->InitializeRemainders();
    state->listener()->OnSettingsStart(frame_header);
    return StartDecodingSettings(state, db);
  }
}

QuicHttpDecodeStatus
QuicHttpQuicHttpSettingsQuicHttpPayloadDecoder::ResumeDecodingPayload(
    QuicHttpFrameDecoderState* state,
    QuicHttpDecodeBuffer* db) {
  DVLOG(2)
      << "QuicHttpQuicHttpSettingsQuicHttpPayloadDecoder::ResumeDecodingPayload"
      << "  remaining_payload=" << state->remaining_payload()
      << "  db->Remaining=" << db->Remaining();
  DCHECK_EQ(QuicHttpFrameType::SETTINGS, state->frame_header().type);
  DCHECK_LE(db->Remaining(), state->frame_header().payload_length);

  QuicHttpDecodeStatus status =
      state->ResumeDecodingStructureInPayload(&setting_fields_, db);
  if (status == QuicHttpDecodeStatus::kDecodeDone) {
    state->listener()->OnSetting(setting_fields_);
    return StartDecodingSettings(state, db);
  }
  return HandleNotDone(state, db, status);
}

QuicHttpDecodeStatus
QuicHttpQuicHttpSettingsQuicHttpPayloadDecoder::StartDecodingSettings(
    QuicHttpFrameDecoderState* state,
    QuicHttpDecodeBuffer* db) {
  DVLOG(2)
      << "QuicHttpQuicHttpSettingsQuicHttpPayloadDecoder::StartDecodingSettings"
      << "  remaining_payload=" << state->remaining_payload()
      << "  db->Remaining=" << db->Remaining();
  while (state->remaining_payload() > 0) {
    QuicHttpDecodeStatus status =
        state->StartDecodingStructureInPayload(&setting_fields_, db);
    if (status == QuicHttpDecodeStatus::kDecodeDone) {
      state->listener()->OnSetting(setting_fields_);
      continue;
    }
    return HandleNotDone(state, db, status);
  }
  DVLOG(2) << "LEAVING "
              "QuicHttpQuicHttpSettingsQuicHttpPayloadDecoder::"
              "StartDecodingSettings"
           << "\n\tdb->Remaining=" << db->Remaining()
           << "\n\t remaining_payload=" << state->remaining_payload();
  state->listener()->OnSettingsEnd();
  return QuicHttpDecodeStatus::kDecodeDone;
}

QuicHttpDecodeStatus
QuicHttpQuicHttpSettingsQuicHttpPayloadDecoder::HandleNotDone(
    QuicHttpFrameDecoderState* state,
    QuicHttpDecodeBuffer* db,
    QuicHttpDecodeStatus status) {
  // Not done decoding the structure. Either we've got more payload to decode,
  // or we've run out because the payload is too short, in which case
  // OnFrameSizeError will have already been called.
  DCHECK((status == QuicHttpDecodeStatus::kDecodeInProgress &&
          state->remaining_payload() > 0) ||
         (status == QuicHttpDecodeStatus::kDecodeError &&
          state->remaining_payload() == 0))
      << "\n status=" << status
      << "; remaining_payload=" << state->remaining_payload()
      << "; db->Remaining=" << db->Remaining();
  return status;
}

}  // namespace gfe_quic
