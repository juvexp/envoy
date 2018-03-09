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

#include "common/quic/core/frames/quic_stop_sending_frame.h"

namespace gfe_quic {

QuicStopSendingFrame::QuicStopSendingFrame()
    : stream_id(0), application_error_code(0) {}

QuicStopSendingFrame::QuicStopSendingFrame(
    QuicControlFrameId control_frame_id,
    QuicStreamId stream_id,
    QuicApplicationErrorCode application_error_code)
    : QuicControlFrame(control_frame_id),
      stream_id(stream_id),
      application_error_code(application_error_code) {}

std::ostream& operator<<(std::ostream& os, const QuicStopSendingFrame& frame) {
  os << "{ control_frame_id: " << frame.control_frame_id
     << ", stream_id: " << frame.stream_id
     << ", application_error_code: " << frame.application_error_code << " }\n";
  return os;
}

}  // namespace gfe_quic
