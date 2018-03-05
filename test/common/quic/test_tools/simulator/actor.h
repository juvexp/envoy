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

#ifndef GFE_QUIC_TEST_TOOLS_SIMULATOR_ACTOR_H_
#define GFE_QUIC_TEST_TOOLS_SIMULATOR_ACTOR_H_

#include <string>

#include "common/quic/core/quic_time.h"
#include "common/quic/platform/api/quic_clock.h"

namespace gfe_quic {
namespace simulator {

class Simulator;

// Actor is the base class for all participants of the simulation which can
// schedule events to be triggered at the specified time.  Every actor has a
// name assigned to it, which can be used for debugging and addressing purposes.
//
// The Actor object is scheduled as follows:
// 1. Every Actor object appears at most once in the event queue, for one
//    specific time.
// 2. Actor is scheduled by calling Schedule() method.
// 3. If Schedule() method is called with multiple different times specified,
//    Act() method will be called at the earliest time specified.
// 4. Before Act() is called, the Actor is removed from the event queue.  Act()
//    will not be called again unless Schedule() is called.
class Actor {
 public:
  Actor(Simulator* simulator, string name);
  virtual ~Actor();

  // Trigger all the events the actor can potentially handle at this point.
  // Before Act() is called, the actor is removed from the event queue, and has
  // to schedule the next call manually.
  virtual void Act() = 0;

  inline string name() const { return name_; }
  inline Simulator* simulator() const { return simulator_; }

 protected:
  // Calls Schedule() on the associated simulator.
  void Schedule(QuicTime next_tick);

  // Calls Unschedule() on the associated simulator.
  void Unschedule();

  Simulator* simulator_;
  const QuicClock* clock_;
  string name_;

 private:
  // Since the Actor object registers itself with a simulator using a pointer to
  // itself, do not allow it to be moved.
  Actor(Actor&&) = delete;
  Actor& operator=(Actor&&) = delete;

  DISALLOW_COPY_AND_ASSIGN(Actor);
};

}  // namespace simulator
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_SIMULATOR_ACTOR_H_
