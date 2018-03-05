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

#include "common/quic/core/host_port_pair.h"

#include "common/quic/platform/api/quic_bug_tracker.h"
#include "common/quic/platform/api/quic_str_cat.h"
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {

HostPortPair::HostPortPair() : port_(0) {}

HostPortPair::HostPortPair(const QuicString& host, uint16_t port)
    : port_(port) {
  set_host(host);
}

void HostPortPair::set_host(const QuicString& host) {
  // Check to see if the host is an IPv6 address. If so, check it doesn't have
  // brackets.
  if (host.find(':') != QuicString::npos) {
    DCHECK_NE('[', host_[0]);
  }
  host_ = host;
}

QuicString HostPortPair::ToString() const {
  return QuicStringPrintf("%s:%u", HostForURL().c_str(), port_);
}

QuicString HostPortPair::HostForURL() const {
  // TODO: Add support for |host| to have '\0'.
  if (host_.find('\0') != QuicString::npos) {
    QUIC_BUG << "Host has a null char: " << host_;
  }
  // Check to see if the host is an IPv6 address.  If so, added brackets.
  if (host_.find(':') != QuicString::npos) {
    DCHECK_NE('[', host_[0]);
    return QuicStringPrintf("[%s]", host_.c_str());
  }

  return host_;
}

}  // namespace gfe_quic
