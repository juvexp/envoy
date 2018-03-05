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

#ifndef GFE_QUIC_PLATFORM_API_QUIC_IP_ADDRESS_H_
#define GFE_QUIC_PLATFORM_API_QUIC_IP_ADDRESS_H_

#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/impl/quic_ip_address_impl.h"

namespace gfe_quic {

class QUIC_EXPORT_PRIVATE QuicIpAddress {
  // A class representing an IPv4 or IPv6 address in QUIC. The actual
  // implementation (platform dependent) of an IP address is in
  // QuicIpAddressImpl.
 public:
  enum : size_t {
    kIPv4AddressSize = QuicIpAddressImpl::kIPv4AddressSize,
    kIPv6AddressSize = QuicIpAddressImpl::kIPv6AddressSize
  };

  // TODO: Remove Loopback*() and use TestLoopback*() in tests.
  static QuicIpAddress Loopback4();
  static QuicIpAddress Loopback6();
  static QuicIpAddress Any4();
  static QuicIpAddress Any6();

  QuicIpAddress() = default;
  QuicIpAddress(const QuicIpAddress& other) = default;
  explicit QuicIpAddress(const QuicIpAddressImpl& impl);
  QuicIpAddress& operator=(const QuicIpAddress& other) = default;
  QuicIpAddress& operator=(QuicIpAddress&& other) = default;
  QUIC_EXPORT_PRIVATE friend bool operator==(QuicIpAddress lhs,
                                             QuicIpAddress rhs);
  QUIC_EXPORT_PRIVATE friend bool operator!=(QuicIpAddress lhs,
                                             QuicIpAddress rhs);

  bool IsInitialized() const;
  IpAddressFamily address_family() const;
  int AddressFamilyToInt() const;
  // Returns the address as a sequence of bytes in network-byte-order. IPv4 will
  // be 6 bytes. IPv6 will be 18 bytes.
  QuicString ToPackedString() const;
  // Returns string representation of the address.
  QuicString ToString() const;
  // Normalizes the address representation with respect to IPv4 addresses, i.e,
  // mapped IPv4 addresses ("::ffff:X.Y.Z.Q") are converted to pure IPv4
  // addresses.  All other IPv4, IPv6, and empty values are left unchanged.
  QuicIpAddress Normalized() const;
  // Returns an address suitable for use in IPv6-aware contexts.  This is the
  // opposite of NormalizeIPAddress() above.  IPv4 addresses are converted into
  // their IPv4-mapped address equivalents (e.g. 192.0.2.1 becomes
  // ::ffff:192.0.2.1).  IPv6 addresses are a noop (they are returned
  // unchanged).
  QuicIpAddress DualStacked() const;
  bool FromPackedString(const char* data, size_t length);
  bool FromString(QuicString str);
  bool IsIPv4() const;
  bool IsIPv6() const;
  bool InSameSubnet(const QuicIpAddress& other, int subnet_length);

  const QuicIpAddressImpl& impl() const { return impl_; }

 private:
  QuicIpAddressImpl impl_;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_PLATFORM_API_QUIC_IP_ADDRESS_H_
