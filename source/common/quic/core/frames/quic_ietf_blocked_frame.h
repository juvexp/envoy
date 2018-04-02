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

#ifndef GFE_QUIC_CORE_FRAMES_QUIC_IETF_BLOCKED_FRAME_H_
#define GFE_QUIC_CORE_FRAMES_QUIC_IETF_BLOCKED_FRAME_H_

#include <ostream>

#include "common/quic/core/frames/quic_control_frame.h"

namespace gfe_quic {

// IETF format BLOCKED frame.
// The sender uses the BLOCKED frame to inform the receiver that the
// sender is unable to send data because of connection-level flow control.
struct QUIC_EXPORT_PRIVATE QuicIetfBlockedFrame : public QuicControlFrame {
  QuicIetfBlockedFrame();
  QuicIetfBlockedFrame(QuicControlFrameId control_frame_id,
                       QuicStreamOffset offset);

  friend QUIC_EXPORT_PRIVATE std::ostream& operator<<(
      std::ostream& os,
      const QuicIetfBlockedFrame& frame);

  // Offset at which the BLOCKED applies
  QuicStreamOffset offset;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_FRAMES_QUIC_IETF_BLOCKED_FRAME_H_
