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

#include "test/common/quic/test_tools/quic_client_peer.h"

#include "common/quic/tools/quic_client.h"

namespace gfe_quic {
namespace test {

// static
bool QuicClientPeer::CreateUDPSocketAndBind(QuicClient* client) {
  return client->network_helper()->CreateUDPSocketAndBind(
      client->server_address(), client->bind_to_address(),
      client->local_port());
}

// static
void QuicClientPeer::CleanUpUDPSocket(QuicClient* client, int fd) {
  client->epoll_network_helper()->CleanUpUDPSocket(fd);
}

// static
void QuicClientPeer::SetClientPort(QuicClient* client, int port) {
  client->epoll_network_helper()->SetClientPort(port);
}

// static
void QuicClientPeer::SetWriter(QuicClient* client, QuicPacketWriter* writer) {
  client->set_writer(writer);
}

}  // namespace test
}  // namespace gfe_quic
