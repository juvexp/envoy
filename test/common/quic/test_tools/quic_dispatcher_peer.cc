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

#include "test/common/quic/test_tools/quic_dispatcher_peer.h"

#include "common/quic/core/quic_dispatcher.h"
#include "common/quic/core/quic_packet_writer_wrapper.h"

namespace gfe_quic {
namespace test {

// static
void QuicDispatcherPeer::SetTimeWaitListManager(
    QuicDispatcher* dispatcher,
    QuicTimeWaitListManager* time_wait_list_manager) {
  dispatcher->time_wait_list_manager_.reset(time_wait_list_manager);
}

// static
void QuicDispatcherPeer::UseWriter(QuicDispatcher* dispatcher,
                                   QuicPacketWriterWrapper* writer) {
  writer->set_writer(dispatcher->writer_.release());
  dispatcher->writer_.reset(writer);
}

// static
QuicPacketWriter* QuicDispatcherPeer::GetWriter(QuicDispatcher* dispatcher) {
  return dispatcher->writer_.get();
}

// static
QuicCompressedCertsCache* QuicDispatcherPeer::GetCache(
    QuicDispatcher* dispatcher) {
  return dispatcher->compressed_certs_cache();
}

// static
QuicConnectionHelperInterface* QuicDispatcherPeer::GetHelper(
    QuicDispatcher* dispatcher) {
  return dispatcher->helper_.get();
}

// static
QuicAlarmFactory* QuicDispatcherPeer::GetAlarmFactory(
    QuicDispatcher* dispatcher) {
  return dispatcher->alarm_factory_.get();
}

// static
QuicDispatcher::WriteBlockedList* QuicDispatcherPeer::GetWriteBlockedList(
    QuicDispatcher* dispatcher) {
  return &dispatcher->write_blocked_list_;
}

// static
QuicErrorCode QuicDispatcherPeer::GetAndClearLastError(
    QuicDispatcher* dispatcher) {
  QuicErrorCode ret = dispatcher->last_error_;
  dispatcher->last_error_ = QUIC_NO_ERROR;
  return ret;
}

// static
QuicBufferedPacketStore* QuicDispatcherPeer::GetBufferedPackets(
    QuicDispatcher* dispatcher) {
  return &(dispatcher->buffered_packets_);
}

// static
const QuicDispatcher::SessionMap& QuicDispatcherPeer::session_map(
    QuicDispatcher* dispatcher) {
  return dispatcher->session_map();
}

// static
void QuicDispatcherPeer::set_new_sessions_allowed_per_event_loop(
    QuicDispatcher* dispatcher,
    size_t num_session_allowed) {
  return dispatcher->set_new_sessions_allowed_per_event_loop(
      num_session_allowed);
}

// static
void QuicDispatcherPeer::SendPublicReset(
    QuicDispatcher* dispatcher,
    const QuicSocketAddress& server_address,
    const QuicSocketAddress& client_address,
    QuicConnectionId connection_id) {
  dispatcher->time_wait_list_manager()->SendPublicReset(
      server_address, client_address, connection_id);
}

}  // namespace test
}  // namespace gfe_quic
