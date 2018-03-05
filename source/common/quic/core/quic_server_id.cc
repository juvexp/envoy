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

#include "common/quic/core/quic_server_id.h"

#include <tuple>

#include "common/quic/platform/api/quic_estimate_memory_usage.h"
#include "common/quic/platform/api/quic_str_cat.h"
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {

QuicServerId::QuicServerId() : privacy_mode_(PRIVACY_MODE_DISABLED) {}

QuicServerId::QuicServerId(const HostPortPair& host_port_pair,
                           PrivacyMode privacy_mode)
    : host_port_pair_(host_port_pair), privacy_mode_(privacy_mode) {}

QuicServerId::QuicServerId(const QuicString& host, uint16_t port)
    : host_port_pair_(host, port), privacy_mode_(PRIVACY_MODE_DISABLED) {}

QuicServerId::QuicServerId(const QuicString& host,
                           uint16_t port,
                           PrivacyMode privacy_mode)
    : host_port_pair_(host, port), privacy_mode_(privacy_mode) {}

QuicServerId::~QuicServerId() {}

bool QuicServerId::operator<(const QuicServerId& other) const {
  return std::tie(host_port_pair_, privacy_mode_) <
         std::tie(other.host_port_pair_, other.privacy_mode_);
}

bool QuicServerId::operator==(const QuicServerId& other) const {
  return privacy_mode_ == other.privacy_mode_ &&
         host_port_pair_.Equals(other.host_port_pair_);
}

QuicString QuicServerId::ToString() const {
  return QuicStrCat("https://", host_port_pair_.ToString(),
                    (privacy_mode_ == PRIVACY_MODE_ENABLED ? "/private" : ""));
}

size_t QuicServerId::EstimateMemoryUsage() const {
  return QuicEstimateMemoryUsage(host_port_pair_);
}

}  // namespace gfe_quic
