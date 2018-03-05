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

#include "common/quic/core/frames/quic_stop_waiting_frame.h"

#include "common/quic/core/quic_constants.h"

namespace gfe_quic {

QuicStopWaitingFrame::QuicStopWaitingFrame() : least_unacked(0) {}

QuicStopWaitingFrame::~QuicStopWaitingFrame() {}

std::ostream& operator<<(std::ostream& os,
                         const QuicStopWaitingFrame& sent_info) {
  os << "{ least_unacked: " << sent_info.least_unacked << " }\n";
  return os;
}

}  // namespace gfe_quic
