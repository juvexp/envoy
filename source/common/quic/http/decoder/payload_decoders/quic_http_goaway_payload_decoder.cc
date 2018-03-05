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

#include "common/quic/http/decoder/payload_decoders/quic_http_goaway_payload_decoder.h"

#include <stddef.h>

#include "base/logging.h"
#include "base/macros.h"
#include "common/quic/http/decoder/quic_http_decode_buffer.h"
#include "common/quic/http/decoder/quic_http_frame_decoder_listener.h"
#include "common/quic/http/quic_http_constants.h"
#include "common/quic/http/quic_http_structures.h"
#include "common/quic/platform/api/quic_bug_tracker.h"
#include "common/quic/platform/api/quic_fallthrough.h"

namespace gfe_quic {

std::ostream& operator<<(std::ostream& out,
                         QuicHttpGoAwayQuicHttpPayloadDecoder::PayloadState v) {
  switch (v) {
    case QuicHttpGoAwayQuicHttpPayloadDecoder::PayloadState::
        kStartDecodingFixedFields:
      return out << "kStartDecodingFixedFields";
    case QuicHttpGoAwayQuicHttpPayloadDecoder::PayloadState::
        kHandleFixedFieldsStatus:
      return out << "kHandleFixedFieldsStatus";
    case QuicHttpGoAwayQuicHttpPayloadDecoder::PayloadState::kReadOpaqueData:
      return out << "kReadOpaqueData";
    case QuicHttpGoAwayQuicHttpPayloadDecoder::PayloadState::
        kResumeDecodingFixedFields:
      return out << "kResumeDecodingFixedFields";
  }
  // Since the value doesn't come over the wire, only a programming bug should
  // result in reaching this point.
  int unknown = static_cast<int>(v);
  QUIC_BUG << "Invalid QuicHttpGoAwayQuicHttpPayloadDecoder::PayloadState: "
           << unknown;
  return out << "QuicHttpGoAwayQuicHttpPayloadDecoder::PayloadState(" << unknown
             << ")";
}

QuicHttpDecodeStatus QuicHttpGoAwayQuicHttpPayloadDecoder::StartDecodingPayload(
    QuicHttpFrameDecoderState* state,
    QuicHttpDecodeBuffer* db) {
  DVLOG(2) << "QuicHttpGoAwayQuicHttpPayloadDecoder::StartDecodingPayload: "
           << state->frame_header();
  DCHECK_EQ(QuicHttpFrameType::GOAWAY, state->frame_header().type);
  DCHECK_LE(db->Remaining(), state->frame_header().payload_length);
  DCHECK_EQ(0, state->frame_header().flags);

  state->InitializeRemainders();
  payload_state_ = PayloadState::kStartDecodingFixedFields;
  return ResumeDecodingPayload(state, db);
}

QuicHttpDecodeStatus
QuicHttpGoAwayQuicHttpPayloadDecoder::ResumeDecodingPayload(
    QuicHttpFrameDecoderState* state,
    QuicHttpDecodeBuffer* db) {
  DVLOG(2) << "QuicHttpGoAwayQuicHttpPayloadDecoder::ResumeDecodingPayload: "
              "remaining_payload="
           << state->remaining_payload()
           << ", db->Remaining=" << db->Remaining();

  const QuicHttpFrameHeader& frame_header = state->frame_header();
  DCHECK_EQ(QuicHttpFrameType::GOAWAY, frame_header.type);
  DCHECK_LE(db->Remaining(), frame_header.payload_length);
  DCHECK_NE(PayloadState::kHandleFixedFieldsStatus, payload_state_);

  QuicHttpDecodeStatus status = QuicHttpDecodeStatus::kDecodeError;

  size_t avail;
  while (true) {
    DVLOG(2) << "QuicHttpGoAwayQuicHttpPayloadDecoder::ResumeDecodingPayload "
                "payload_state_="
             << payload_state_;
    switch (payload_state_) {
      case PayloadState::kStartDecodingFixedFields:
        status = state->StartDecodingStructureInPayload(&goaway_fields_, db);
        QUIC_FALLTHROUGH_INTENDED;

      case PayloadState::kHandleFixedFieldsStatus:
        if (status == QuicHttpDecodeStatus::kDecodeDone) {
          state->listener()->OnGoAwayStart(frame_header, goaway_fields_);
        } else {
          // Not done decoding the structure. Either we've got more payload
          // to decode, or we've run out because the payload is too short,
          // in which case OnFrameSizeError will have already been called.
          DCHECK((status == QuicHttpDecodeStatus::kDecodeInProgress &&
                  state->remaining_payload() > 0) ||
                 (status == QuicHttpDecodeStatus::kDecodeError &&
                  state->remaining_payload() == 0))
              << "\n status=" << status
              << "; remaining_payload=" << state->remaining_payload();
          payload_state_ = PayloadState::kResumeDecodingFixedFields;
          return status;
        }
        QUIC_FALLTHROUGH_INTENDED;

      case PayloadState::kReadOpaqueData:
        // The opaque data is all the remains to be decoded, so anything left
        // in the decode buffer is opaque data.
        avail = db->Remaining();
        if (avail > 0) {
          state->listener()->OnGoAwayOpaqueData(db->cursor(), avail);
          db->AdvanceCursor(avail);
          state->ConsumePayload(avail);
        }
        if (state->remaining_payload() > 0) {
          payload_state_ = PayloadState::kReadOpaqueData;
          return QuicHttpDecodeStatus::kDecodeInProgress;
        }
        state->listener()->OnGoAwayEnd();
        return QuicHttpDecodeStatus::kDecodeDone;

      case PayloadState::kResumeDecodingFixedFields:
        status = state->ResumeDecodingStructureInPayload(&goaway_fields_, db);
        payload_state_ = PayloadState::kHandleFixedFieldsStatus;
        continue;
    }
    QUIC_BUG << "PayloadState: " << payload_state_;
  }
}

}  // namespace gfe_quic
