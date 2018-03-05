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

#include "test/common/quic/test_tools/quic_flow_controller_peer.h"

#include <list>

#include "common/quic/core/quic_flow_controller.h"
#include "common/quic/core/quic_packets.h"

namespace gfe_quic {
namespace test {

// static
void QuicFlowControllerPeer::SetSendWindowOffset(
    QuicFlowController* flow_controller,
    QuicStreamOffset offset) {
  flow_controller->send_window_offset_ = offset;
}

// static
void QuicFlowControllerPeer::SetReceiveWindowOffset(
    QuicFlowController* flow_controller,
    QuicStreamOffset offset) {
  flow_controller->receive_window_offset_ = offset;
}

// static
void QuicFlowControllerPeer::SetMaxReceiveWindow(
    QuicFlowController* flow_controller,
    QuicByteCount window_size) {
  flow_controller->receive_window_size_ = window_size;
}

// static
QuicStreamOffset QuicFlowControllerPeer::SendWindowOffset(
    QuicFlowController* flow_controller) {
  return flow_controller->send_window_offset_;
}

// static
QuicByteCount QuicFlowControllerPeer::SendWindowSize(
    QuicFlowController* flow_controller) {
  return flow_controller->SendWindowSize();
}

// static
QuicStreamOffset QuicFlowControllerPeer::ReceiveWindowOffset(
    QuicFlowController* flow_controller) {
  return flow_controller->receive_window_offset_;
}

// static
QuicByteCount QuicFlowControllerPeer::ReceiveWindowSize(
    QuicFlowController* flow_controller) {
  return flow_controller->receive_window_offset_ -
         flow_controller->highest_received_byte_offset_;
}

// static
QuicByteCount QuicFlowControllerPeer::WindowUpdateThreshold(
    QuicFlowController* flow_controller) {
  return flow_controller->WindowUpdateThreshold();
}

}  // namespace test
}  // namespace gfe_quic
