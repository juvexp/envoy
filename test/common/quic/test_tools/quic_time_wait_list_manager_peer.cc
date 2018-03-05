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

#include "test/common/quic/test_tools/quic_time_wait_list_manager_peer.h"

namespace gfe_quic {
namespace test {

bool QuicTimeWaitListManagerPeer::ShouldSendResponse(
    QuicTimeWaitListManager* manager,
    int received_packet_count) {
  return manager->ShouldSendResponse(received_packet_count);
}

QuicTime::Delta QuicTimeWaitListManagerPeer::time_wait_period(
    QuicTimeWaitListManager* manager) {
  return manager->time_wait_period_;
}

QuicTransportVersion
QuicTimeWaitListManagerPeer::GetQuicVersionFromConnectionId(
    QuicTimeWaitListManager* manager,
    QuicConnectionId connection_id) {
  return manager->GetQuicVersionFromConnectionId(connection_id)
      .transport_version;
}

QuicAlarm* QuicTimeWaitListManagerPeer::expiration_alarm(
    QuicTimeWaitListManager* manager) {
  return manager->connection_id_clean_up_alarm_.get();
}

void QuicTimeWaitListManagerPeer::set_clock(QuicTimeWaitListManager* manager,
                                            const QuicClock* clock) {
  manager->clock_ = clock;
}

}  // namespace test
}  // namespace gfe_quic
