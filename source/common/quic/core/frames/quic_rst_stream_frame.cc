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

#include "common/quic/core/frames/quic_rst_stream_frame.h"

namespace gfe_quic {

QuicRstStreamFrame::QuicRstStreamFrame()
    : stream_id(0), error_code(QUIC_STREAM_NO_ERROR), byte_offset(0) {}

QuicRstStreamFrame::QuicRstStreamFrame(QuicControlFrameId control_frame_id,
                                       QuicStreamId stream_id,
                                       QuicRstStreamErrorCode error_code,
                                       QuicStreamOffset bytes_written)
    : QuicControlFrame(control_frame_id),
      stream_id(stream_id),
      error_code(error_code),
      byte_offset(bytes_written) {}

std::ostream& operator<<(std::ostream& os,
                         const QuicRstStreamFrame& rst_frame) {
  os << "{ control_frame_id: " << rst_frame.control_frame_id
     << ", stream_id: " << rst_frame.stream_id
     << ", error_code: " << rst_frame.error_code << " }\n";
  return os;
}

}  // namespace gfe_quic
