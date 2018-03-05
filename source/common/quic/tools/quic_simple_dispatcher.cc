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

#include "common/quic/tools/quic_simple_dispatcher.h"

#include "common/quic/tools/quic_simple_server_session.h"

namespace gfe_quic {

QuicSimpleDispatcher::QuicSimpleDispatcher(
    const QuicConfig& config,
    const QuicCryptoServerConfig* crypto_config,
    QuicVersionManager* version_manager,
    std::unique_ptr<QuicConnectionHelperInterface> helper,
    std::unique_ptr<QuicCryptoServerStream::Helper> session_helper,
    std::unique_ptr<QuicAlarmFactory> alarm_factory,
    QuicHttpResponseCache* response_cache)
    : QuicDispatcher(config,
                     crypto_config,
                     version_manager,
                     std::move(helper),
                     std::move(session_helper),
                     std::move(alarm_factory)),
      response_cache_(response_cache) {}

QuicSimpleDispatcher::~QuicSimpleDispatcher() {}

int QuicSimpleDispatcher::GetRstErrorCount(
    QuicRstStreamErrorCode error_code) const {
  auto it = rst_error_map_.find(error_code);
  if (it == rst_error_map_.end()) {
    return 0;
  }
  return it->second;
}

void QuicSimpleDispatcher::OnRstStreamReceived(
    const QuicRstStreamFrame& frame) {
  auto it = rst_error_map_.find(frame.error_code);
  if (it == rst_error_map_.end()) {
    rst_error_map_.insert(std::make_pair(frame.error_code, 1));
  } else {
    it->second++;
  }
}

QuicServerSessionBase* QuicSimpleDispatcher::CreateQuicSession(
    QuicConnectionId connection_id,
    const QuicSocketAddress& client_address,
    QuicStringPiece /*alpn*/) {
  // The QuicServerSessionBase takes ownership of |connection| below.
  QuicConnection* connection = new QuicConnection(
      connection_id, client_address, helper(), alarm_factory(),
      CreatePerConnectionWriter(),
      /* owns_writer= */ true, Perspective::IS_SERVER, GetSupportedVersions());

  QuicServerSessionBase* session = new QuicSimpleServerSession(
      config(), connection, this, session_helper(), crypto_config(),
      compressed_certs_cache(), response_cache_);
  session->Initialize();
  return session;
}

}  // namespace gfe_quic
