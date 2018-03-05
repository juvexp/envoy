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

#include "test/common/quic/spdy_utils/core/spdy_no_op_visitor.h"

#include <type_traits>

namespace gfe_spdy {
namespace test {

SpdyNoOpVisitor::SpdyNoOpVisitor() {
  static_assert(std::is_abstract<SpdyNoOpVisitor>::value == false,
                "Need to update SpdyNoOpVisitor.");
}
SpdyNoOpVisitor::~SpdyNoOpVisitor() = default;

SpdyHeadersHandlerInterface* SpdyNoOpVisitor::OnHeaderFrameStart(
    SpdyStreamId stream_id) {
  return this;
}

bool SpdyNoOpVisitor::OnUnknownFrame(SpdyStreamId stream_id,
                                     uint8_t frame_type) {
  return true;
}

}  // namespace test
}  // namespace gfe_spdy
