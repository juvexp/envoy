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

#ifndef GFE_QUIC_CORE_FRAMES_QUIC_WINDOW_UPDATE_FRAME_H_
#define GFE_QUIC_CORE_FRAMES_QUIC_WINDOW_UPDATE_FRAME_H_

#include <ostream>

#include "common/quic/core/frames/quic_control_frame.h"

namespace gfe_quic {

// Flow control updates per-stream and at the connection level.
// Based on SPDY's WINDOW_UPDATE frame, but uses an absolute byte offset rather
// than a window delta.
// TODO: A possible future optimization is to make stream_id and
//                byte_offset variable length, similar to stream frames.
struct QUIC_EXPORT_PRIVATE QuicWindowUpdateFrame : public QuicControlFrame {
  QuicWindowUpdateFrame();
  QuicWindowUpdateFrame(QuicControlFrameId control_frame_id,
                        QuicStreamId stream_id,
                        QuicStreamOffset byte_offset);

  friend QUIC_EXPORT_PRIVATE std::ostream& operator<<(
      std::ostream& os,
      const QuicWindowUpdateFrame& w);

  // The stream this frame applies to.  0 is a special case meaning the overall
  // connection rather than a specific stream.
  QuicStreamId stream_id;

  // Byte offset in the stream or connection. The receiver of this frame must
  // not send data which would result in this offset being exceeded.
  //
  // TODO: Rename this to max_data and change the type to
  // QuicByteCount because the IETF defines this as the "maximum
  // amount of data that can be sent".
  QuicStreamOffset byte_offset;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_FRAMES_QUIC_WINDOW_UPDATE_FRAME_H_
