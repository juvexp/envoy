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

#ifndef GFE_QUIC_TEST_TOOLS_MOCK_RANDOM_H_
#define GFE_QUIC_TEST_TOOLS_MOCK_RANDOM_H_

#include "base/macros.h"
#include "common/quic/core/crypto/quic_random.h"

namespace gfe_quic {
namespace test {

class MockRandom : public QuicRandom {
 public:
  // Initializes base_ to 0xDEADBEEF.
  MockRandom();
  explicit MockRandom(uint32_t base);

  // QuicRandom:
  // Fills the |data| buffer with a repeating byte, initially 'r'.
  void RandBytes(void* data, size_t len) override;
  // Returns base + the current increment.
  uint64_t RandUint64() override;
  // Does nothing.
  void Reseed(const void* additional_entropy, size_t entropy_len) override;

  // ChangeValue increments |increment_|. This causes the value returned by
  // |RandUint64| and the byte that |RandBytes| fills with, to change.
  void ChangeValue();

 private:
  uint32_t base_;
  uint8_t increment_;

  DISALLOW_COPY_AND_ASSIGN(MockRandom);
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_MOCK_RANDOM_H_
