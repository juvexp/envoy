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

#ifndef GFE_QUIC_CORE_QUIC_SPDY_SERVER_STREAM_BASE_H_
#define GFE_QUIC_CORE_QUIC_SPDY_SERVER_STREAM_BASE_H_

#include "common/quic/core/quic_spdy_stream.h"

namespace gfe_quic {

class QuicSpdyServerStreamBase : public QuicSpdyStream {
 public:
  QuicSpdyServerStreamBase(QuicStreamId id, QuicSpdySession* session);

  // Override the base class to send QUIC_STREAM_NO_ERROR to the peer
  // when the stream has not received all the data.
  void CloseWriteSide() override;
  void StopReading() override;

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicSpdyServerStreamBase);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_SPDY_SERVER_STREAM_BASE_H_
