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

#ifndef GFE_QUIC_TEST_TOOLS_SIMULATOR_PORT_H_
#define GFE_QUIC_TEST_TOOLS_SIMULATOR_PORT_H_

#include <string>
#include <utility>

#include "common/quic/core/quic_packets.h"
#include "test/common/quic/test_tools/simulator/actor.h"

namespace gfe_quic {
namespace simulator {

struct Packet {
  Packet();
  ~Packet();
  Packet(const Packet& packet);

  string source;
  string destination;
  QuicTime tx_timestamp;

  string contents;
  QuicByteCount size;
};

// An interface for anything that accepts packets at arbitrary rate.
class UnconstrainedPortInterface {
 public:
  virtual ~UnconstrainedPortInterface() {}
  virtual void AcceptPacket(std::unique_ptr<Packet> packet) = 0;
};

// An interface for any device that accepts packets at a specific rate.
// Typically one would use a Queue object in order to write into a constrained
// port.
class ConstrainedPortInterface {
 public:
  virtual ~ConstrainedPortInterface() {}

  // Accept a packet for a port.  TimeUntilAvailable() must be zero before this
  // method is called.
  virtual void AcceptPacket(std::unique_ptr<Packet> packet) = 0;

  // Time until write for the next port is available.  Cannot be infinite.
  virtual QuicTime::Delta TimeUntilAvailable() = 0;
};

// A convenience class for any network endpoints, i.e. the objects which can
// both accept and send packets.
class Endpoint : public Actor {
 public:
  virtual UnconstrainedPortInterface* GetRxPort() = 0;
  virtual void SetTxPort(ConstrainedPortInterface* port) = 0;

 protected:
  Endpoint(Simulator* simulator, string name);
};

}  // namespace simulator
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_SIMULATOR_PORT_H_
