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

#include "common/quic/platform/api/quic_socket_address.h"
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {

QuicSocketAddress::QuicSocketAddress(QuicIpAddress address, uint16_t port)
    : impl_(address.impl(), port) {}

QuicSocketAddress::QuicSocketAddress(const struct sockaddr_storage& saddr)
    : impl_(saddr) {}

QuicSocketAddress::QuicSocketAddress(const struct sockaddr& saddr)
    : impl_(saddr) {}

QuicSocketAddress::QuicSocketAddress(const QuicSocketAddressImpl& impl)
    : impl_(impl) {}

bool operator==(const QuicSocketAddress& lhs, const QuicSocketAddress& rhs) {
  return lhs.impl_ == rhs.impl_;
}

bool operator!=(const QuicSocketAddress& lhs, const QuicSocketAddress& rhs) {
  return lhs.impl_ != rhs.impl_;
}

bool QuicSocketAddress::IsInitialized() const {
  return impl_.IsInitialized();
}

QuicString QuicSocketAddress::ToString() const {
  return impl_.ToString();
}

int QuicSocketAddress::FromSocket(int fd) {
  return impl_.FromSocket(fd);
}

QuicSocketAddress QuicSocketAddress::Normalized() const {
  return QuicSocketAddress(impl_.Normalized());
}

QuicIpAddress QuicSocketAddress::host() const {
  return QuicIpAddress(impl_.host());
}

uint16_t QuicSocketAddress::port() const {
  return impl_.port();
}

sockaddr_storage QuicSocketAddress::generic_address() const {
  return impl_.generic_address();
}

}  // namespace gfe_quic
