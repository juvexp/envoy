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

#include "common/quic/core/frames/quic_window_update_frame.h"

namespace gfe_quic {

QuicWindowUpdateFrame::QuicWindowUpdateFrame() {}

QuicWindowUpdateFrame::QuicWindowUpdateFrame(
    QuicControlFrameId control_frame_id,
    QuicStreamId stream_id,
    QuicStreamOffset byte_offset)
    : QuicControlFrame(control_frame_id),
      stream_id(stream_id),
      byte_offset(byte_offset) {}

std::ostream& operator<<(std::ostream& os,
                         const QuicWindowUpdateFrame& window_update_frame) {
  os << "{ control_frame_id: " << window_update_frame.control_frame_id
     << ", stream_id: " << window_update_frame.stream_id
     << ", byte_offset: " << window_update_frame.byte_offset << " }\n";
  return os;
}

}  // namespace gfe_quic
