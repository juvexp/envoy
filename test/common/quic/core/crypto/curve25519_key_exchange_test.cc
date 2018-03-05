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

#include "common/quic/core/crypto/curve25519_key_exchange.h"

#include <memory>

#include "common/quic/core/crypto/quic_random.h"
#include "common/quic/platform/api/quic_string.h"
#include "test/common/quic/platform/api/quic_test.h"

namespace gfe_quic {
namespace test {

class Curve25519KeyExchangeTest : public QuicTest {};

// SharedKey just tests that the basic key exchange identity holds: that both
// parties end up with the same key.
TEST_F(Curve25519KeyExchangeTest, SharedKey) {
  QuicRandom* const rand = QuicRandom::GetInstance();

  for (int i = 0; i < 5; i++) {
    const QuicString alice_key(Curve25519KeyExchange::NewPrivateKey(rand));
    const QuicString bob_key(Curve25519KeyExchange::NewPrivateKey(rand));

    std::unique_ptr<Curve25519KeyExchange> alice(
        Curve25519KeyExchange::New(alice_key));
    std::unique_ptr<Curve25519KeyExchange> bob(
        Curve25519KeyExchange::New(bob_key));

    const QuicStringPiece alice_public(alice->public_value());
    const QuicStringPiece bob_public(bob->public_value());

    QuicString alice_shared, bob_shared;
    ASSERT_TRUE(alice->CalculateSharedKey(bob_public, &alice_shared));
    ASSERT_TRUE(bob->CalculateSharedKey(alice_public, &bob_shared));
    ASSERT_EQ(alice_shared, bob_shared);
  }
}

}  // namespace test
}  // namespace gfe_quic
