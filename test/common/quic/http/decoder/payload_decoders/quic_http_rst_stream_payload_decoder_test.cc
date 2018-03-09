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

#include "common/quic/http/decoder/payload_decoders/quic_http_rst_stream_payload_decoder.h"

#include <stddef.h>

#include <cstdint>

#include "base/logging.h"
#include "test/common/quic/http/decoder/payload_decoders/quic_http_payload_decoder_base_test_util.h"
#include "common/quic/http/decoder/quic_http_frame_decoder_listener.h"
#include "common/quic/http/quic_http_constants.h"
#include "test/common/quic/http/quic_http_constants_test_util.h"
#include "test/common/quic/http/quic_http_structures_test_util.h"
#include "test/common/quic/http/test_tools/quic_http_frame_parts.h"
#include "test/common/quic/http/test_tools/quic_http_frame_parts_collector.h"
#include "test/common/quic/http/tools/quic_http_frame_builder.h"
#include "test/common/quic/http/tools/quic_http_random_decoder_test.h"
#include "gtest/gtest.h"

namespace gfe_quic {
namespace test {

class QuicHttpRstStreamQuicHttpPayloadDecoderPeer {
 public:
  static constexpr QuicHttpFrameType FrameType() {
    return QuicHttpFrameType::RST_STREAM;
  }

  // Returns the mask of flags that affect the decoding of the payload (i.e.
  // flags that that indicate the presence of certain fields or padding).
  static constexpr uint8_t FlagsAffectingPayloadDecoding() { return 0; }

  static void Randomize(QuicHttpRstStreamQuicHttpPayloadDecoder* p,
                        QuicTestRandomBase* rng) {
    VLOG(1) << "QuicHttpRstStreamQuicHttpPayloadDecoderPeer::Randomize";
    test::Randomize(&p->rst_stream_fields_, rng);
  }
};

namespace {

struct Listener : public QuicHttpFramePartsCollector {
  void OnRstStream(const QuicHttpFrameHeader& header,
                   QuicHttpErrorCode error_code) override {
    VLOG(1) << "OnRstStream: " << header << "; error_code=" << error_code;
    StartAndEndFrame(header)->OnRstStream(header, error_code);
  }

  void OnFrameSizeError(const QuicHttpFrameHeader& header) override {
    VLOG(1) << "OnFrameSizeError: " << header;
    FrameError(header)->OnFrameSizeError(header);
  }
};

class QuicHttpRstStreamQuicHttpPayloadDecoderTest
    : public AbstractQuicHttpPayloadDecoderTest<
          QuicHttpRstStreamQuicHttpPayloadDecoder,
          QuicHttpRstStreamQuicHttpPayloadDecoderPeer,
          Listener> {
 protected:
  QuicHttpRstStreamFields RandRstStreamFields() {
    QuicHttpRstStreamFields fields;
    test::Randomize(&fields, RandomPtr());
    return fields;
  }
};

// Confirm we get an error if the payload is not the correct size to hold
// exactly one QuicHttpRstStreamFields.
TEST_F(QuicHttpRstStreamQuicHttpPayloadDecoderTest, WrongSize) {
  auto approve_size = [](size_t size) {
    return size != QuicHttpRstStreamFields::EncodedSize();
  };
  QuicHttpFrameBuilder fb;
  fb.Append(RandRstStreamFields());
  fb.Append(RandRstStreamFields());
  fb.Append(RandRstStreamFields());
  EXPECT_TRUE(VerifyDetectsFrameSizeError(0, fb.buffer(), approve_size));
}

TEST_F(QuicHttpRstStreamQuicHttpPayloadDecoderTest, AllErrors) {
  for (auto error_code : AllQuicHttpErrorCodes()) {
    QuicHttpRstStreamFields fields{error_code};
    QuicHttpFrameBuilder fb;
    fb.Append(fields);
    QuicHttpFrameHeader header(fb.size(), QuicHttpFrameType::RST_STREAM,
                               RandFlags(), RandStreamId());
    set_frame_header(header);
    QuicHttpFrameParts expected(header);
    expected.opt_rst_stream_error_code = error_code;
    EXPECT_TRUE(DecodePayloadAndValidateSeveralWays(fb.buffer(), expected));
  }
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
