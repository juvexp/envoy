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

#ifndef GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_MESSAGE_GENERATOR_H_
#define GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_MESSAGE_GENERATOR_H_

// Functions to generate MessageInterface instances for use in benchmarking.

#include <stddef.h>

#include <memory>

#include "test/common/quic/http/tools/quic_http_message_interface.h"
#include "common/quic/platform/api/quic_test_random.h"

namespace gfe_quic {
namespace test {

// Generate a request message vaguely similar to a gRPC request. The body will
// have a randomly selected size in the range [body_size_lo, body_size_hi).
std::unique_ptr<RequestMessageInterface> GenerateGrpcRequest(
    size_t body_size_lo,
    size_t body_size_hi,
    QuicTestRandomBase* rng);

// Generate a response message vaguely similar to a gRPC response. The body will
// have a randomly selected size in the range [body_size_lo, body_size_hi).
std::unique_ptr<ResponseMessageInterface> GenerateGrpcResponse(
    size_t body_size_lo,
    size_t body_size_hi,
    QuicTestRandomBase* rng);

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_MESSAGE_GENERATOR_H_
