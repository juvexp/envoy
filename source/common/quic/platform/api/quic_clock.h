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

#ifndef GFE_QUIC_PLATFORM_API_QUIC_CLOCK_H_
#define GFE_QUIC_PLATFORM_API_QUIC_CLOCK_H_

#include "common/quic/core/quic_time.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

// Interface for retreiving the current time.
class QUIC_EXPORT_PRIVATE QuicClock {
 public:
  QuicClock();
  virtual ~QuicClock();

  QuicClock(const QuicClock&) = delete;
  QuicClock& operator=(const QuicClock&) = delete;

  // Returns the approximate current time as a QuicTime object.
  virtual QuicTime ApproximateNow() const = 0;

  // Returns the current time as a QuicTime object.
  // Note: this use significant resources please use only if needed.
  virtual QuicTime Now() const = 0;

  // WallNow returns the current wall-time - a time that is consistent across
  // different clocks.
  virtual QuicWallTime WallNow() const = 0;

  // Converts |walltime| to a QuicTime relative to this clock's epoch.
  virtual QuicTime ConvertWallTimeToQuicTime(
      const QuicWallTime& walltime) const;

 protected:
  // Creates a new QuicTime using |time_us| as the internal value.
  QuicTime CreateTimeFromMicroseconds(uint64_t time_us) const {
    return QuicTime(time_us);
  }
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_PLATFORM_API_QUIC_CLOCK_H_
