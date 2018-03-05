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

#ifndef GFE_QUIC_TEST_TOOLS_SIMULATOR_LINK_H_
#define GFE_QUIC_TEST_TOOLS_SIMULATOR_LINK_H_

#include <utility>

#include "common/quic/core/crypto/quic_random.h"
#include "test/common/quic/test_tools/simulator/actor.h"
#include "test/common/quic/test_tools/simulator/port.h"

namespace gfe_quic {
namespace simulator {

// A reliable simplex link between two endpoints with constrained bandwidth.  A
// few microseconds of random delay are added for every packet to avoid
// synchronization issues.
class OneWayLink : public Actor, public ConstrainedPortInterface {
 public:
  OneWayLink(Simulator* simulator,
             string name,
             UnconstrainedPortInterface* sink,
             QuicBandwidth bandwidth,
             QuicTime::Delta propagation_delay);
  ~OneWayLink() override;

  void AcceptPacket(std::unique_ptr<Packet> packet) override;
  QuicTime::Delta TimeUntilAvailable() override;
  void Act() override;

  inline QuicBandwidth bandwidth() const { return bandwidth_; }

 private:
  struct QueuedPacket {
    std::unique_ptr<Packet> packet;
    QuicTime dequeue_time;

    QueuedPacket(std::unique_ptr<Packet> packet, QuicTime dequeue_time);
    QueuedPacket(QueuedPacket&& other);
    ~QueuedPacket();
  };

  // Schedule the next packet to be egressed out of the link if there are
  // packets on the link.
  void ScheduleNextPacketDeparture();

  // Get the value of a random delay imposed on each packet in order to avoid
  // artifical synchronization artifacts during the simulation.
  QuicTime::Delta GetRandomDelay(QuicTime::Delta transfer_time);

  UnconstrainedPortInterface* sink_;
  QuicQueue<QueuedPacket> packets_in_transit_;

  const QuicBandwidth bandwidth_;
  const QuicTime::Delta propagation_delay_;

  QuicTime next_write_at_;

  DISALLOW_COPY_AND_ASSIGN(OneWayLink);
};

// A full-duplex link between two endpoints, functionally equivalent to two
// OneWayLink objects tied together.
class SymmetricLink {
 public:
  SymmetricLink(Simulator* simulator,
                string name,
                UnconstrainedPortInterface* sink_a,
                UnconstrainedPortInterface* sink_b,
                QuicBandwidth bandwidth,
                QuicTime::Delta propagation_delay);
  SymmetricLink(Endpoint* endpoint_a,
                Endpoint* endpoint_b,
                QuicBandwidth bandwidth,
                QuicTime::Delta propagation_delay);

  inline QuicBandwidth bandwidth() { return a_to_b_link_.bandwidth(); }

 private:
  OneWayLink a_to_b_link_;
  OneWayLink b_to_a_link_;

  DISALLOW_COPY_AND_ASSIGN(SymmetricLink);
};

}  // namespace simulator
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_SIMULATOR_LINK_H_
