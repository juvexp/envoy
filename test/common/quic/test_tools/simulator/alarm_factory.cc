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

#include "test/common/quic/test_tools/simulator/alarm_factory.h"

#include "common/quic/core/quic_alarm.h"
#include "common/quic/platform/api/quic_str_cat.h"

namespace gfe_quic {
namespace simulator {

// Alarm is an implementation of QuicAlarm which can schedule alarms in the
// simulation timeline.
class Alarm : public QuicAlarm {
 public:
  Alarm(Simulator* simulator,
        string name,
        QuicArenaScopedPtr<QuicAlarm::Delegate> delegate)
      : QuicAlarm(std::move(delegate)), adapter_(simulator, name, this) {}
  ~Alarm() override {}

  void SetImpl() override {
    DCHECK(deadline().IsInitialized());
    adapter_.Set(deadline());
  }

  void CancelImpl() override { adapter_.Cancel(); }

 private:
  // An adapter class triggering a QuicAlarm using a simulation time system.
  // An adapter is required here because neither Actor nor QuicAlarm are pure
  // interfaces.
  class Adapter : public Actor {
   public:
    Adapter(Simulator* simulator, string name, Alarm* parent)
        : Actor(simulator, name), parent_(parent) {}
    ~Adapter() override {}

    void Set(QuicTime time) { Schedule(time); }
    void Cancel() { Unschedule(); }

    void Act() override {
      DCHECK(clock_->Now() == parent_->deadline());
      parent_->Fire();
    }

   private:
    Alarm* parent_;
  };
  Adapter adapter_;
};

AlarmFactory::AlarmFactory(Simulator* simulator, string name)
    : simulator_(simulator), name_(std::move(name)), counter_(0) {}

AlarmFactory::~AlarmFactory() {}

string AlarmFactory::GetNewAlarmName() {
  ++counter_;
  return QuicStringPrintf("%s (alarm %i)", name_.c_str(), counter_);
}

QuicAlarm* AlarmFactory::CreateAlarm(QuicAlarm::Delegate* delegate) {
  return new Alarm(simulator_, GetNewAlarmName(),
                   QuicArenaScopedPtr<QuicAlarm::Delegate>(delegate));
}

QuicArenaScopedPtr<QuicAlarm> AlarmFactory::CreateAlarm(
    QuicArenaScopedPtr<QuicAlarm::Delegate> delegate,
    QuicConnectionArena* arena) {
  if (arena != nullptr) {
    return arena->New<Alarm>(simulator_, GetNewAlarmName(),
                             std::move(delegate));
  }
  return QuicArenaScopedPtr<QuicAlarm>(
      new Alarm(simulator_, GetNewAlarmName(), std::move(delegate)));
}

}  // namespace simulator
}  // namespace gfe_quic
