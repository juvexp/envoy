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

#ifndef GFE_QUIC_TEST_TOOLS_MOCK_QUIC_SPDY_CLIENT_STREAM_H_
#define GFE_QUIC_TEST_TOOLS_MOCK_QUIC_SPDY_CLIENT_STREAM_H_

#include "base/macros.h"
#include "common/quic/core/quic_header_list.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_spdy_client_stream.h"
#include "testing/base/public/gmock.h"

namespace gfe_quic {
namespace test {

class MockQuicSpdyClientStream : public QuicSpdyClientStream {
 public:
  MockQuicSpdyClientStream(QuicStreamId id, QuicSpdyClientSession* session);
  ~MockQuicSpdyClientStream() override;

  MOCK_METHOD1(OnStreamFrame, void(const QuicStreamFrame& frame));
  MOCK_METHOD3(OnPromiseHeaderList,
               void(QuicStreamId promised_stream_id,
                    size_t frame_len,
                    const QuicHeaderList& list));
  MOCK_METHOD0(OnDataAvailable, void());
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_MOCK_QUIC_SPDY_CLIENT_STREAM_H_
