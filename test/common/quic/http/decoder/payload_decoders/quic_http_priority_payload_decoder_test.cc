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

#include "common/quic/http/decoder/payload_decoders/quic_http_priority_payload_decoder.h"

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
#include "testing/base/public/gunit.h"

namespace gfe_quic {
namespace test {

class QuicHttpPriorityQuicHttpPayloadDecoderPeer {
 public:
  static constexpr QuicHttpFrameType FrameType() {
    return QuicHttpFrameType::QUIC_HTTP_PRIORITY;
  }

  // Returns the mask of flags that affect the decoding of the payload (i.e.
  // flags that that indicate the presence of certain fields or padding).
  static constexpr uint8_t FlagsAffectingPayloadDecoding() { return 0; }

  static void Randomize(QuicHttpPriorityQuicHttpPayloadDecoder* p,
                        QuicTestRandomBase* rng) {
    VLOG(1) << "QuicHttpPriorityQuicHttpPayloadDecoderPeer::Randomize";
    test::Randomize(&p->priority_fields_, rng);
  }
};

namespace {

struct Listener : public QuicHttpFramePartsCollector {
  void OnPriorityFrame(const QuicHttpFrameHeader& header,
                       const QuicHttpPriorityFields& priority_fields) override {
    VLOG(1) << "OnPriority: " << header << "; " << priority_fields;
    StartAndEndFrame(header)->OnPriorityFrame(header, priority_fields);
  }

  void OnFrameSizeError(const QuicHttpFrameHeader& header) override {
    VLOG(1) << "OnFrameSizeError: " << header;
    FrameError(header)->OnFrameSizeError(header);
  }
};

class QuicHttpPriorityQuicHttpPayloadDecoderTest
    : public AbstractQuicHttpPayloadDecoderTest<
          QuicHttpPriorityQuicHttpPayloadDecoder,
          QuicHttpPriorityQuicHttpPayloadDecoderPeer,
          Listener> {
 protected:
  QuicHttpPriorityFields RandPriorityFields() {
    QuicHttpPriorityFields fields;
    test::Randomize(&fields, RandomPtr());
    return fields;
  }
};

// Confirm we get an error if the payload is not the correct size to hold
// exactly one QuicHttpPriorityFields.
TEST_F(QuicHttpPriorityQuicHttpPayloadDecoderTest, WrongSize) {
  auto approve_size = [](size_t size) {
    return size != QuicHttpPriorityFields::EncodedSize();
  };
  QuicHttpFrameBuilder fb;
  fb.Append(RandPriorityFields());
  fb.Append(RandPriorityFields());
  EXPECT_TRUE(VerifyDetectsFrameSizeError(0, fb.buffer(), approve_size));
}

TEST_F(QuicHttpPriorityQuicHttpPayloadDecoderTest, VariousPayloads) {
  for (int n = 0; n < 100; ++n) {
    QuicHttpPriorityFields fields = RandPriorityFields();
    QuicHttpFrameBuilder fb;
    fb.Append(fields);
    QuicHttpFrameHeader header(fb.size(), QuicHttpFrameType::QUIC_HTTP_PRIORITY,
                               RandFlags(), RandStreamId());
    set_frame_header(header);
    QuicHttpFrameParts expected(header);
    expected.opt_priority = fields;
    EXPECT_TRUE(DecodePayloadAndValidateSeveralWays(fb.buffer(), expected));
  }
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
