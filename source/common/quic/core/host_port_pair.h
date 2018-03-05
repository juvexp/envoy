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

#ifndef GFE_QUIC_CORE_HOST_PORT_PAIR_H_
#define GFE_QUIC_CORE_HOST_PORT_PAIR_H_

#include <cstdint>
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {

class HostPortPair {
 public:
  HostPortPair();
  // If |host| represents an IPv6 address, it should not bracket the address.
  // TODO: Add support for |host| to have '\0'.
  HostPortPair(const QuicString& host, uint16_t port);

  // Comparator function so this can be placed in a std::map.
  bool operator<(const HostPortPair& other) const {
    if (port_ != other.port_)
      return port_ < other.port_;
    return host_ < other.host_;
  }

  // Equality test of contents.
  bool Equals(const HostPortPair& other) const {
    return host_ == other.host_ && port_ == other.port_;
  }

  const QuicString& host() const { return host_; }

  uint16_t port() const { return port_; }

  void set_host(const QuicString& host);

  // ToString() will convert the HostPortPair to "host:port".  If |host_| is an
  // IPv6 literal, it will add brackets around |host_|.
  // This function is used in the client code only and doesn't work if |host_|
  // has '\0'.
  QuicString ToString() const;

  // Returns |host_|, adding IPv6 brackets if needed.
  // This function is used in the client code only and doesn't work if |host_|
  // has '\0'.
  QuicString HostForURL() const;

 private:
  // If |host_| represents an IPv6 address, this string will not contain
  // brackets around the address.
  QuicString host_;
  uint16_t port_;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_HOST_PORT_PAIR_H_
