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

#ifndef GFE_QUIC_TEST_TOOLS_MOCK_CLOCK_H_
#define GFE_QUIC_TEST_TOOLS_MOCK_CLOCK_H_

#include "base/macros.h"
#include "gfe/gfe2/test_tools/mock_epoll_server.h"
#include "common/quic/platform/impl/quic_epoll_clock.h"

namespace gfe_quic {

class MockClock : public QuicEpollClock {
 public:
  MockClock();
  ~MockClock() override;

  void AdvanceTime(QuicTime::Delta delta);

 private:
  gfe2::test::MockEpollServer epoll_server_;

  DISALLOW_COPY_AND_ASSIGN(MockClock);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_MOCK_CLOCK_H_
