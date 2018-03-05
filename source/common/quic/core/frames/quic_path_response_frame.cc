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

#include "common/quic/core/frames/quic_path_response_frame.h"
#include "common/quic/platform/api/quic_bug_tracker.h"

namespace gfe_quic {

QuicPathResponseFrame::QuicPathResponseFrame() : QuicControlFrame(0) {}

QuicPathResponseFrame::QuicPathResponseFrame(
    QuicControlFrameId control_frame_id,
    const QuicPathFrameBuffer& data_buff)
    : QuicControlFrame(control_frame_id) {
  memcpy(data_buffer.data(), data_buff.data(), data_buffer.size());
}

std::ostream& operator<<(std::ostream& os, const QuicPathResponseFrame& frame) {
  os << "{ control_frame_id: " << frame.control_frame_id
     << ", data: " << static_cast<unsigned>(frame.data_buffer[0]) << " "
     << static_cast<unsigned>(frame.data_buffer[1]) << " "
     << static_cast<unsigned>(frame.data_buffer[2]) << " "
     << static_cast<unsigned>(frame.data_buffer[3]) << " "
     << static_cast<unsigned>(frame.data_buffer[4]) << " "
     << static_cast<unsigned>(frame.data_buffer[5]) << " "
     << static_cast<unsigned>(frame.data_buffer[6]) << " "
     << static_cast<unsigned>(frame.data_buffer[7]) << " }\n";
  return os;
}

}  // namespace gfe_quic
