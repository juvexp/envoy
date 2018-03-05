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

#ifndef GFE_QUIC_CORE_QUIC_ALARM_H_
#define GFE_QUIC_CORE_QUIC_ALARM_H_

#include "base/macros.h"
#include "common/quic/core/quic_arena_scoped_ptr.h"
#include "common/quic/core/quic_time.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

// Abstract class which represents an alarm which will go off at a
// scheduled time, and execute the |OnAlarm| method of the delegate.
// An alarm may be cancelled, in which case it may or may not be
// removed from the underlying scheduling system, but in either case
// the task will not be executed.
class QUIC_EXPORT_PRIVATE QuicAlarm {
 public:
  class QUIC_EXPORT_PRIVATE Delegate {
   public:
    virtual ~Delegate() {}

    // Invoked when the alarm fires.
    virtual void OnAlarm() = 0;
  };

  explicit QuicAlarm(QuicArenaScopedPtr<Delegate> delegate);
  virtual ~QuicAlarm();

  // Sets the alarm to fire at |deadline|.  Must not be called while
  // the alarm is set.  To reschedule an alarm, call Cancel() first,
  // then Set().
  void Set(QuicTime new_deadline);

  // Cancels the alarm.  May be called repeatedly.  Does not
  // guarantee that the underlying scheduling system will remove
  // the alarm's associated task, but guarantees that the
  // delegates OnAlarm method will not be called.
  void Cancel();

  // Cancels and sets the alarm if the |deadline| is farther from the current
  // deadline than |granularity|, and otherwise does nothing.  If |deadline| is
  // not initialized, the alarm is cancelled.
  void Update(QuicTime new_deadline, QuicTime::Delta granularity);

  // Returns true if |deadline_| has been set to a non-zero time.
  bool IsSet() const;

  QuicTime deadline() const { return deadline_; }

 protected:
  // Subclasses implement this method to perform the platform-specific
  // scheduling of the alarm.  Is called from Set() or Fire(), after the
  // deadline has been updated.
  virtual void SetImpl() = 0;

  // Subclasses implement this method to perform the platform-specific
  // cancelation of the alarm.
  virtual void CancelImpl() = 0;

  // Subclasses implement this method to perform the platform-specific update of
  // the alarm if there exists a more optimal implementation than calling
  // CancelImpl() and SetImpl().
  virtual void UpdateImpl();

  // Called by subclasses when the alarm fires.  Invokes the
  // delegates |OnAlarm| if a delegate is set, and if the deadline
  // has been exceeded.  Implementations which do not remove the
  // alarm from the underlying scheduler on Cancel() may need to handle
  // the situation where the task executes before the deadline has been
  // reached, in which case they need to reschedule the task and must not
  // call invoke this method.
  void Fire();

 private:
  QuicArenaScopedPtr<Delegate> delegate_;
  QuicTime deadline_;

  DISALLOW_COPY_AND_ASSIGN(QuicAlarm);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_ALARM_H_
