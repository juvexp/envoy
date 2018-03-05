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

#ifndef GFE_QUIC_CORE_QUIC_EPOLL_ALARM_FACTORY_H_
#define GFE_QUIC_CORE_QUIC_EPOLL_ALARM_FACTORY_H_

#include "gfe/gfe2/base/epoll_server.h"
#include "common/quic/core/quic_alarm.h"
#include "common/quic/core/quic_alarm_factory.h"
#include "common/quic/core/quic_one_block_arena.h"

namespace gfe_quic {

// QuicConnections currently use around 1KB of polymorphic types which would
// ordinarily be on the heap. Instead, store them inline in an arena.
using QuicConnectionArena = QuicOneBlockArena<1024>;

// Creates alarms that use the supplied EpollServer for timing and firing.
class QuicEpollAlarmFactory : public QuicAlarmFactory {
 public:
  explicit QuicEpollAlarmFactory(gfe2::EpollServer* eps);
  ~QuicEpollAlarmFactory() override;

  // QuicAlarmFactory interface.
  QuicAlarm* CreateAlarm(QuicAlarm::Delegate* delegate) override;
  QuicArenaScopedPtr<QuicAlarm> CreateAlarm(
      QuicArenaScopedPtr<QuicAlarm::Delegate> delegate,
      QuicConnectionArena* arena) override;

 private:
  gfe2::EpollServer* epoll_server_;  // Not owned.

  DISALLOW_COPY_AND_ASSIGN(QuicEpollAlarmFactory);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_EPOLL_ALARM_FACTORY_H_
