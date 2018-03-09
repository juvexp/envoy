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

#ifndef GFE_QUIC_TEST_TOOLS_MOCK_QUIC_TIME_WAIT_LIST_MANAGER_H_
#define GFE_QUIC_TEST_TOOLS_MOCK_QUIC_TIME_WAIT_LIST_MANAGER_H_

#include "common/quic/core/quic_time_wait_list_manager.h"
#include "gmock/gmock.h"

namespace gfe_quic {
namespace test {

class MockTimeWaitListManager : public QuicTimeWaitListManager {
 public:
  MockTimeWaitListManager(QuicPacketWriter* writer,
                          Visitor* visitor,
                          QuicConnectionHelperInterface* helper,
                          QuicAlarmFactory* alarm_factory);
  ~MockTimeWaitListManager() override;

  MOCK_METHOD4(AddConnectionIdToTimeWait,
               void(QuicConnectionId connection_id,
                    ParsedQuicVersion version,
                    bool connection_rejected_statelessly,
                    std::vector<std::unique_ptr<QuicEncryptedPacket>>*
                        termination_packets));

  void QuicTimeWaitListManager_AddConnectionIdToTimeWait(
      QuicConnectionId connection_id,
      ParsedQuicVersion version,
      bool connection_rejected_statelessly,
      std::vector<std::unique_ptr<QuicEncryptedPacket>>* termination_packets) {
    QuicTimeWaitListManager::AddConnectionIdToTimeWait(
        connection_id, version, connection_rejected_statelessly,
        termination_packets);
  }

  MOCK_METHOD3(ProcessPacket,
               void(const QuicSocketAddress& server_address,
                    const QuicSocketAddress& client_address,
                    QuicConnectionId connection_id));

  MOCK_METHOD4(SendVersionNegotiationPacket,
               void(QuicConnectionId connection_id,
                    const ParsedQuicVersionVector& supported_versions,
                    const QuicSocketAddress& server_address,
                    const QuicSocketAddress& client_address));
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_MOCK_QUIC_TIME_WAIT_LIST_MANAGER_H_
