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
#include "gtest/gtest.h"

namespace gfe_quic {
namespace test {

class QuicHttpGoAwayQuicHttpPayloadDecoderPeer {
 public:
  static constexpr QuicHttpFrameType FrameType() {
    return QuicHttpFrameType::GOAWAY;
  }

  // Returns the mask of flags that affect the decoding of the payload (i.e.
  // flags that that indicate the presence of certain fields or padding).
  static constexpr uint8_t FlagsAffectingPayloadDecoding() { return 0; }

  static void Randomize(QuicHttpGoAwayQuicHttpPayloadDecoder* p,
                        QuicTestRandomBase* rng) {
    CorruptEnum(&p->payload_state_, rng);
    test::Randomize(&p->goaway_fields_, rng);
    VLOG(1)
        << "QuicHttpGoAwayQuicHttpPayloadDecoderPeer::Randomize goaway_fields: "
        << p->goaway_fields_;
  }
};

namespace {

struct Listener : public QuicHttpFramePartsCollector {
  void OnGoAwayStart(const QuicHttpFrameHeader& header,
                     const QuicHttpGoAwayFields& goaway) override {
    VLOG(1) << "OnGoAwayStart header: " << header << "; goaway: " << goaway;
    StartFrame(header)->OnGoAwayStart(header, goaway);
  }

  void OnGoAwayOpaqueData(const char* data, size_t len) override {
    VLOG(1) << "OnGoAwayOpaqueData: len=" << len;
    CurrentFrame()->OnGoAwayOpaqueData(data, len);
  }

  void OnGoAwayEnd() override {
    VLOG(1) << "OnGoAwayEnd";
    EndFrame()->OnGoAwayEnd();
  }

  void OnFrameSizeError(const QuicHttpFrameHeader& header) override {
    VLOG(1) << "OnFrameSizeError: " << header;
    FrameError(header)->OnFrameSizeError(header);
  }
};

class QuicHttpGoAwayQuicHttpPayloadDecoderTest
    : public AbstractQuicHttpPayloadDecoderTest<
          QuicHttpGoAwayQuicHttpPayloadDecoder,
          QuicHttpGoAwayQuicHttpPayloadDecoderPeer,
          Listener> {};

// Confirm we get an error if the payload is not long enough to hold
// QuicHttpGoAwayFields.
TEST_F(QuicHttpGoAwayQuicHttpPayloadDecoderTest, Truncated) {
  auto approve_size = [](size_t size) {
    return size != QuicHttpGoAwayFields::EncodedSize();
  };
  QuicHttpFrameBuilder fb;
  fb.Append(QuicHttpGoAwayFields(123, QuicHttpErrorCode::ENHANCE_YOUR_CALM));
  EXPECT_TRUE(VerifyDetectsFrameSizeError(0, fb.buffer(), approve_size));
}

class QuicHttpGoAwayOpaqueDataLengthTests
    : public QuicHttpGoAwayQuicHttpPayloadDecoderTest,
      public ::testing::WithParamInterface<uint32_t> {
 protected:
  QuicHttpGoAwayOpaqueDataLengthTests() : length_(GetParam()) {
    // To increase coverage of the D?VLOG's, if the length is 3, then boost
    // verbosity. The reason for choosing length=3 is that when decoding a
    // string 1 byte at a time, there will be a chunk at the start of the
    // string, a chunk in the middle, and a chunk at the end, thus exercising
    // handling of all 3 of those situations.
    if (length_ == 3) {
      BoostLoggingIfDefault(4);
    }
    VLOG(1) << "################  length_=" << length_ << "  ################";
  }

  const uint32_t length_;
};

INSTANTIATE_TEST_CASE_P(VariousLengths,
                        QuicHttpGoAwayOpaqueDataLengthTests,
                        ::testing::Values(0, 1, 2, 3, 4, 5, 6));

TEST_P(QuicHttpGoAwayOpaqueDataLengthTests, ValidLength) {
  QuicHttpGoAwayFields goaway;
  Randomize(&goaway, RandomPtr());
  QuicString opaque_bytes = Random().RandString(length_);
  QuicHttpFrameBuilder fb;
  fb.Append(goaway);
  fb.Append(opaque_bytes);
  QuicHttpFrameHeader header(fb.size(), QuicHttpFrameType::GOAWAY, RandFlags(),
                             RandStreamId());
  set_frame_header(header);
  QuicHttpFrameParts expected(header, opaque_bytes);
  expected.opt_goaway = goaway;
  ASSERT_TRUE(DecodePayloadAndValidateSeveralWays(fb.buffer(), expected));
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
