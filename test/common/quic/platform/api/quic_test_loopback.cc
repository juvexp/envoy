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

#include "test/common/quic/platform/api/quic_test_loopback.h"

namespace gfe_quic {

IpAddressFamily AddressFamilyUnderTest() {
  return AddressFamilyUnderTestImpl();
}

QuicIpAddress TestLoopback4() {
  return TestLoopback4Impl();
}

QuicIpAddress TestLoopback6() {
  return TestLoopback6Impl();
}

QuicIpAddress TestLoopback() {
  return TestLoopbackImpl();
}

QuicIpAddress TestLoopback(int index) {
  return TestLoopbackImpl(index);
}

}  // namespace gfe_quic
