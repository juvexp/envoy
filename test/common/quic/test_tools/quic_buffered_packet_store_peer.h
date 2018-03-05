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

#ifndef GFE_QUIC_TEST_TOOLS_QUIC_BUFFERED_PACKET_STORE_PEER_H_
#define GFE_QUIC_TEST_TOOLS_QUIC_BUFFERED_PACKET_STORE_PEER_H_

#include <memory>

#include "common/quic/core/quic_alarm.h"
#include "common/quic/platform/api/quic_clock.h"

namespace gfe_quic {

class QuicBufferedPacketStore;

namespace test {

class QuicBufferedPacketStorePeer {
 public:
  static QuicAlarm* expiration_alarm(QuicBufferedPacketStore* store);

  static void set_clock(QuicBufferedPacketStore* store, const QuicClock* clock);

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicBufferedPacketStorePeer);
};

}  // namespace test

}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_QUIC_BUFFERED_PACKET_STORE_PEER_H_
