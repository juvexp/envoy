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

#include "common/quic/core/quic_spdy_server_stream_base.h"

#include "common/quic/core/quic_error_codes.h"
#include "common/quic/platform/api/quic_logging.h"

namespace gfe_quic {

QuicSpdyServerStreamBase::QuicSpdyServerStreamBase(QuicStreamId id,
                                                   QuicSpdySession* session)
    : QuicSpdyStream(id, session) {}

void QuicSpdyServerStreamBase::CloseWriteSide() {
  if (!fin_received() && !rst_received() && sequencer()->ignore_read_data() &&
      !rst_sent()) {
    // Early cancel the stream if it has stopped reading before receiving FIN
    // or RST.
    DCHECK(fin_sent());
    // Tell the peer to stop sending further data.
    QUIC_DVLOG(1) << " Server: Send QUIC_STREAM_NO_ERROR on stream " << id();
    Reset(QUIC_STREAM_NO_ERROR);
  }

  QuicSpdyStream::CloseWriteSide();
}

void QuicSpdyServerStreamBase::StopReading() {
  if (!fin_received() && !rst_received() && write_side_closed() &&
      !rst_sent()) {
    DCHECK(fin_sent());
    // Tell the peer to stop sending further data.
    QUIC_DVLOG(1) << " Server: Send QUIC_STREAM_NO_ERROR on stream " << id();
    Reset(QUIC_STREAM_NO_ERROR);
  }
  QuicSpdyStream::StopReading();
}

}  // namespace gfe_quic
