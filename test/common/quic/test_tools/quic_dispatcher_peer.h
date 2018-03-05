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

#ifndef GFE_QUIC_TEST_TOOLS_QUIC_DISPATCHER_PEER_H_
#define GFE_QUIC_TEST_TOOLS_QUIC_DISPATCHER_PEER_H_

#include "base/macros.h"
#include "common/quic/core/quic_dispatcher.h"

namespace gfe_quic {

class QuicPacketWriterWrapper;

namespace test {

class QuicDispatcherPeer {
 public:
  static void SetTimeWaitListManager(
      QuicDispatcher* dispatcher,
      QuicTimeWaitListManager* time_wait_list_manager);

  // Injects |writer| into |dispatcher| as the shared writer.
  static void UseWriter(QuicDispatcher* dispatcher,
                        QuicPacketWriterWrapper* writer);

  static QuicPacketWriter* GetWriter(QuicDispatcher* dispatcher);

  static QuicCompressedCertsCache* GetCache(QuicDispatcher* dispatcher);

  static QuicConnectionHelperInterface* GetHelper(QuicDispatcher* dispatcher);

  static QuicAlarmFactory* GetAlarmFactory(QuicDispatcher* dispatcher);

  static QuicDispatcher::WriteBlockedList* GetWriteBlockedList(
      QuicDispatcher* dispatcher);

  // Get the dispatcher's record of the last error reported to its framer
  // visitor's OnError() method.  Then set that record to QUIC_NO_ERROR.
  static QuicErrorCode GetAndClearLastError(QuicDispatcher* dispatcher);

  static QuicBufferedPacketStore* GetBufferedPackets(
      QuicDispatcher* dispatcher);

  static const QuicDispatcher::SessionMap& session_map(
      QuicDispatcher* dispatcher);

  static void set_new_sessions_allowed_per_event_loop(
      QuicDispatcher* dispatcher,
      size_t num_session_allowed);

  static void SendPublicReset(QuicDispatcher* dispatcher,
                              const QuicSocketAddress& server_address,
                              const QuicSocketAddress& client_address,
                              QuicConnectionId connection_id);

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicDispatcherPeer);
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_QUIC_DISPATCHER_PEER_H_
