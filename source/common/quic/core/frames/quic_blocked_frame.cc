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

#include "common/quic/core/frames/quic_blocked_frame.h"

namespace gfe_quic {

QuicBlockedFrame::QuicBlockedFrame() {}

QuicBlockedFrame::QuicBlockedFrame(QuicControlFrameId control_frame_id,
                                   QuicStreamId stream_id)
    : QuicControlFrame(control_frame_id), stream_id(stream_id) {}

std::ostream& operator<<(std::ostream& os,
                         const QuicBlockedFrame& blocked_frame) {
  os << "{ control_frame_id: " << blocked_frame.control_frame_id
     << ", stream_id: " << blocked_frame.stream_id << " }\n";
  return os;
}

}  // namespace gfe_quic
