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

#ifndef GFE_QUIC_CORE_QUIC_ALARM_FACTORY_H_
#define GFE_QUIC_CORE_QUIC_ALARM_FACTORY_H_

#include "common/quic/core/quic_alarm.h"
#include "common/quic/core/quic_one_block_arena.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

// QuicConnections currently use around 1KB of polymorphic types which would
// ordinarily be on the heap. Instead, store them inline in an arena.
using QuicConnectionArena = QuicOneBlockArena<1024>;

// Creates platform-specific alarms used throughout QUIC.
class QUIC_EXPORT_PRIVATE QuicAlarmFactory {
 public:
  virtual ~QuicAlarmFactory() {}

  // Creates a new platform-specific alarm which will be configured to notify
  // |delegate| when the alarm fires. Returns an alarm allocated on the heap.
  // Caller takes ownership of the new alarm, which will not yet be "set" to
  // fire.
  virtual QuicAlarm* CreateAlarm(QuicAlarm::Delegate* delegate) = 0;

  // Creates a new platform-specific alarm which will be configured to notify
  // |delegate| when the alarm fires. Caller takes ownership of the new alarm,
  // which will not yet be "set" to fire. If |arena| is null, then the alarm
  // will be created on the heap. Otherwise, it will be created in |arena|.
  virtual QuicArenaScopedPtr<QuicAlarm> CreateAlarm(
      QuicArenaScopedPtr<QuicAlarm::Delegate> delegate,
      QuicConnectionArena* arena) = 0;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_ALARM_FACTORY_H_
