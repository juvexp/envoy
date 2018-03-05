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

#ifndef GFE_QUIC_CORE_FRAMES_QUIC_PATH_RESPONSE_FRAME_H_
#define GFE_QUIC_CORE_FRAMES_QUIC_PATH_RESPONSE_FRAME_H_

#include <memory>
#include <ostream>

#include "common/quic/core/frames/quic_control_frame.h"
#include "common/quic/core/quic_types.h"

namespace gfe_quic {

// Size of the entire IETF Quic Path Response frame, including
// type byte.
const size_t kQuicPathResponseFrameSize = (kQuicPathFrameBufferSize + 1);

struct QUIC_EXPORT_PRIVATE QuicPathResponseFrame : public QuicControlFrame {
  QuicPathResponseFrame();
  QuicPathResponseFrame(QuicControlFrameId control_frame_id,
                        const QuicPathFrameBuffer& data_buff);
  ~QuicPathResponseFrame() {}

  friend QUIC_EXPORT_PRIVATE std::ostream& operator<<(
      std::ostream& os,
      const QuicPathResponseFrame& frame);

  QuicPathFrameBuffer data_buffer;

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicPathResponseFrame);
};
}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_FRAMES_QUIC_PATH_RESPONSE_FRAME_H_
