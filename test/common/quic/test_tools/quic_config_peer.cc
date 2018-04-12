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

#include "test/common/quic/test_tools/quic_config_peer.h"

#include "common/quic/core/quic_config.h"

namespace gfe_quic {
namespace test {

// static
void QuicConfigPeer::SetReceivedInitialStreamFlowControlWindow(
    QuicConfig* config,
    uint32_t window_bytes) {
  config->initial_stream_flow_control_window_bytes_.SetReceivedValue(
      window_bytes);
}

// static
void QuicConfigPeer::SetReceivedInitialSessionFlowControlWindow(
    QuicConfig* config,
    uint32_t window_bytes) {
  config->initial_session_flow_control_window_bytes_.SetReceivedValue(
      window_bytes);
}

// static
void QuicConfigPeer::SetReceivedConnectionOptions(
    QuicConfig* config,
    const QuicTagVector& options) {
  config->connection_options_.SetReceivedValues(options);
}

// static
void QuicConfigPeer::SetReceivedBytesForConnectionId(QuicConfig* config,
                                                     uint32_t bytes) {
  DCHECK(bytes == 0 || bytes == 8);
  config->bytes_for_connection_id_.SetReceivedValue(bytes);
}

// static
void QuicConfigPeer::SetReceivedDisableConnectionMigration(QuicConfig* config) {
  config->connection_migration_disabled_.SetReceivedValue(1);
}

// static
void QuicConfigPeer::SetReceivedMaxIncomingDynamicStreams(
    QuicConfig* config,
    uint32_t max_streams) {
  config->max_incoming_dynamic_streams_.SetReceivedValue(max_streams);
}

// static
void QuicConfigPeer::SetConnectionOptionsToSend(QuicConfig* config,
                                                const QuicTagVector& options) {
  config->SetConnectionOptionsToSend(options);
}

// static
void QuicConfigPeer::SetReceivedStatelessResetToken(QuicConfig* config,
                                                    absl::uint128 token) {
  config->stateless_reset_token_.SetReceivedValue(token);
}

}  // namespace test
}  // namespace gfe_quic
