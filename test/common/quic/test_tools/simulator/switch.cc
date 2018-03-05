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

#include <cinttypes>

#include "base/port.h"
#include "common/quic/platform/api/quic_ptr_util.h"
#include "common/quic/platform/api/quic_str_cat.h"
#include "test/common/quic/test_tools/simulator/switch.h"

namespace gfe_quic {
namespace simulator {

Switch::Switch(Simulator* simulator,
               string name,
               SwitchPortNumber port_count,
               QuicByteCount queue_capacity) {
  for (size_t port_number = 1; port_number <= port_count; port_number++) {
    ports_.emplace_back(simulator, QuicStringPrintf("%s (port %" PRIuS ")",
                                                    name.c_str(), port_number),
                        this, port_number, queue_capacity);
  }
}

Switch::~Switch() {}

Switch::Port::Port(Simulator* simulator,
                   string name,
                   Switch* parent,
                   SwitchPortNumber port_number,
                   QuicByteCount queue_capacity)
    : Endpoint(simulator, name),
      parent_(parent),
      port_number_(port_number),
      connected_(false),
      queue_(simulator,
             QuicStringPrintf("%s (queue)", name.c_str()),
             queue_capacity) {}

void Switch::Port::AcceptPacket(std::unique_ptr<Packet> packet) {
  parent_->DispatchPacket(port_number_, std::move(packet));
}

void Switch::Port::EnqueuePacket(std::unique_ptr<Packet> packet) {
  queue_.AcceptPacket(std::move(packet));
}

UnconstrainedPortInterface* Switch::Port::GetRxPort() {
  return this;
}

void Switch::Port::SetTxPort(ConstrainedPortInterface* port) {
  queue_.set_tx_port(port);
  connected_ = true;
}

void Switch::Port::Act() {}

void Switch::DispatchPacket(SwitchPortNumber port_number,
                            std::unique_ptr<Packet> packet) {
  Port* source_port = &ports_[port_number - 1];
  const auto source_mapping_it = switching_table_.find(packet->source);
  if (source_mapping_it == switching_table_.end()) {
    switching_table_.insert(std::make_pair(packet->source, source_port));
  }

  const auto destination_mapping_it =
      switching_table_.find(packet->destination);
  if (destination_mapping_it != switching_table_.end()) {
    destination_mapping_it->second->EnqueuePacket(std::move(packet));
    return;
  }

  // If no mapping is available yet, broadcast the packet to all ports
  // different from the source.
  for (Port& egress_port : ports_) {
    if (!egress_port.connected()) {
      continue;
    }
    egress_port.EnqueuePacket(QuicMakeUnique<Packet>(*packet));
  }
}

}  // namespace simulator
}  // namespace gfe_quic
