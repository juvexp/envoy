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

#include "common/quic/core/quic_epoll_alarm_factory.h"

#include <memory>

#include "base/macros.h"
#include "common/quic/core/quic_arena_scoped_ptr.h"

namespace gfe_quic {
namespace {

class QuicEpollAlarm : public QuicAlarm {
 public:
  QuicEpollAlarm(gfe2::EpollServer* epoll_server,
                 QuicArenaScopedPtr<QuicAlarm::Delegate> delegate)
      : QuicAlarm(std::move(delegate)),
        epoll_server_(epoll_server),
        epoll_alarm_impl_(this) {}
  ~QuicEpollAlarm() override{};

 protected:
  void SetImpl() override {
    DCHECK(deadline().IsInitialized());
    epoll_server_->RegisterAlarm(
        (deadline() - QuicTime::Zero()).ToMicroseconds(), &epoll_alarm_impl_);
  }

  void CancelImpl() override {
    DCHECK(!deadline().IsInitialized());
    epoll_alarm_impl_.UnregisterIfRegistered();
  }

  void UpdateImpl() override {
    DCHECK(deadline().IsInitialized());
    int64_t epoll_deadline = (deadline() - QuicTime::Zero()).ToMicroseconds();
    if (epoll_alarm_impl_.registered()) {
      epoll_alarm_impl_.ReregisterAlarm(epoll_deadline);
    } else {
      epoll_server_->RegisterAlarm(epoll_deadline, &epoll_alarm_impl_);
    }
  }

 private:
  class EpollAlarmImpl : public gfe2::EpollAlarm {
   public:
    explicit EpollAlarmImpl(QuicEpollAlarm* alarm) : alarm_(alarm) {}

    // Use the same integer type as the base class.
    int64 /* allow-non-std-int */ OnAlarm() override {
      EpollAlarm::OnAlarm();
      alarm_->Fire();
      // Fire will take care of registering the alarm, if needed.
      return 0;
    }

   private:
    QuicEpollAlarm* alarm_;
  };

  gfe2::EpollServer* epoll_server_;
  EpollAlarmImpl epoll_alarm_impl_;
};

}  // namespace

QuicEpollAlarmFactory::QuicEpollAlarmFactory(gfe2::EpollServer* epoll_server)
    : epoll_server_(epoll_server) {}

QuicEpollAlarmFactory::~QuicEpollAlarmFactory() {}

QuicAlarm* QuicEpollAlarmFactory::CreateAlarm(QuicAlarm::Delegate* delegate) {
  return new QuicEpollAlarm(epoll_server_,
                            QuicArenaScopedPtr<QuicAlarm::Delegate>(delegate));
}

QuicArenaScopedPtr<QuicAlarm> QuicEpollAlarmFactory::CreateAlarm(
    QuicArenaScopedPtr<QuicAlarm::Delegate> delegate,
    QuicConnectionArena* arena) {
  if (arena != nullptr) {
    return arena->New<QuicEpollAlarm>(epoll_server_, std::move(delegate));
  }
  return QuicArenaScopedPtr<QuicAlarm>(
      new QuicEpollAlarm(epoll_server_, std::move(delegate)));
}

}  // namespace gfe_quic
