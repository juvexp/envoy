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

#include "test/common/quic/test_tools/mock_quic_time_wait_list_manager.h"

using testing::_;
using testing::Invoke;

namespace gfe_quic {
namespace test {

MockTimeWaitListManager::MockTimeWaitListManager(
    QuicPacketWriter* writer,
    Visitor* visitor,
    QuicConnectionHelperInterface* helper,
    QuicAlarmFactory* alarm_factory)
    : QuicTimeWaitListManager(writer, visitor, helper, alarm_factory) {
  // Though AddConnectionIdToTimeWait is mocked, we want to retain its
  // functionality.
  EXPECT_CALL(*this, AddConnectionIdToTimeWait(_, _, _, _))
      .Times(testing::AnyNumber());
  ON_CALL(*this, AddConnectionIdToTimeWait(_, _, _, _))
      .WillByDefault(
          Invoke(this, &MockTimeWaitListManager::
                           QuicTimeWaitListManager_AddConnectionIdToTimeWait));
}

MockTimeWaitListManager::~MockTimeWaitListManager() {}

}  // namespace test
}  // namespace gfe_quic
