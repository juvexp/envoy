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

#ifndef GFE_QUIC_CORE_FRAMES_QUIC_CONTROL_FRAME_H_
#define GFE_QUIC_CORE_FRAMES_QUIC_CONTROL_FRAME_H_

#include "common/quic/core/quic_constants.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

struct QUIC_EXPORT_PRIVATE QuicControlFrame {
  QuicControlFrame() : control_frame_id(kInvalidControlFrameId) {}
  explicit QuicControlFrame(QuicControlFrameId control_frame_id)
      : control_frame_id(control_frame_id) {}

  // A unique identifier of this control frame. 0 when this frame is received,
  // and non-zero when sent.
  QuicControlFrameId control_frame_id;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_FRAMES_QUIC_CONTROL_FRAME_H_
