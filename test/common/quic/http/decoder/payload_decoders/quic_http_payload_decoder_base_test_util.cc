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

#include "test/common/quic/http/decoder/payload_decoders/quic_http_payload_decoder_base_test_util.h"

#include "base/logging.h"
#include "common/quic/http/decoder/quic_http_decode_buffer.h"
#include "common/quic/http/decoder/quic_http_decode_status.h"
#include "test/common/quic/http/decoder/quic_http_frame_decoder_state_test_util.h"
#include "common/quic/http/quic_http_structures.h"
#include "test/common/quic/http/quic_http_structures_test_util.h"
#include "test/common/quic/http/tools/quic_http_random_decoder_test.h"
#include "gtest/gtest.h"

namespace gfe_quic {
namespace test {
QuicHttpPayloadDecoderBaseTest::QuicHttpPayloadDecoderBaseTest() {
  // If the test adds more data after the frame payload,
  // stop as soon as the payload is decoded.
  stop_decode_on_done_ = true;
  frame_header_is_set_ = false;
  Randomize(&frame_header_, RandomPtr());
}

QuicHttpDecodeStatus QuicHttpPayloadDecoderBaseTest::StartDecoding(
    QuicHttpDecodeBuffer* db) {
  DVLOG(2) << "StartDecoding, db->Remaining=" << db->Remaining();
  // Make sure sub-class has set frame_header_ so that we can inject it
  // into the payload decoder below.
  if (!frame_header_is_set_) {
    ADD_FAILURE() << "frame_header_ is not set";
    return QuicHttpDecodeStatus::kDecodeError;
  }
  // The contract with the payload decoders is that they won't receive a
  // decode buffer that extends beyond the end of the frame.
  if (db->Remaining() > frame_header_.payload_length) {
    ADD_FAILURE() << "QuicHttpDecodeBuffer has too much data: "
                  << db->Remaining() << " > " << frame_header_.payload_length;
    return QuicHttpDecodeStatus::kDecodeError;
  }

  // Prepare the payload decoder.
  PrepareQuicHttpPayloadDecoder();

  // Reconstruct the QuicHttpFrameDecoderState, prepare the listener, and add it
  // to the QuicHttpFrameDecoderState.
  QuicReconstructObject(&frame_decoder_state_, RandomPtr());
  frame_decoder_state_.set_listener(PrepareListener());

  // Make sure that a listener was provided.
  if (frame_decoder_state_.listener() == nullptr) {
    ADD_FAILURE() << "PrepareListener must return a listener.";
    return QuicHttpDecodeStatus::kDecodeError;
  }

  // Now that nothing in the payload decoder should be valid, inject the
  // QuicHttpFrameHeader whose payload we're about to decode. That header is the
  // only state that a payload decoder should expect is valid when its Start
  // method is called.
  QuicHttpFrameDecoderStatePeer::set_frame_header(frame_header_,
                                                  &frame_decoder_state_);
  QuicHttpDecodeStatus status = StartDecodingPayload(db);
  if (status != QuicHttpDecodeStatus::kDecodeInProgress) {
    // Keep track of this so that a concrete test can verify that both fast
    // and slow decoding paths have been tested.
    ++fast_decode_count_;
  }
  return status;
}

QuicHttpDecodeStatus QuicHttpPayloadDecoderBaseTest::ResumeDecoding(
    QuicHttpDecodeBuffer* db) {
  DVLOG(2) << "ResumeDecoding, db->Remaining=" << db->Remaining();
  QuicHttpDecodeStatus status = ResumeDecodingPayload(db);
  if (status != QuicHttpDecodeStatus::kDecodeInProgress) {
    // Keep track of this so that a concrete test can verify that both fast
    // and slow decoding paths have been tested.
    ++slow_decode_count_;
  }
  return status;
}

::testing::AssertionResult
QuicHttpPayloadDecoderBaseTest::DecodePayloadAndValidateSeveralWays(
    QuicStringPiece payload,
    const Validator& validator) {
  VERIFY_TRUE(frame_header_is_set_);
  // Cap the payload to be decoded at the declared payload length. This is
  // required by the decoders' preconditions; they are designed on the
  // assumption that they're never passed more than they're permitted to
  // consume.
  // Note that it is OK if the payload is too short; the validator may be
  // designed to check for that.
  if (payload.size() > frame_header_.payload_length) {
    payload = QuicStringPiece(payload.data(), frame_header_.payload_length);
  }
  QuicHttpDecodeBuffer db(payload);
  ResetDecodeSpeedCounters();
  const bool kMayReturnZeroOnFirst = false;
  return DecodeAndValidateSeveralWays(&db, kMayReturnZeroOnFirst, validator);
}

}  // namespace test
}  // namespace gfe_quic
