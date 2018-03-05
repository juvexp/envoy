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

#include "common/quic/core/quic_client_push_promise_index.h"

#include "common/quic/core/quic_spdy_client_session.h"
#include "common/quic/core/spdy_utils.h"
#include "common/quic/core/tls_client_handshaker.h"
#include "common/quic/platform/api/quic_string.h"
#include "test/common/quic/platform/api/quic_test.h"
#include "test/common/quic/test_tools/crypto_test_utils.h"
#include "test/common/quic/test_tools/mock_quic_client_promised_info.h"
#include "test/common/quic/test_tools/quic_spdy_session_peer.h"
#include "test/common/quic/test_tools/quic_test_utils.h"

using testing::_;
using testing::Return;
using testing::StrictMock;

namespace gfe_quic {
namespace test {
namespace {

class MockQuicSpdyClientSession : public QuicSpdyClientSession {
 public:
  explicit MockQuicSpdyClientSession(
      QuicConnection* connection,
      QuicClientPushPromiseIndex* push_promise_index)
      : QuicSpdyClientSession(
            DefaultQuicConfig(),
            connection,
            QuicServerId("example.com", 443, PRIVACY_MODE_DISABLED),
            &crypto_config_,
            push_promise_index),
        crypto_config_(crypto_test_utils::ProofVerifierForTesting(),
                       TlsClientHandshaker::CreateSslCtx()) {}
  ~MockQuicSpdyClientSession() override {}

  MOCK_METHOD1(CloseStream, void(QuicStreamId stream_id));

 private:
  QuicCryptoClientConfig crypto_config_;

  DISALLOW_COPY_AND_ASSIGN(MockQuicSpdyClientSession);
};

class QuicClientPushPromiseIndexTest : public QuicTest {
 public:
  QuicClientPushPromiseIndexTest()
      : connection_(new StrictMock<MockQuicConnection>(&helper_,
                                                       &alarm_factory_,
                                                       Perspective::IS_CLIENT)),
        session_(connection_, &index_),
        promised_(
            &session_,
            QuicSpdySessionPeer::GetNthServerInitiatedStreamId(session_, 0),
            url_) {
    request_[":path"] = "/bar";
    request_[":authority"] = "www.google.com";
    request_[":version"] = "HTTP/1.1";
    request_[":method"] = "GET";
    request_[":scheme"] = "https";
    url_ = SpdyUtils::GetPromisedUrlFromHeaders(request_);
  }

  MockQuicConnectionHelper helper_;
  MockAlarmFactory alarm_factory_;
  StrictMock<MockQuicConnection>* connection_;
  MockQuicSpdyClientSession session_;
  QuicClientPushPromiseIndex index_;
  gfe_spdy::SpdyHeaderBlock request_;
  QuicString url_;
  MockQuicClientPromisedInfo promised_;
  QuicClientPushPromiseIndex::TryHandle* handle_;
};

TEST_F(QuicClientPushPromiseIndexTest, TryRequestSuccess) {
  (*index_.promised_by_url())[url_] = &promised_;
  EXPECT_CALL(promised_, HandleClientRequest(_, _))
      .WillOnce(Return(QUIC_SUCCESS));
  EXPECT_EQ(index_.Try(request_, nullptr, &handle_), QUIC_SUCCESS);
}

TEST_F(QuicClientPushPromiseIndexTest, TryRequestPending) {
  (*index_.promised_by_url())[url_] = &promised_;
  EXPECT_CALL(promised_, HandleClientRequest(_, _))
      .WillOnce(Return(QUIC_PENDING));
  EXPECT_EQ(index_.Try(request_, nullptr, &handle_), QUIC_PENDING);
}

TEST_F(QuicClientPushPromiseIndexTest, TryRequestFailure) {
  (*index_.promised_by_url())[url_] = &promised_;
  EXPECT_CALL(promised_, HandleClientRequest(_, _))
      .WillOnce(Return(QUIC_FAILURE));
  EXPECT_EQ(index_.Try(request_, nullptr, &handle_), QUIC_FAILURE);
}

TEST_F(QuicClientPushPromiseIndexTest, TryNoPromise) {
  EXPECT_EQ(index_.Try(request_, nullptr, &handle_), QUIC_FAILURE);
}

TEST_F(QuicClientPushPromiseIndexTest, GetNoPromise) {
  EXPECT_EQ(index_.GetPromised(url_), nullptr);
}

TEST_F(QuicClientPushPromiseIndexTest, GetPromise) {
  (*index_.promised_by_url())[url_] = &promised_;
  EXPECT_EQ(index_.GetPromised(url_), &promised_);
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
