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

#ifndef GFE_QUIC_TEST_TOOLS_MOCK_QUIC_SESSION_VISITOR_H_
#define GFE_QUIC_TEST_TOOLS_MOCK_QUIC_SESSION_VISITOR_H_

#include "base/macros.h"
#include "common/quic/core/quic_crypto_server_stream.h"
#include "common/quic/core/quic_time_wait_list_manager.h"
#include "gmock/gmock.h"

namespace gfe_quic {
namespace test {

class MockQuicSessionVisitor : public QuicTimeWaitListManager::Visitor {
 public:
  MockQuicSessionVisitor();
  ~MockQuicSessionVisitor() override;
  MOCK_METHOD3(OnConnectionClosed,
               void(QuicConnectionId connection_id,
                    QuicErrorCode error,
                    const string& error_details));
  MOCK_METHOD1(OnWriteBlocked,
               void(QuicBlockedWriterInterface* blocked_writer));
  MOCK_METHOD1(OnRstStreamReceived, void(const QuicRstStreamFrame& frame));
  MOCK_METHOD1(OnConnectionAddedToTimeWaitList,
               void(QuicConnectionId connection_id));

 private:
  DISALLOW_COPY_AND_ASSIGN(MockQuicSessionVisitor);
};

class MockQuicCryptoServerStreamHelper : public QuicCryptoServerStream::Helper {
 public:
  MockQuicCryptoServerStreamHelper();
  ~MockQuicCryptoServerStreamHelper() override;
  MOCK_CONST_METHOD1(GenerateConnectionIdForReject,
                     QuicConnectionId(QuicConnectionId connection_id));
  MOCK_CONST_METHOD3(CanAcceptClientHello,
                     bool(const CryptoHandshakeMessage& message,
                          const QuicSocketAddress& self_address,
                          string* error_details));

 private:
  DISALLOW_COPY_AND_ASSIGN(MockQuicCryptoServerStreamHelper);
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_MOCK_QUIC_SESSION_VISITOR_H_
