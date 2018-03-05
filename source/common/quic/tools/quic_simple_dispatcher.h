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

#ifndef GFE_QUIC_TOOLS_QUIC_SIMPLE_DISPATCHER_H_
#define GFE_QUIC_TOOLS_QUIC_SIMPLE_DISPATCHER_H_

#include "common/quic/core/quic_dispatcher.h"
#include "common/quic/core/quic_server_session_base.h"
#include "common/quic/tools/quic_http_response_cache.h"

namespace gfe_quic {

class QuicSimpleDispatcher : public QuicDispatcher {
 public:
  QuicSimpleDispatcher(
      const QuicConfig& config,
      const QuicCryptoServerConfig* crypto_config,
      QuicVersionManager* version_manager,
      std::unique_ptr<QuicConnectionHelperInterface> helper,
      std::unique_ptr<QuicCryptoServerStream::Helper> session_helper,
      std::unique_ptr<QuicAlarmFactory> alarm_factory,
      QuicHttpResponseCache* response_cache);

  ~QuicSimpleDispatcher() override;

  int GetRstErrorCount(QuicRstStreamErrorCode rst_error_code) const;

  void OnRstStreamReceived(const QuicRstStreamFrame& frame) override;

 protected:
  QuicServerSessionBase* CreateQuicSession(
      QuicConnectionId connection_id,
      const QuicSocketAddress& client_address,
      QuicStringPiece alpn) override;

  QuicHttpResponseCache* response_cache() { return response_cache_; }

 private:
  QuicHttpResponseCache* response_cache_;  // Unowned.

  // The map of the reset error code with its counter.
  std::map<QuicRstStreamErrorCode, int> rst_error_map_;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_TOOLS_QUIC_SIMPLE_DISPATCHER_H_
