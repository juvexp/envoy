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

#include "test/common/quic/http/tools/quic_http_stream_id_generator.h"

#include "base/logging.h"

namespace gfe_quic {
namespace test {

StreamIdGenerator::StreamIdGenerator(uint32_t first_client_id,
                                     uint32_t first_server_id)
    : next_client_id_(first_client_id), next_server_id_(first_server_id) {
  CHECK_LT(0, first_client_id);
  CHECK_LT(0, first_server_id);
  CHECK_EQ(1, first_client_id % 2) << first_client_id;
  CHECK_EQ(0, first_server_id % 2) << first_server_id;
}

StreamIdGenerator::StreamIdGenerator() : StreamIdGenerator(1, 2) {}

StreamIdGenerator::StreamIdGenerator(QuicTestRandomBase* rng)
    : StreamIdGenerator(rng->Rand16() | 1, rng->Rand16() & ~1) {}

StreamIdGenerator::~StreamIdGenerator() {}

uint32_t StreamIdGenerator::NextClientId() {
  uint32_t id = next_client_id_;
  next_client_id_ += 2;
  return id;
}

uint32_t StreamIdGenerator::NextServerId() {
  uint32_t id = next_server_id_;
  next_server_id_ += 2;
  return id;
}

}  // namespace test
}  // namespace gfe_quic
