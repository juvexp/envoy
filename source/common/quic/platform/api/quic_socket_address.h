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

#ifndef GFE_QUIC_PLATFORM_API_QUIC_SOCKET_ADDRESS_H_
#define GFE_QUIC_PLATFORM_API_QUIC_SOCKET_ADDRESS_H_

#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_ip_address.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/impl/quic_socket_address_impl.h"

namespace gfe_quic {

class QUIC_EXPORT_PRIVATE QuicSocketAddress {
  // A class representing a socket endpoint address (i.e., IP address plus a
  // port) in QUIC. The actual implementation (platform dependent) of a socket
  // address is in QuicSocketAddressImpl.
 public:
  QuicSocketAddress() = default;
  QuicSocketAddress(QuicIpAddress address, uint16_t port);
  explicit QuicSocketAddress(const struct sockaddr_storage& saddr);
  explicit QuicSocketAddress(const struct sockaddr& saddr);
  explicit QuicSocketAddress(const QuicSocketAddressImpl& impl);
  QuicSocketAddress(const QuicSocketAddress& other) = default;
  QuicSocketAddress& operator=(const QuicSocketAddress& other) = default;
  QuicSocketAddress& operator=(QuicSocketAddress&& other) = default;
  QUIC_EXPORT_PRIVATE friend bool operator==(const QuicSocketAddress& lhs,
                                             const QuicSocketAddress& rhs);
  QUIC_EXPORT_PRIVATE friend bool operator!=(const QuicSocketAddress& lhs,
                                             const QuicSocketAddress& rhs);

  bool IsInitialized() const;
  QuicString ToString() const;
  int FromSocket(int fd);
  QuicSocketAddress Normalized() const;

  QuicIpAddress host() const;
  uint16_t port() const;
  sockaddr_storage generic_address() const;
  const QuicSocketAddressImpl& impl() const { return impl_; }

 private:
  QuicSocketAddressImpl impl_;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_PLATFORM_API_QUIC_SOCKET_ADDRESS_H_
