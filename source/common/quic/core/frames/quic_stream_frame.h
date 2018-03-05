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

#ifndef GFE_QUIC_CORE_FRAMES_QUIC_STREAM_FRAME_H_
#define GFE_QUIC_CORE_FRAMES_QUIC_STREAM_FRAME_H_

#include <memory>
#include <ostream>

#include "common/quic/core/quic_buffer_allocator.h"
#include "common/quic/core/quic_types.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {

struct QUIC_EXPORT_PRIVATE QuicStreamFrame {
  QuicStreamFrame();
  QuicStreamFrame(QuicStreamId stream_id,
                  bool fin,
                  QuicStreamOffset offset,
                  QuicStringPiece data);
  QuicStreamFrame(QuicStreamId stream_id,
                  bool fin,
                  QuicStreamOffset offset,
                  QuicPacketLength data_length);
  ~QuicStreamFrame();

  friend QUIC_EXPORT_PRIVATE std::ostream& operator<<(std::ostream& os,
                                                      const QuicStreamFrame& s);

  QuicStreamId stream_id;
  bool fin;
  QuicPacketLength data_length;
  const char* data_buffer;
  QuicStreamOffset offset;  // Location of this data in the stream.

  QuicStreamFrame(QuicStreamId stream_id,
                  bool fin,
                  QuicStreamOffset offset,
                  const char* data_buffer,
                  QuicPacketLength data_length);

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicStreamFrame);
};
static_assert(sizeof(QuicStreamFrame) <= 64,
              "Keep the QuicStreamFrame size to a cacheline.");

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_FRAMES_QUIC_STREAM_FRAME_H_
