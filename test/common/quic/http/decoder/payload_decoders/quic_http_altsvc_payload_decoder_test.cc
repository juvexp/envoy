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

#include "common/quic/http/decoder/payload_decoders/quic_http_altsvc_payload_decoder.h"

#include <stddef.h>

#include <cstdint>

#include "base/logging.h"
#include "test/common/quic/http/decoder/payload_decoders/quic_http_payload_decoder_base_test_util.h"
#include "common/quic/http/decoder/quic_http_frame_decoder_listener.h"
#include "common/quic/http/quic_http_constants.h"
#include "test/common/quic/http/quic_http_structures_test_util.h"
#include "test/common/quic/http/test_tools/quic_http_frame_parts.h"
#include "test/common/quic/http/test_tools/quic_http_frame_parts_collector.h"
#include "test/common/quic/http/tools/quic_http_frame_builder.h"
#include "test/common/quic/http/tools/quic_http_random_decoder_test.h"
#include "common/quic/platform/api/quic_string.h"
#include "testing/base/public/gunit.h"

namespace gfe_quic {
namespace test {

// Provides friend access to an instance of the payload decoder, and also
// provides info to aid in testing.
class QuicHttpAltSvcQuicHttpPayloadDecoderPeer {
 public:
  static constexpr QuicHttpFrameType FrameType() {
    return QuicHttpFrameType::ALTSVC;
  }

  // Returns the mask of flags that affect the decoding of the payload (i.e.
  // flags that that indicate the presence of certain fields or padding).
  static constexpr uint8_t FlagsAffectingPayloadDecoding() { return 0; }

  static void Randomize(QuicHttpAltSvcQuicHttpPayloadDecoder* p,
                        QuicTestRandomBase* rng) {
    CorruptEnum(&p->payload_state_, rng);
    test::Randomize(&p->altsvc_fields_, rng);
    VLOG(1)
        << "QuicHttpAltSvcQuicHttpPayloadDecoderPeer::Randomize altsvc_fields_="
        << p->altsvc_fields_;
  }
};

namespace {

struct Listener : public QuicHttpFramePartsCollector {
  void OnAltSvcStart(const QuicHttpFrameHeader& header,
                     size_t origin_length,
                     size_t value_length) override {
    VLOG(1) << "OnAltSvcStart header: " << header
            << "; origin_length=" << origin_length
            << "; value_length=" << value_length;
    StartFrame(header)->OnAltSvcStart(header, origin_length, value_length);
  }

  void OnAltSvcOriginData(const char* data, size_t len) override {
    VLOG(1) << "OnAltSvcOriginData: len=" << len;
    CurrentFrame()->OnAltSvcOriginData(data, len);
  }

  void OnAltSvcValueData(const char* data, size_t len) override {
    VLOG(1) << "OnAltSvcValueData: len=" << len;
    CurrentFrame()->OnAltSvcValueData(data, len);
  }

  void OnAltSvcEnd() override {
    VLOG(1) << "OnAltSvcEnd";
    EndFrame()->OnAltSvcEnd();
  }

  void OnFrameSizeError(const QuicHttpFrameHeader& header) override {
    VLOG(1) << "OnFrameSizeError: " << header;
    FrameError(header)->OnFrameSizeError(header);
  }
};

class QuicHttpAltSvcQuicHttpPayloadDecoderTest
    : public AbstractQuicHttpPayloadDecoderTest<
          QuicHttpAltSvcQuicHttpPayloadDecoder,
          QuicHttpAltSvcQuicHttpPayloadDecoderPeer,
          Listener> {};

// Confirm we get an error if the payload is not long enough to hold
// QuicHttpAltSvcFields and the indicated length of origin.
TEST_F(QuicHttpAltSvcQuicHttpPayloadDecoderTest, Truncated) {
  QuicHttpFrameBuilder fb;
  fb.Append(
      QuicHttpAltSvcFields{0xffff});  // The longest possible origin length.
  fb.Append("Too little origin!");
  EXPECT_TRUE(
      VerifyDetectsFrameSizeError(0, fb.buffer(), /*approve_size*/ nullptr));
}

class AltSvcPayloadLengthTests
    : public QuicHttpAltSvcQuicHttpPayloadDecoderTest,
      public ::testing::WithParamInterface<
          ::testing::tuple<uint16_t, uint32_t>> {
 protected:
  AltSvcPayloadLengthTests()
      : origin_length_(::testing::get<0>(GetParam())),
        value_length_(::testing::get<1>(GetParam())) {
    VLOG(1) << "################  origin_length_=" << origin_length_
            << "   value_length_=" << value_length_ << "  ################";
    // To increase coverage of the D?VLOG's, if the lengths are both 3, then
    // set --v=4. The reason for choosing length=3 is that when decoding a
    // string 1 byte at a time, there will be a chunk at the start of the
    // string, a chunk in the middle, and a chunk at the end, thus exercising
    // handling of all 3 of those situations.
    if (origin_length_ == 3 && value_length_ == 3) {
      BoostLoggingIfDefault(4);
    }
  }

  const uint16_t origin_length_;
  const uint32_t value_length_;
};

INSTANTIATE_TEST_CASE_P(VariousOriginAndValueLengths,
                        AltSvcPayloadLengthTests,
                        ::testing::Combine(::testing::Values(0, 1, 3, 65535),
                                           ::testing::Values(0, 1, 3, 65537)));

TEST_P(AltSvcPayloadLengthTests, ValidOriginAndValueLength) {
  QuicString origin = Random().RandString(origin_length_);
  QuicString value = Random().RandString(value_length_);
  QuicHttpFrameBuilder fb;
  fb.Append(QuicHttpAltSvcFields{origin_length_});
  fb.Append(origin);
  fb.Append(value);
  QuicHttpFrameHeader header(fb.size(), QuicHttpFrameType::ALTSVC, RandFlags(),
                             RandStreamId());
  set_frame_header(header);
  QuicHttpFrameParts expected(header);
  expected.SetAltSvcExpected(origin, value);
  ASSERT_TRUE(DecodePayloadAndValidateSeveralWays(fb.buffer(), expected));
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
