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

#include "test/common/quic/http/decoder/quic_http_frame_decoder_state_test_util.h"

#include "base/logging.h"
#include "test/common/quic/http/decoder/quic_http_structure_decoder_test_util.h"
#include "common/quic/http/quic_http_structures.h"
#include "test/common/quic/http/quic_http_structures_test_util.h"
#include "test/common/quic/http/tools/quic_http_random_decoder_test.h"

namespace gfe_quic {
namespace test {

// static
void QuicHttpFrameDecoderStatePeer::set_frame_header(
    const QuicHttpFrameHeader& header,
    QuicHttpFrameDecoderState* p) {
  VLOG(1) << "QuicHttpFrameDecoderStatePeer::set_frame_header " << header;
  p->frame_header_ = header;
}

}  // namespace test
}  // namespace gfe_quic
