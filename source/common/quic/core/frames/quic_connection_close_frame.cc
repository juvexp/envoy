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

#include "common/quic/core/frames/quic_connection_close_frame.h"

namespace gfe_quic {

QuicConnectionCloseFrame::QuicConnectionCloseFrame()
    : error_code(QUIC_NO_ERROR) {}

QuicConnectionCloseFrame::QuicConnectionCloseFrame(QuicErrorCode error_code,
                                                   QuicString error_details)
    : error_code(error_code), error_details(error_details) {}

std::ostream& operator<<(
    std::ostream& os,
    const QuicConnectionCloseFrame& connection_close_frame) {
  os << "{ error_code: " << connection_close_frame.error_code
     << ", error_details: '" << connection_close_frame.error_details << "' }\n";
  return os;
}

}  // namespace gfe_quic
