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

#ifndef GFE_QUIC_CORE_FRAMES_QUIC_STOP_SENDING_FRAME_H_
#define GFE_QUIC_CORE_FRAMES_QUIC_STOP_SENDING_FRAME_H_

#include <ostream>

#include "common/quic/core/frames/quic_control_frame.h"
#include "common/quic/core/quic_error_codes.h"

namespace gfe_quic {

struct QUIC_EXPORT_PRIVATE QuicStopSendingFrame : public QuicControlFrame {
  QuicStopSendingFrame();
  QuicStopSendingFrame(QuicControlFrameId control_frame_id,
                       QuicStreamId stream_id,
                       uint16_t application_error_code);

  friend QUIC_EXPORT_PRIVATE std::ostream& operator<<(
      std::ostream& os,
      const QuicStopSendingFrame& frame);

  QuicStreamId stream_id;
  QuicApplicationErrorCode application_error_code;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_FRAMES_QUIC_STOP_SENDING_FRAME_H_
