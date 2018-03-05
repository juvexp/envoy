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

#ifndef GFE_QUIC_CORE_QUIC_STREAM_FRAME_DATA_PRODUCER_H_
#define GFE_QUIC_CORE_QUIC_STREAM_FRAME_DATA_PRODUCER_H_

#include "common/quic/core/quic_types.h"

namespace gfe_quic {

class QuicDataWriter;

// Pure virtual class to retrieve stream data.
class QUIC_EXPORT_PRIVATE QuicStreamFrameDataProducer {
 public:
  virtual ~QuicStreamFrameDataProducer() {}

  // Let |writer| write |data_length| data with |offset| of stream |id|. Returns
  // false when the writing fails either because stream is closed or
  // corresponding data is failed to be retrieved. This method allows writing a
  // single stream frame from data that spans multiple buffers.
  virtual bool WriteStreamData(QuicStreamId id,
                               QuicStreamOffset offset,
                               QuicByteCount data_length,
                               QuicDataWriter* writer) = 0;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_STREAM_FRAME_DATA_PRODUCER_H_
