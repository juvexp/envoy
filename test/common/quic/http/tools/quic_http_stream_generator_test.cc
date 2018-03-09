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

#include "test/common/quic/http/tools/quic_http_stream_generator.h"

#include "base/logging.h"
#include "test/common/quic/http/tools/quic_http_frame_formatter.h"
#include "test/common/quic/http/tools/quic_http_message_generator.h"
#include "test/common/quic/http/tools/quic_http_message_interface.h"
#include "common/quic/spdy_utils/core/spdy_framer.h"
#include "common/quic/spdy_utils/core/spdy_protocol.h"
#include "testing/base/public/googletest.h"
#include "gtest/gtest.h"
#include "util/random/mt_random.h"

using ::gfe_spdy::SpdyFramer;

namespace gfe_quic {
namespace test {
namespace {

// These "tests" mostly exercise the code, rather than test it, though they
// do confirm that the exercised code doesn't crash.

TEST(StreamGeneratorTest, SerializeOneMessage) {
  MTRandom rng(FLAGS_test_random_seed);
  SpdyFramer framer(SpdyFramer::ENABLE_COMPRESSION);
  StreamIdGenerator id_generator(&rng);
  auto mp = GenerateGrpcRequest(10, 100, &rng);

  auto strings = SerializeMessage(*mp, 1, &id_generator, &framer, &rng);

  for (auto& s : strings) {
    LOG(INFO) << "Header:\n\n" << FormatFrameHeader(s, "  ") << "\n\n";
  }
}

TEST(StreamGeneratorTest, SerializeOneMessageTwice) {
  MTRandom rng(FLAGS_test_random_seed);
  auto mp = GenerateGrpcResponse(10, 100, &rng);
  auto serializer = CreateSerializer(&rng);
  serializer->SerializeMessage(*mp, 0);
  serializer->SerializeMessage(*mp, 2);

  const auto& strings = serializer->serialized_frames();

  for (auto& s : strings) {
    LOG(INFO) << "Header:\n\n" << FormatFrameHeader(s, "  ") << "\n\n";
  }
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
