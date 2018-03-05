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

#ifndef GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_STREAM_ID_GENERATOR_H_
#define GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_STREAM_ID_GENERATOR_H_

// StreamIdGenerator produces sequential HTTP/2 stream ids.

#include <cstdint>

#include "common/quic/platform/api/quic_test_random.h"

namespace gfe_quic {
namespace test {

class StreamIdGenerator {
 public:
  // Use 1 as the first client stream id, 2 as the first server stream id.
  StreamIdGenerator();

  // Use the specified values for the first ids.
  StreamIdGenerator(uint32_t first_client_id, uint32_t first_server_id);

  // Chooses random (but valid) values as the first stream id of each kind.
  explicit StreamIdGenerator(QuicTestRandomBase* rng);

  virtual ~StreamIdGenerator();

  // Returns the next client stream id.
  virtual uint32_t NextClientId();

  // Returns the next server stream id.
  virtual uint32_t NextServerId();

 private:
  uint32_t next_client_id_;
  uint32_t next_server_id_;
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_STREAM_ID_GENERATOR_H_
