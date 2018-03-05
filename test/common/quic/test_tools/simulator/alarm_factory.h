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

#ifndef GFE_QUIC_TEST_TOOLS_SIMULATOR_ALARM_FACTORY_H_
#define GFE_QUIC_TEST_TOOLS_SIMULATOR_ALARM_FACTORY_H_

#include "common/quic/core/quic_alarm_factory.h"
#include "test/common/quic/test_tools/simulator/actor.h"

namespace gfe_quic {
namespace simulator {

// AlarmFactory allows to schedule QuicAlarms using the simulation event queue.
class AlarmFactory : public QuicAlarmFactory {
 public:
  AlarmFactory(Simulator* simulator, string name);
  ~AlarmFactory() override;

  QuicAlarm* CreateAlarm(QuicAlarm::Delegate* delegate) override;
  QuicArenaScopedPtr<QuicAlarm> CreateAlarm(
      QuicArenaScopedPtr<QuicAlarm::Delegate> delegate,
      QuicConnectionArena* arena) override;

 private:
  // Automatically generate a name for a new alarm.
  string GetNewAlarmName();

  Simulator* simulator_;
  string name_;
  int counter_;

  DISALLOW_COPY_AND_ASSIGN(AlarmFactory);
};

}  // namespace simulator
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_SIMULATOR_ALARM_FACTORY_H_
