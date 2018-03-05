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

#include "common/quic/core/quic_alarm.h"

namespace gfe_quic {

QuicAlarm::QuicAlarm(QuicArenaScopedPtr<Delegate> delegate)
    : delegate_(std::move(delegate)), deadline_(QuicTime::Zero()) {}

QuicAlarm::~QuicAlarm() {}

void QuicAlarm::Set(QuicTime new_deadline) {
  DCHECK(!IsSet());
  DCHECK(new_deadline.IsInitialized());
  deadline_ = new_deadline;
  SetImpl();
}

void QuicAlarm::Cancel() {
  if (!IsSet()) {
    // Don't try to cancel an alarm that hasn't been set.
    return;
  }
  deadline_ = QuicTime::Zero();
  CancelImpl();
}

void QuicAlarm::Update(QuicTime new_deadline, QuicTime::Delta granularity) {
  if (!new_deadline.IsInitialized()) {
    Cancel();
    return;
  }
  if (std::abs((new_deadline - deadline_).ToMicroseconds()) <
      granularity.ToMicroseconds()) {
    return;
  }
  const bool was_set = IsSet();
  deadline_ = new_deadline;
  if (was_set) {
    UpdateImpl();
  } else {
    SetImpl();
  }
}

bool QuicAlarm::IsSet() const {
  return deadline_.IsInitialized();
}

void QuicAlarm::Fire() {
  if (!IsSet()) {
    return;
  }

  deadline_ = QuicTime::Zero();
  delegate_->OnAlarm();
}

void QuicAlarm::UpdateImpl() {
  // CancelImpl and SetImpl take the new deadline by way of the deadline_
  // member, so save and restore deadline_ before canceling.
  const QuicTime new_deadline = deadline_;

  deadline_ = QuicTime::Zero();
  CancelImpl();

  deadline_ = new_deadline;
  SetImpl();
}

}  // namespace gfe_quic
