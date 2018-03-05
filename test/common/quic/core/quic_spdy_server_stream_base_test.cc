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

#include "common/quic/platform/api/quic_ptr_util.h"
#include "test/common/quic/platform/api/quic_test.h"
#include "test/common/quic/test_tools/quic_spdy_session_peer.h"
#include "test/common/quic/test_tools/quic_test_utils.h"

using testing::_;

namespace gfe_quic {
namespace test {
namespace {

class TestQuicSpdyServerStream : public QuicSpdyServerStreamBase {
 public:
  TestQuicSpdyServerStream(QuicStreamId id, QuicSpdySession* session)
      : QuicSpdyServerStreamBase(id, session) {}

  void OnDataAvailable() override {}
};

class QuicSpdyServerStreamBaseTest : public QuicTest {
 protected:
  QuicSpdyServerStreamBaseTest()
      : session_(new MockQuicConnection(&helper_,
                                        &alarm_factory_,
                                        Perspective::IS_SERVER)) {
    stream_ = new TestQuicSpdyServerStream(
        QuicSpdySessionPeer::GetNthClientInitiatedStreamId(session_, 0),
        &session_);
    session_.ActivateStream(QuicWrapUnique(stream_));
  }

  QuicSpdyServerStreamBase* stream_ = nullptr;
  MockQuicConnectionHelper helper_;
  MockAlarmFactory alarm_factory_;
  MockQuicSpdySession session_;
};

TEST_F(QuicSpdyServerStreamBaseTest,
       SendQuicRstStreamNoErrorWithEarlyResponse) {
  stream_->StopReading();
  EXPECT_CALL(session_, SendRstStream(_, QUIC_STREAM_NO_ERROR, _)).Times(1);
  stream_->set_fin_sent(true);
  stream_->CloseWriteSide();
}

TEST_F(QuicSpdyServerStreamBaseTest,
       DoNotSendQuicRstStreamNoErrorWithRstReceived) {
  EXPECT_FALSE(stream_->reading_stopped());

  EXPECT_CALL(session_, SendRstStream(_, QUIC_STREAM_NO_ERROR, _)).Times(0);
  EXPECT_CALL(session_, SendRstStream(_, QUIC_RST_ACKNOWLEDGEMENT, _)).Times(1);
  QuicRstStreamFrame rst_frame(kInvalidControlFrameId, stream_->id(),
                               QUIC_STREAM_CANCELLED, 1234);
  stream_->OnStreamReset(rst_frame);

  EXPECT_TRUE(stream_->reading_stopped());
  EXPECT_TRUE(stream_->write_side_closed());
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
