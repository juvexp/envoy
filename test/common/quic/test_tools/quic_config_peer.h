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

#ifndef GFE_QUIC_TEST_TOOLS_QUIC_CONFIG_PEER_H_
#define GFE_QUIC_TEST_TOOLS_QUIC_CONFIG_PEER_H_

#include "base/macros.h"
#include "common/quic/core/quic_config.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/platform/api/quic_uint128.h"

namespace gfe_quic {

class QuicConfig;

namespace test {

class QuicConfigPeer {
 public:
  static void SetReceivedSocketReceiveBuffer(QuicConfig* config,
                                             uint32_t receive_buffer_bytes);

  static void SetReceivedInitialStreamFlowControlWindow(QuicConfig* config,
                                                        uint32_t window_bytes);

  static void SetReceivedInitialSessionFlowControlWindow(QuicConfig* config,
                                                         uint32_t window_bytes);

  static void SetReceivedConnectionOptions(QuicConfig* config,
                                           const QuicTagVector& options);

  static void SetReceivedBytesForConnectionId(QuicConfig* config,
                                              uint32_t bytes);

  static void SetReceivedDisableConnectionMigration(QuicConfig* config);

  static void SetReceivedMaxIncomingDynamicStreams(QuicConfig* config,
                                                   uint32_t max_streams);

  static void SetConnectionOptionsToSend(QuicConfig* config,
                                         const QuicTagVector& options);

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicConfigPeer);
};

}  // namespace test

}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_QUIC_CONFIG_PEER_H_
