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
#include "test/common/quic/http/decoder/payload_decoders/quic_http_payload_decoder_base_test_util.h"
#include "common/quic/http/decoder/quic_http_frame_decoder_listener.h"
#include "common/quic/http/quic_http_constants.h"
#include "common/quic/http/quic_http_structures.h"
#include "test/common/quic/http/quic_http_structures_test_util.h"
#include "test/common/quic/http/test_tools/quic_http_frame_parts.h"
#include "test/common/quic/http/test_tools/quic_http_frame_parts_collector.h"
#include "test/common/quic/http/tools/quic_http_frame_builder.h"
#include "test/common/quic/http/tools/quic_http_random_decoder_test.h"
#include "common/quic/platform/api/quic_string.h"
#include "testing/base/public/gunit.h"

using ::testing::AssertionResult;

namespace gfe_quic {
namespace test {

// Provides friend access to an instance of the payload decoder, and also
// provides info to aid in testing.
class QuicHttpDataQuicHttpPayloadDecoderPeer {
 public:
  static constexpr QuicHttpFrameType FrameType() {
    return QuicHttpFrameType::DATA;
  }

  // Returns the mask of flags that affect the decoding of the payload (i.e.
  // flags that that indicate the presence of certain fields or padding).
  static constexpr uint8_t FlagsAffectingPayloadDecoding() {
    return QuicHttpFrameFlag::QUIC_HTTP_PADDED;
  }

  static void Randomize(QuicHttpDataQuicHttpPayloadDecoder* p,
                        QuicTestRandomBase* rng) {
    VLOG(1) << "QuicHttpDataQuicHttpPayloadDecoderPeer::Randomize";
    CorruptEnum(&p->payload_state_, rng);
  }
};

namespace {

struct Listener : public QuicHttpFramePartsCollector {
  void OnDataStart(const QuicHttpFrameHeader& header) override {
    VLOG(1) << "OnDataStart: " << header;
    StartFrame(header)->OnDataStart(header);
  }

  void OnDataPayload(const char* data, size_t len) override {
    VLOG(1) << "OnDataPayload: len=" << len;
    CurrentFrame()->OnDataPayload(data, len);
  }

  void OnDataEnd() override {
    VLOG(1) << "OnDataEnd";
    EndFrame()->OnDataEnd();
  }

  void OnPadLength(size_t pad_length) override {
    VLOG(1) << "OnPadLength: " << pad_length;
    CurrentFrame()->OnPadLength(pad_length);
  }

  void OnPadding(const char* padding, size_t skipped_length) override {
    VLOG(1) << "OnPadding: " << skipped_length;
    CurrentFrame()->OnPadding(padding, skipped_length);
  }

  void OnPaddingTooLong(const QuicHttpFrameHeader& header,
                        size_t missing_length) override {
    VLOG(1) << "OnPaddingTooLong: " << header
            << "    missing_length: " << missing_length;
    EndFrame()->OnPaddingTooLong(header, missing_length);
  }
};

class QuicHttpDataQuicHttpPayloadDecoderTest
    : public AbstractPaddableQuicHttpPayloadDecoderTest<
          QuicHttpDataQuicHttpPayloadDecoder,
          QuicHttpDataQuicHttpPayloadDecoderPeer,
          Listener> {
 protected:
  AssertionResult CreateAndDecodeDataOfSize(size_t data_size) {
    Reset();
    // Boost logging when the data and *trailing* padding are both 3 bytes long,
    // as this size of a string has both a beginning, middle and end when
    // segmented into 1 byte long buffers, so should exercise all the paths.
    // But we don't want to do extra logging all the time as it is expensive.
    const bool do_boost = data_size == 3 && total_pad_length_ == 4;
    RestoreVerbosity rv(BoostLoggingIfDefault(do_boost ? 4 : 0));
    uint8_t flags = RandFlags();

    QuicString data_payload = Random().RandString(data_size);
    frame_builder_.Append(data_payload);
    MaybeAppendTrailingPadding();

    QuicHttpFrameHeader frame_header(
        frame_builder_.size(), QuicHttpFrameType::DATA, flags, RandStreamId());
    set_frame_header(frame_header);
    ScrubFlagsOfHeader(&frame_header);
    QuicHttpFrameParts expected(frame_header, data_payload, total_pad_length_);
    VERIFY_AND_RETURN_SUCCESS(
        DecodePayloadAndValidateSeveralWays(frame_builder_.buffer(), expected));
  }
};

INSTANTIATE_TEST_CASE_P(VariousPadLengths,
                        QuicHttpDataQuicHttpPayloadDecoderTest,
                        ::testing::Values(0, 1, 2, 3, 4, 254, 255, 256));

TEST_P(QuicHttpDataQuicHttpPayloadDecoderTest, VariousDataPayloadSizes) {
  for (size_t data_size : {0, 1, 2, 3, 255, 256, 1024}) {
    EXPECT_TRUE(CreateAndDecodeDataOfSize(data_size));
  }
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
