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

#include "test/common/quic/test_tools/simulator/packet_filter.h"

namespace gfe_quic {
namespace simulator {

PacketFilter::PacketFilter(Simulator* simulator, string name, Endpoint* input)
    : Endpoint(simulator, name), input_(input) {
  input_->SetTxPort(this);
}

PacketFilter::~PacketFilter() {}

void PacketFilter::AcceptPacket(std::unique_ptr<Packet> packet) {
  if (FilterPacket(*packet)) {
    output_tx_port_->AcceptPacket(std::move(packet));
  }
}

QuicTime::Delta PacketFilter::TimeUntilAvailable() {
  return output_tx_port_->TimeUntilAvailable();
}

void PacketFilter::Act() {}

UnconstrainedPortInterface* PacketFilter::GetRxPort() {
  return input_->GetRxPort();
}

void PacketFilter::SetTxPort(ConstrainedPortInterface* port) {
  output_tx_port_ = port;
}

}  // namespace simulator
}  // namespace gfe_quic
