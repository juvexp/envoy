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

#ifndef GFE_QUIC_TEST_TOOLS_SIMULATOR_PACKET_FILTER_H_
#define GFE_QUIC_TEST_TOOLS_SIMULATOR_PACKET_FILTER_H_

#include "test/common/quic/test_tools/simulator/port.h"

namespace gfe_quic {
namespace simulator {

// Packet filter allows subclasses to filter out the packets that enter the
// input port and exit the output port.  Packets in the other direction are
// always passed through.
//
// The filter wraps around the input endpoint, and exposes the resulting
// filtered endpoint via the output() method.  For example, if initially there
// are two endpoints, A and B, connected via a symmetric link:
//
//   QuicEndpoint endpoint_a;
//   QuicEndpoint endpoint_b;
//
//   [...]
//
//   SymmetricLink a_b_link(&endpoint_a, &endpoint_b, ...);
//
// and the goal is to filter the traffic from A to B, then the new invocation
// would be as follows:
//
//   PacketFilter filter(&simulator, "A-to-B packet filter", endpoint_a);
//   SymmetricLink a_b_link(&filter, &endpoint_b, ...);
//
// Note that the filter drops the packet instanteneously, without it ever
// reaching the output wire.  This means that in a direct endpoint-to-endpoint
// scenario, whenever the packet is dropped, the link would become immediately
// available for the next packet.
class PacketFilter : public Endpoint, public ConstrainedPortInterface {
 public:
  // Initialize the filter by wrapping around |input|.  Does not take the
  // ownership of |input|.
  PacketFilter(Simulator* simulator, string name, Endpoint* input);
  ~PacketFilter() override;

  // Implementation of ConstrainedPortInterface.
  void AcceptPacket(std::unique_ptr<Packet> packet) override;
  QuicTime::Delta TimeUntilAvailable() override;

  // Implementation of Endpoint interface methods.
  UnconstrainedPortInterface* GetRxPort() override;
  void SetTxPort(ConstrainedPortInterface* port) override;

  // Implementation of Actor interface methods.
  void Act() override;

 protected:
  // Returns true if the packet should be passed through, and false if it should
  // be dropped.  The function is called once per packet, in the order that the
  // packets arrive, so it is safe for the function to alter the internal state
  // of the filter.
  virtual bool FilterPacket(const Packet& packet) = 0;

 private:
  // The port onto which the filtered packets are egressed.
  ConstrainedPortInterface* output_tx_port_;

  // The original network endpoint wrapped by the class.
  Endpoint* input_;

  DISALLOW_COPY_AND_ASSIGN(PacketFilter);
};

}  // namespace simulator
}  // namespace gfe_quic
#endif  // GFE_QUIC_TEST_TOOLS_SIMULATOR_PACKET_FILTER_H_
