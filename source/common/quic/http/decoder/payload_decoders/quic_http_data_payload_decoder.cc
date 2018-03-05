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

#include "common/quic/http/decoder/payload_decoders/quic_http_data_payload_decoder.h"

#include <stddef.h>

#include <cstdint>

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
                         QuicHttpDataQuicHttpPayloadDecoder::PayloadState v) {
  switch (v) {
    case QuicHttpDataQuicHttpPayloadDecoder::PayloadState::kReadPadLength:
      return out << "kReadPadLength";
    case QuicHttpDataQuicHttpPayloadDecoder::PayloadState::kReadPayload:
      return out << "kReadPayload";
    case QuicHttpDataQuicHttpPayloadDecoder::PayloadState::kSkipPadding:
      return out << "kSkipPadding";
  }
  // Since the value doesn't come over the wire, only a programming bug should
  // result in reaching this point.
  int unknown = static_cast<int>(v);
  QUIC_BUG << "Invalid QuicHttpDataQuicHttpPayloadDecoder::PayloadState: "
           << unknown;
  return out << "QuicHttpDataQuicHttpPayloadDecoder::PayloadState(" << unknown
             << ")";
}

QuicHttpDecodeStatus QuicHttpDataQuicHttpPayloadDecoder::StartDecodingPayload(
    QuicHttpFrameDecoderState* state,
    QuicHttpDecodeBuffer* db) {
  const QuicHttpFrameHeader& frame_header = state->frame_header();
  const uint32_t total_length = frame_header.payload_length;

  DVLOG(2) << "QuicHttpDataQuicHttpPayloadDecoder::StartDecodingPayload: "
           << frame_header;
  DCHECK_EQ(QuicHttpFrameType::DATA, frame_header.type);
  DCHECK_LE(db->Remaining(), total_length);
  DCHECK_EQ(0, frame_header.flags & ~(QuicHttpFrameFlag::QUIC_HTTP_END_STREAM |
                                      QuicHttpFrameFlag::QUIC_HTTP_PADDED));

  // Special case for the hoped for common case: unpadded and fits fully into
  // the decode buffer. TO BE SEEN if that is true. It certainly requires that
  // the transport buffers be large (e.g. >> 16KB typically).
  // TODO Add counters.
  DVLOG(2) << "StartDecodingPayload total_length=" << total_length;
  if (!frame_header.IsPadded()) {
    DVLOG(2) << "StartDecodingPayload !IsPadded";
    if (db->Remaining() == total_length) {
      DVLOG(2) << "StartDecodingPayload all present";
      // Note that we don't cache the listener field so that the callee can
      // replace it if the frame is bad.
      // If this case is common enough, consider combining the 3 callbacks
      // into one.
      state->listener()->OnDataStart(frame_header);
      if (total_length > 0) {
        state->listener()->OnDataPayload(db->cursor(), total_length);
        db->AdvanceCursor(total_length);
      }
      state->listener()->OnDataEnd();
      return QuicHttpDecodeStatus::kDecodeDone;
    }
    payload_state_ = PayloadState::kReadPayload;
  } else {
    payload_state_ = PayloadState::kReadPadLength;
  }
  state->InitializeRemainders();
  state->listener()->OnDataStart(frame_header);
  return ResumeDecodingPayload(state, db);
}

QuicHttpDecodeStatus QuicHttpDataQuicHttpPayloadDecoder::ResumeDecodingPayload(
    QuicHttpFrameDecoderState* state,
    QuicHttpDecodeBuffer* db) {
  DVLOG(2) << "QuicHttpDataQuicHttpPayloadDecoder::ResumeDecodingPayload "
              "payload_state_="
           << payload_state_;
  const QuicHttpFrameHeader& frame_header = state->frame_header();
  DCHECK_EQ(QuicHttpFrameType::DATA, frame_header.type);
  DCHECK_LE(state->remaining_payload_and_padding(),
            frame_header.payload_length);
  DCHECK_LE(db->Remaining(), state->remaining_payload_and_padding());
  QuicHttpDecodeStatus status;
  size_t avail;
  switch (payload_state_) {
    case PayloadState::kReadPadLength:
      // ReadPadLength handles the OnPadLength callback, and updating the
      // remaining_payload and remaining_padding fields. If the amount of
      // padding is too large to fit in the frame's payload, ReadPadLength
      // instead calls OnPaddingTooLong and returns kDecodeError.
      status = state->ReadPadLength(db, /*report_pad_length*/ true);
      if (status != QuicHttpDecodeStatus::kDecodeDone) {
        return status;
      }
      QUIC_FALLTHROUGH_INTENDED;

    case PayloadState::kReadPayload:
      avail = state->AvailablePayload(db);
      if (avail > 0) {
        state->listener()->OnDataPayload(db->cursor(), avail);
        db->AdvanceCursor(avail);
        state->ConsumePayload(avail);
      }
      if (state->remaining_payload() > 0) {
        payload_state_ = PayloadState::kReadPayload;
        return QuicHttpDecodeStatus::kDecodeInProgress;
      }
      QUIC_FALLTHROUGH_INTENDED;

    case PayloadState::kSkipPadding:
      // SkipPadding handles the OnPadding callback.
      if (state->SkipPadding(db)) {
        state->listener()->OnDataEnd();
        return QuicHttpDecodeStatus::kDecodeDone;
      }
      payload_state_ = PayloadState::kSkipPadding;
      return QuicHttpDecodeStatus::kDecodeInProgress;
  }
  QUIC_BUG << "PayloadState: " << payload_state_;
  return QuicHttpDecodeStatus::kDecodeError;
}

}  // namespace gfe_quic
