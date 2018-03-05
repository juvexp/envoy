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

#include "common/quic/tools/quic_simple_crypto_server_stream_helper.h"

#include "test/common/quic/platform/api/quic_test.h"
#include "test/common/quic/test_tools/mock_random.h"

namespace gfe_quic {

class QuicSimpleCryptoServerStreamHelperTest : public QuicTest {};

TEST_F(QuicSimpleCryptoServerStreamHelperTest, GenerateConnectionIdForReject) {
  test::MockRandom random;
  QuicSimpleCryptoServerStreamHelper helper(&random);

  EXPECT_EQ(random.RandUint64(), helper.GenerateConnectionIdForReject(42));
}

}  // namespace gfe_quic
