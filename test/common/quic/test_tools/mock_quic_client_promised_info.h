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

#ifndef GFE_QUIC_TEST_TOOLS_MOCK_QUIC_CLIENT_PROMISED_INFO_H_
#define GFE_QUIC_TEST_TOOLS_MOCK_QUIC_CLIENT_PROMISED_INFO_H_

#include <string>

#include "base/macros.h"
#include "common/quic/core/quic_client_promised_info.h"
#include "common/quic/core/quic_packets.h"
#include "gmock/gmock.h"

namespace gfe_quic {
namespace test {

class MockQuicClientPromisedInfo : public QuicClientPromisedInfo {
 public:
  MockQuicClientPromisedInfo(QuicSpdyClientSessionBase* session,
                             QuicStreamId id,
                             string url);
  ~MockQuicClientPromisedInfo() override;

  MOCK_METHOD2(HandleClientRequest,
               QuicAsyncStatus(const gfe_spdy::SpdyHeaderBlock& headers,
                               QuicClientPushPromiseIndex::Delegate* delegate));
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_MOCK_QUIC_CLIENT_PROMISED_INFO_H_
