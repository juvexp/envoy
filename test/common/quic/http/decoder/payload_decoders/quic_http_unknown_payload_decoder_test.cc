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

#include <cstdint>
#include <type_traits>

#include "base/logging.h"
#include "test/common/quic/http/decoder/payload_decoders/quic_http_payload_decoder_base_test_util.h"
#include "common/quic/http/decoder/quic_http_frame_decoder_listener.h"
#include "common/quic/http/quic_http_constants.h"
#include "common/quic/http/quic_http_structures.h"
#include "test/common/quic/http/test_tools/quic_http_frame_parts.h"
#include "test/common/quic/http/test_tools/quic_http_frame_parts_collector.h"
#include "test/common/quic/http/tools/quic_http_random_decoder_test.h"
#include "common/quic/platform/api/quic_string.h"
#include "testing/base/public/gunit.h"

namespace gfe_quic {
namespace test {
namespace {
QuicHttpFrameType g_unknown_frame_type;
}  // namespace

// Provides friend access to an instance of the payload decoder, and also
// provides info to aid in testing.
class QuicHttpUnknownQuicHttpPayloadDecoderPeer {
 public:
  static QuicHttpFrameType FrameType() { return g_unknown_frame_type; }

  // Returns the mask of flags that affect the decoding of the payload (i.e.
  // flags that that indicate the presence of certain fields or padding).
  static constexpr uint8_t FlagsAffectingPayloadDecoding() { return 0; }

  static void Randomize(QuicHttpUnknownQuicHttpPayloadDecoder* p,
                        QuicTestRandomBase* rng) {
    // QuicHttpUnknownQuicHttpPayloadDecoder has no fields, so there is nothing
    // to randomize.
    static_assert(
        std::is_empty<QuicHttpUnknownQuicHttpPayloadDecoder>::value,
        "Need to randomize fields of QuicHttpUnknownQuicHttpPayloadDecoder");
  }
};

namespace {

struct Listener : public QuicHttpFramePartsCollector {
  void OnUnknownStart(const QuicHttpFrameHeader& header) override {
    VLOG(1) << "OnUnknownStart: " << header;
    StartFrame(header)->OnUnknownStart(header);
  }

  void OnUnknownPayload(const char* data, size_t len) override {
    VLOG(1) << "OnUnknownPayload: len=" << len;
    CurrentFrame()->OnUnknownPayload(data, len);
  }

  void OnUnknownEnd() override {
    VLOG(1) << "OnUnknownEnd";
    EndFrame()->OnUnknownEnd();
  }
};

constexpr bool SupportedFrameType = false;

class QuicHttpUnknownQuicHttpPayloadDecoderTest
    : public AbstractQuicHttpPayloadDecoderTest<
          QuicHttpUnknownQuicHttpPayloadDecoder,
          QuicHttpUnknownQuicHttpPayloadDecoderPeer,
          Listener,
          SupportedFrameType>,
      public ::testing::WithParamInterface<uint32_t> {
 protected:
  QuicHttpUnknownQuicHttpPayloadDecoderTest() : length_(GetParam()) {
    VLOG(1) << "################  length_=" << length_ << "  ################";

    // To increase coverage of the D?VLOG's, if the length is 3, then raise the
    // verbosity level. The reason for choosing length=3 is that when decoding a
    // payload or string 1 byte at a time, there will be a chunk at the start
    // of the string, a chunk in the middle, and a chunk at the end, thus
    // exercising handling of all 3 of those situations.
    if (length_ == 3) {
      BoostLoggingIfDefault(4);
    }

    // Each test case will choose a random frame type that isn't supported.
    do {
      g_unknown_frame_type = static_cast<QuicHttpFrameType>(Random().Rand8());
    } while (IsSupportedQuicHttpFrameType(g_unknown_frame_type));
  }

  const uint32_t length_;
};

INSTANTIATE_TEST_CASE_P(VariousLengths,
                        QuicHttpUnknownQuicHttpPayloadDecoderTest,
                        ::testing::Values(0, 1, 2, 3, 255, 256));

TEST_P(QuicHttpUnknownQuicHttpPayloadDecoderTest, ValidLength) {
  QuicString unknown_payload = Random().RandString(length_);
  QuicHttpFrameHeader frame_header(length_, g_unknown_frame_type,
                                   Random().Rand8(), RandStreamId());
  set_frame_header(frame_header);
  QuicHttpFrameParts expected(frame_header, unknown_payload);
  EXPECT_TRUE(DecodePayloadAndValidateSeveralWays(unknown_payload, expected));
  // TODO: Check here (and in other such tests) that the fast
  // and slow decode counts are both non-zero. Perhaps also add some kind of
  // test for the listener having been called. That could simply be a test
  // that there is a single collected QuicHttpFrameParts instance, and that it
  // matches expected.
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
