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

#include "common/quic/core/frames/quic_stream_frame.h"

#include "common/quic/platform/api/quic_logging.h"

namespace gfe_quic {

QuicStreamFrame::QuicStreamFrame() : QuicStreamFrame(0, false, 0, nullptr, 0) {}

QuicStreamFrame::QuicStreamFrame(QuicStreamId stream_id, bool fin,
                                 QuicStreamOffset offset, QuicStringPiece data)
    : QuicStreamFrame(stream_id, fin, offset, data.data(), data.length()) {}

QuicStreamFrame::QuicStreamFrame(QuicStreamId stream_id,
                                 bool fin,
                                 QuicStreamOffset offset,
                                 QuicPacketLength data_length)
    : QuicStreamFrame(stream_id, fin, offset, nullptr, data_length) {}

QuicStreamFrame::QuicStreamFrame(QuicStreamId stream_id,
                                 bool fin,
                                 QuicStreamOffset offset,
                                 const char* data_buffer,
                                 QuicPacketLength data_length)
    : stream_id(stream_id),
      fin(fin),
      data_length(data_length),
      data_buffer(data_buffer),
      offset(offset) {}

QuicStreamFrame::~QuicStreamFrame() {}

std::ostream& operator<<(std::ostream& os,
                         const QuicStreamFrame& stream_frame) {
  os << "{ stream_id: " << stream_frame.stream_id
     << ", fin: " << stream_frame.fin << ", offset: " << stream_frame.offset
     << ", length: " << stream_frame.data_length << " }\n";
  return os;
}

}  // namespace gfe_quic
