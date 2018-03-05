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

#include "common/quic/core/quic_bandwidth.h"

#include <cinttypes>

#include "common/quic/platform/api/quic_str_cat.h"
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {

QuicString QuicBandwidth::ToDebugValue() const {
  if (bits_per_second_ < 80000) {
    return QuicStringPrintf("%" PRId64 " bits/s (%" PRId64 " bytes/s)",
                            bits_per_second_, bits_per_second_ / 8);
  }

  double divisor;
  char unit;
  if (bits_per_second_ < 8 * 1000 * 1000) {
    divisor = 1e3;
    unit = 'k';
  } else if (bits_per_second_ < INT64_C(8) * 1000 * 1000 * 1000) {
    divisor = 1e6;
    unit = 'M';
  } else {
    divisor = 1e9;
    unit = 'G';
  }

  double bits_per_second_with_unit = bits_per_second_ / divisor;
  double bytes_per_second_with_unit = bits_per_second_with_unit / 8;
  return QuicStringPrintf("%.2f %cbits/s (%.2f %cbytes/s)",
                          bits_per_second_with_unit, unit,
                          bytes_per_second_with_unit, unit);
}

}  // namespace gfe_quic
