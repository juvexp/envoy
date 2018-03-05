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

#include "common/quic/platform/api/quic_ip_address.h"
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {

QuicIpAddress QuicIpAddress::Loopback4() {
  return QuicIpAddress(QuicIpAddressImpl::Loopback4());
}

QuicIpAddress QuicIpAddress::Loopback6() {
  return QuicIpAddress(QuicIpAddressImpl::Loopback6());
}

QuicIpAddress QuicIpAddress::Any4() {
  return QuicIpAddress(QuicIpAddressImpl::Any4());
}

QuicIpAddress QuicIpAddress::Any6() {
  return QuicIpAddress(QuicIpAddressImpl::Any6());
}

QuicIpAddress::QuicIpAddress(const QuicIpAddressImpl& impl) : impl_(impl) {}

bool operator==(QuicIpAddress lhs, QuicIpAddress rhs) {
  return lhs.impl_ == rhs.impl_;
}

bool operator!=(QuicIpAddress lhs, QuicIpAddress rhs) {
  return !(lhs == rhs);
}

bool QuicIpAddress::IsInitialized() const {
  return impl_.IsInitialized();
}

IpAddressFamily QuicIpAddress::address_family() const {
  return impl_.address_family();
}

int QuicIpAddress::AddressFamilyToInt() const {
  return impl_.AddressFamilyToInt();
}

QuicString QuicIpAddress::ToPackedString() const {
  return impl_.ToPackedString();
}

QuicString QuicIpAddress::ToString() const {
  return impl_.ToString();
}

QuicIpAddress QuicIpAddress::Normalized() const {
  return QuicIpAddress(impl_.Normalized());
}

QuicIpAddress QuicIpAddress::DualStacked() const {
  return QuicIpAddress(impl_.DualStacked());
}

bool QuicIpAddress::FromPackedString(const char* data, size_t length) {
  return impl_.FromPackedString(data, length);
}

bool QuicIpAddress::FromString(QuicString str) {
  return impl_.FromString(str);
}

bool QuicIpAddress::IsIPv4() const {
  return impl_.IsIPv4();
}

bool QuicIpAddress::IsIPv6() const {
  return impl_.IsIPv6();
}

bool QuicIpAddress::InSameSubnet(const QuicIpAddress& other,
                                 int subnet_length) {
  return impl_.InSameSubnet(other.impl(), subnet_length);
}

}  // namespace gfe_quic
