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

#include "common/quic/tools/quic_client.h"

#include <errno.h>
#include <features.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <iostream>

#include "common/quic/core/crypto/quic_random.h"
#include "common/quic/core/quic_connection.h"
#include "common/quic/core/quic_data_reader.h"
#include "common/quic/core/quic_epoll_alarm_factory.h"
#include "common/quic/core/quic_epoll_connection_helper.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_server_id.h"
#include "common/quic/core/spdy_utils.h"
#include "common/quic/platform/api/quic_bug_tracker.h"
#include "common/quic/platform/api/quic_logging.h"
#include "common/quic/platform/api/quic_ptr_util.h"
#include "common/quic/platform/impl/quic_socket_utils.h"

#ifndef SO_RXQ_OVFL
#define SO_RXQ_OVFL 40
#endif

namespace gfe_quic {

QuicClient::QuicClient(QuicSocketAddress server_address,
                       const QuicServerId& server_id,
                       const ParsedQuicVersionVector& supported_versions,
                       gfe2::EpollServer* epoll_server,
                       std::unique_ptr<ProofVerifier> proof_verifier)
    : QuicClient(
          server_address,
          server_id,
          supported_versions,
          QuicConfig(),
          epoll_server,
          QuicWrapUnique(new QuicClientEpollNetworkHelper(epoll_server, this)),
          std::move(proof_verifier)) {}

QuicClient::QuicClient(
    QuicSocketAddress server_address,
    const QuicServerId& server_id,
    const ParsedQuicVersionVector& supported_versions,
    gfe2::EpollServer* epoll_server,
    std::unique_ptr<QuicClientEpollNetworkHelper> network_helper,
    std::unique_ptr<ProofVerifier> proof_verifier)
    : QuicClient(server_address,
                 server_id,
                 supported_versions,
                 QuicConfig(),
                 epoll_server,
                 std::move(network_helper),
                 std::move(proof_verifier)) {}

QuicClient::QuicClient(
    QuicSocketAddress server_address,
    const QuicServerId& server_id,
    const ParsedQuicVersionVector& supported_versions,
    const QuicConfig& config,
    gfe2::EpollServer* epoll_server,
    std::unique_ptr<QuicClientEpollNetworkHelper> network_helper,
    std::unique_ptr<ProofVerifier> proof_verifier)
    : QuicSpdyClientBase(
          server_id,
          supported_versions,
          config,
          new QuicEpollConnectionHelper(epoll_server, QuicAllocator::SIMPLE),
          new QuicEpollAlarmFactory(epoll_server),
          std::move(network_helper),
          std::move(proof_verifier)) {
  set_server_address(server_address);
}

QuicClient::~QuicClient() {}

QuicClientEpollNetworkHelper* QuicClient::epoll_network_helper() {
  return down_cast<QuicClientEpollNetworkHelper*>(network_helper());
}

const QuicClientEpollNetworkHelper* QuicClient::epoll_network_helper() const {
  return down_cast<const QuicClientEpollNetworkHelper*>(network_helper());
}

}  // namespace gfe_quic
