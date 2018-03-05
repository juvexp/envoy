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

#ifndef GFE_QUIC_CORE_QUIC_SERVER_ID_H_
#define GFE_QUIC_CORE_QUIC_SERVER_ID_H_

#include <cstdint>

#include "base/port.h"
#include "common/quic/core/host_port_pair.h"
#include "common/quic/core/privacy_mode.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {

// The id used to identify sessions. Includes the hostname, port, scheme and
// privacy_mode.
class QUIC_EXPORT_PRIVATE QuicServerId {
 public:
  QuicServerId();
  QuicServerId(const HostPortPair& host_port_pair, PrivacyMode privacy_mode);
  QuicServerId(const QuicString& host, uint16_t port);
  QuicServerId(const QuicString& host, uint16_t port, PrivacyMode privacy_mode);
  ~QuicServerId();

  // Needed to be an element of std::set.
  bool operator<(const QuicServerId& other) const;
  bool operator==(const QuicServerId& other) const;

  // ToString() will convert the QuicServerId to "scheme:hostname:port" or
  // "scheme:hostname:port/private". "scheme" will be "https".
  QuicString ToString() const;

  // Used in Chromium, but not internally.
  const HostPortPair& host_port_pair() const { return host_port_pair_; }

  const QuicString& host() const { return host_port_pair_.host(); }

  uint16_t port() const { return host_port_pair_.port(); }

  PrivacyMode privacy_mode() const { return privacy_mode_; }

  size_t EstimateMemoryUsage() const;

 private:
  HostPortPair host_port_pair_;
  PrivacyMode privacy_mode_;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_SERVER_ID_H_
