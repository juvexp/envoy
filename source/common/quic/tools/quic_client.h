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

// A toy client, which connects to a specified port and sends QUIC
// request to that endpoint.

#ifndef GFE_QUIC_TOOLS_QUIC_CLIENT_H_
#define GFE_QUIC_TOOLS_QUIC_CLIENT_H_

#include <cstdint>
#include <memory>
#include <string>

#include "base/macros.h"
#include "gfe/gfe2/base/epoll_server.h"
#include "common/quic/core/quic_client_push_promise_index.h"
#include "common/quic/core/quic_config.h"
#include "common/quic/core/quic_packet_reader.h"
#include "common/quic/core/quic_process_packet_interface.h"
#include "common/quic/core/quic_spdy_client_session.h"
#include "common/quic/core/quic_spdy_client_stream.h"
#include "common/quic/platform/api/quic_containers.h"
#include "common/quic/tools/quic_client_epoll_network_helper.h"
#include "common/quic/tools/quic_spdy_client_base.h"

namespace gfe_quic {

class QuicServerId;

namespace test {
class QuicClientPeer;
}  // namespace test

class QuicClient : public QuicSpdyClientBase {
 public:
  // This will create its own QuicClientEpollNetworkHelper.
  QuicClient(QuicSocketAddress server_address,
             const QuicServerId& server_id,
             const ParsedQuicVersionVector& supported_versions,
             gfe2::EpollServer* epoll_server,
             std::unique_ptr<ProofVerifier> proof_verifier);
  // This will take ownership of a passed in network primitive.
  QuicClient(QuicSocketAddress server_address,
             const QuicServerId& server_id,
             const ParsedQuicVersionVector& supported_versions,
             gfe2::EpollServer* epoll_server,
             std::unique_ptr<QuicClientEpollNetworkHelper> network_helper,
             std::unique_ptr<ProofVerifier> proof_verifier);
  QuicClient(QuicSocketAddress server_address,
             const QuicServerId& server_id,
             const ParsedQuicVersionVector& supported_versions,
             const QuicConfig& config,
             gfe2::EpollServer* epoll_server,
             std::unique_ptr<QuicClientEpollNetworkHelper> network_helper,
             std::unique_ptr<ProofVerifier> proof_verifier);

  ~QuicClient() override;

  // Exposed for the quic client test.
  int GetLatestFD() const { return epoll_network_helper()->GetLatestFD(); }

  QuicClientEpollNetworkHelper* epoll_network_helper();
  const QuicClientEpollNetworkHelper* epoll_network_helper() const;

 private:
  friend class test::QuicClientPeer;

  DISALLOW_COPY_AND_ASSIGN(QuicClient);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_TOOLS_QUIC_CLIENT_H_
