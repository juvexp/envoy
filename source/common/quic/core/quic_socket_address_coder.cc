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

#include "common/quic/core/quic_socket_address_coder.h"
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {

namespace {

// For convenience, the values of these constants match the values of AF_INET
// and AF_INET6 on Linux.
const uint16_t kIPv4 = 2;
const uint16_t kIPv6 = 10;

}  // namespace

QuicSocketAddressCoder::QuicSocketAddressCoder() {}

QuicSocketAddressCoder::QuicSocketAddressCoder(const QuicSocketAddress& address)
    : address_(address) {}

QuicSocketAddressCoder::~QuicSocketAddressCoder() {}

QuicString QuicSocketAddressCoder::Encode() const {
  QuicString serialized;
  uint16_t address_family;
  switch (address_.host().address_family()) {
    case IpAddressFamily::IP_V4:
      address_family = kIPv4;
      break;
    case IpAddressFamily::IP_V6:
      address_family = kIPv6;
      break;
    default:
      return serialized;
  }
  serialized.append(reinterpret_cast<const char*>(&address_family),
                    sizeof(address_family));
  serialized.append(address_.host().ToPackedString());
  uint16_t port = address_.port();
  serialized.append(reinterpret_cast<const char*>(&port), sizeof(port));
  return serialized;
}

bool QuicSocketAddressCoder::Decode(const char* data, size_t length) {
  uint16_t address_family;
  if (length < sizeof(address_family)) {
    return false;
  }
  memcpy(&address_family, data, sizeof(address_family));
  data += sizeof(address_family);
  length -= sizeof(address_family);

  size_t ip_length;
  switch (address_family) {
    case kIPv4:
      ip_length = QuicIpAddress::kIPv4AddressSize;
      break;
    case kIPv6:
      ip_length = QuicIpAddress::kIPv6AddressSize;
      break;
    default:
      return false;
  }
  if (length < ip_length) {
    return false;
  }
  std::vector<uint8_t> ip(ip_length);
  memcpy(&ip[0], data, ip_length);
  data += ip_length;
  length -= ip_length;

  uint16_t port;
  if (length != sizeof(port)) {
    return false;
  }
  memcpy(&port, data, length);

  QuicIpAddress ip_address;
  ip_address.FromPackedString(reinterpret_cast<const char*>(&ip[0]), ip_length);
  address_ = QuicSocketAddress(ip_address, port);
  return true;
}

}  // namespace gfe_quic
