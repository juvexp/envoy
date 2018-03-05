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

#ifndef GFE_QUIC_CORE_FRAMES_QUIC_BLOCKED_FRAME_H_
#define GFE_QUIC_CORE_FRAMES_QUIC_BLOCKED_FRAME_H_

#include <ostream>

#include "common/quic/core/frames/quic_control_frame.h"

namespace gfe_quic {

// The BLOCKED frame is used to indicate to the remote endpoint that this
// endpoint believes itself to be flow-control blocked but otherwise ready to
// send data. The BLOCKED frame is purely advisory and optional.
// Based on SPDY's BLOCKED frame (undocumented as of 2014-01-28).
struct QUIC_EXPORT_PRIVATE QuicBlockedFrame : public QuicControlFrame {
  QuicBlockedFrame();
  QuicBlockedFrame(QuicControlFrameId control_frame_id, QuicStreamId stream_id);

  friend QUIC_EXPORT_PRIVATE std::ostream& operator<<(
      std::ostream& os,
      const QuicBlockedFrame& b);

  // The stream this frame applies to.  0 is a special case meaning the overall
  // connection rather than a specific stream.
  QuicStreamId stream_id;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_FRAMES_QUIC_BLOCKED_FRAME_H_
