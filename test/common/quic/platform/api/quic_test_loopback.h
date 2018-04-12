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

#ifndef GFE_QUIC_PLATFORM_API_QUIC_TEST_LOOPBACK_H_
#define GFE_QUIC_PLATFORM_API_QUIC_TEST_LOOPBACK_H_

#include "test/common/quic/platform/impl/quic_test_loopback_impl.h"

namespace gfe_quic {

// Returns the address family (IPv4 or IPv6) used to run test under.
IpAddressFamily AddressFamilyUnderTest();

// Returns an IPv4 loopback address.
QuicIpAddress TestLoopback4();

// Returns the only IPv6 loopback address.
QuicIpAddress TestLoopback6();

// Returns an appropriate IPv4/Ipv6 loopback address based upon whether the
// test's environment.
QuicIpAddress TestLoopback();

// If address family under test is IPv4, returns an indexed IPv4 loopback
// address. If address family under test is IPv6, the address returned is
// platform-dependent.
QuicIpAddress TestLoopback(int index);

}  // namespace gfe_quic

#endif  // GFE_QUIC_PLATFORM_API_QUIC_TEST_LOOPBACK_H_
