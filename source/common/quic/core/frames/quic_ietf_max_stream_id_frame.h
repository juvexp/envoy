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

#ifndef GFE_QUIC_CORE_FRAMES_QUIC_IETF_MAX_STREAM_ID_FRAME_H_
#define GFE_QUIC_CORE_FRAMES_QUIC_IETF_MAX_STREAM_ID_FRAME_H_

#include <ostream>

#include "common/quic/core/frames/quic_control_frame.h"

namespace gfe_quic {

// IETF format max-stream id frame.
// This frame is used by the sender to inform the peer of the largest
// stream id that the peer may open and that the sender will accept.
struct QUIC_EXPORT_PRIVATE QuicIetfMaxStreamIdFrame : public QuicControlFrame {
  QuicIetfMaxStreamIdFrame();
  QuicIetfMaxStreamIdFrame(QuicControlFrameId control_frame_id,
                           QuicStreamId max_stream_id);

  friend QUIC_EXPORT_PRIVATE std::ostream& operator<<(
      std::ostream& os,
      const QuicIetfMaxStreamIdFrame& frame);

  // The maximum stream id to support.
  QuicStreamId max_stream_id;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_FRAMES_QUIC_IETF_MAX_STREAM_ID_FRAME_H_
