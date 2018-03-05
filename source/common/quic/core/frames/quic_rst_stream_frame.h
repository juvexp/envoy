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

#ifndef GFE_QUIC_CORE_FRAMES_QUIC_RST_STREAM_FRAME_H_
#define GFE_QUIC_CORE_FRAMES_QUIC_RST_STREAM_FRAME_H_

#include <ostream>

#include "common/quic/core/frames/quic_control_frame.h"
#include "common/quic/core/quic_error_codes.h"

namespace gfe_quic {

struct QUIC_EXPORT_PRIVATE QuicRstStreamFrame : public QuicControlFrame {
  QuicRstStreamFrame();
  QuicRstStreamFrame(QuicControlFrameId control_frame_id,
                     QuicStreamId stream_id,
                     QuicRstStreamErrorCode error_code,
                     QuicStreamOffset bytes_written);

  friend QUIC_EXPORT_PRIVATE std::ostream& operator<<(
      std::ostream& os,
      const QuicRstStreamFrame& r);

  QuicStreamId stream_id;
  QuicRstStreamErrorCode error_code;

  // Used to update flow control windows. On termination of a stream, both
  // endpoints must inform the peer of the number of bytes they have sent on
  // that stream. This can be done through normal termination (data packet with
  // FIN) or through a RST.
  QuicStreamOffset byte_offset;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_FRAMES_QUIC_RST_STREAM_FRAME_H_
