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

#ifndef GFE_QUIC_TEST_TOOLS_QUIC_FLOW_CONTROLLER_PEER_H_
#define GFE_QUIC_TEST_TOOLS_QUIC_FLOW_CONTROLLER_PEER_H_

#include "base/macros.h"
#include "common/quic/core/quic_packets.h"

namespace gfe_quic {

class QuicFlowController;

namespace test {

class QuicFlowControllerPeer {
 public:
  static void SetSendWindowOffset(QuicFlowController* flow_controller,
                                  QuicStreamOffset offset);

  static void SetReceiveWindowOffset(QuicFlowController* flow_controller,
                                     QuicStreamOffset offset);

  static void SetMaxReceiveWindow(QuicFlowController* flow_controller,
                                  QuicByteCount window_size);

  static QuicStreamOffset SendWindowOffset(QuicFlowController* flow_controller);

  static QuicByteCount SendWindowSize(QuicFlowController* flow_controller);

  static QuicStreamOffset ReceiveWindowOffset(
      QuicFlowController* flow_controller);

  static QuicByteCount ReceiveWindowSize(QuicFlowController* flow_controller);

  static QuicByteCount WindowUpdateThreshold(
      QuicFlowController* flow_controller);

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicFlowControllerPeer);
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_QUIC_FLOW_CONTROLLER_PEER_H_
