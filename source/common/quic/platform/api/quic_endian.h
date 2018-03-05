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

#ifndef GFE_QUIC_PLATFORM_API_QUIC_ENDIAN_H_
#define GFE_QUIC_PLATFORM_API_QUIC_ENDIAN_H_

#include "common/quic/platform/impl/quic_endian_impl.h"

namespace gfe_quic {

enum Endianness {
  NETWORK_BYTE_ORDER,  // big endian
  HOST_BYTE_ORDER      // little endian
};

// Provide utility functions that convert from/to network order (big endian)
// to/from host order (can be either little or big endian depending on the
// platform).
class QuicEndian {
 public:
  // Convert |x| from host order (can be either little or big endian depending
  // on the platform) to network order (big endian).
  static uint16_t HostToNet16(uint16_t x) {
    return QuicEndianImpl::HostToNet16(x);
  }
  static uint32_t HostToNet32(uint32_t x) {
    return QuicEndianImpl::HostToNet32(x);
  }
  static uint64_t HostToNet64(uint64_t x) {
    return QuicEndianImpl::HostToNet64(x);
  }

  // Convert |x| from network order (big endian) to host order (can be either
  // little or big endian depending on the platform).
  static uint16_t NetToHost16(uint16_t x) {
    return QuicEndianImpl::NetToHost16(x);
  }
  static uint32_t NetToHost32(uint32_t x) {
    return QuicEndianImpl::NetToHost32(x);
  }
  static uint64_t NetToHost64(uint64_t x) {
    return QuicEndianImpl::NetToHost64(x);
  }

  // Returns true if current host order is little endian.
  static bool HostIsLittleEndian() {
    return QuicEndianImpl::HostIsLittleEndian();
  }
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_PLATFORM_API_QUIC_ENDIAN_H_
