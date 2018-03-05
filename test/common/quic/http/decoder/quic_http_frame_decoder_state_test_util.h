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

#ifndef GFE_QUIC_HTTP_DECODER_QUIC_HTTP_FRAME_DECODER_STATE_TEST_UTIL_H_
#define GFE_QUIC_HTTP_DECODER_QUIC_HTTP_FRAME_DECODER_STATE_TEST_UTIL_H_

#include "common/quic/http/decoder/quic_http_frame_decoder_state.h"
#include "common/quic/http/quic_http_structures.h"
#include "test/common/quic/http/tools/quic_http_random_decoder_test.h"

namespace gfe_quic {
namespace test {

class QuicHttpFrameDecoderStatePeer {
 public:
  // Inject a frame header into the QuicHttpFrameDecoderState.
  // QuicHttpPayloadDecoderBaseTest::StartDecoding calls this just after calling
  // Randomize (above), to simulate a full frame decoder having just finished
  // decoding the common frame header and then calling the appropriate payload
  // decoder based on the frame type in that frame header.
  static void set_frame_header(const QuicHttpFrameHeader& header,
                               QuicHttpFrameDecoderState* p);
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_DECODER_QUIC_HTTP_FRAME_DECODER_STATE_TEST_UTIL_H_
