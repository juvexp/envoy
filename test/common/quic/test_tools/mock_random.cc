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

#include "test/common/quic/test_tools/mock_random.h"

namespace gfe_quic {
namespace test {

MockRandom::MockRandom() : base_(0xDEADBEEF), increment_(0) {}

MockRandom::MockRandom(uint32_t base) : base_(base), increment_(0) {}

void MockRandom::RandBytes(void* data, size_t len) {
  memset(data, 'r' + increment_, len);
}

uint64_t MockRandom::RandUint64() {
  return base_ + increment_;
}

void MockRandom::Reseed(const void* additional_entropy, size_t entropy_len) {}

void MockRandom::ChangeValue() {
  increment_++;
}

}  // namespace test
}  // namespace gfe_quic
