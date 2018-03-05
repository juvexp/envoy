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

#include "common/quic/core/frames/quic_goaway_frame.h"
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {

QuicGoAwayFrame::QuicGoAwayFrame()
    : error_code(QUIC_NO_ERROR), last_good_stream_id(0) {}

QuicGoAwayFrame::QuicGoAwayFrame(QuicControlFrameId control_frame_id,
                                 QuicErrorCode error_code,
                                 QuicStreamId last_good_stream_id,
                                 const QuicString& reason)
    : QuicControlFrame(control_frame_id),
      error_code(error_code),
      last_good_stream_id(last_good_stream_id),
      reason_phrase(reason) {}

std::ostream& operator<<(std::ostream& os,
                         const QuicGoAwayFrame& goaway_frame) {
  os << "{ control_frame_id: " << goaway_frame.control_frame_id
     << ", error_code: " << goaway_frame.error_code
     << ", last_good_stream_id: " << goaway_frame.last_good_stream_id
     << ", reason_phrase: '" << goaway_frame.reason_phrase << "' }\n";
  return os;
}

}  // namespace gfe_quic
