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

#ifndef GFE_QUIC_CORE_FRAMES_QUIC_IETF_STREAM_ID_BLOCKED_FRAME_H_
#define GFE_QUIC_CORE_FRAMES_QUIC_IETF_STREAM_ID_BLOCKED_FRAME_H_

#include <ostream>

#include "common/quic/core/frames/quic_control_frame.h"

namespace gfe_quic {

// IETF format STREAM ID BLOCKED frame.
// The sender uses this to inform the peer that the sender wished to
// open a new stream but was blocked from doing so due due to the
// maximum stream ID limit set by the peer (via a MAX_STREAM_ID frame)
struct QUIC_EXPORT_PRIVATE QuicIetfStreamIdBlockedFrame
    : public QuicControlFrame {
  QuicIetfStreamIdBlockedFrame();
  QuicIetfStreamIdBlockedFrame(QuicControlFrameId control_frame_id,
                               QuicStreamId stream_id);

  friend QUIC_EXPORT_PRIVATE std::ostream& operator<<(
      std::ostream& os,
      const QuicIetfStreamIdBlockedFrame& frame);

  QuicStreamId stream_id;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_FRAMES_QUIC_IETF_STREAM_ID_BLOCKED_FRAME_H_
