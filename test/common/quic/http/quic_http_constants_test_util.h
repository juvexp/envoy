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

#ifndef GFE_QUIC_HTTP_QUIC_HTTP_CONSTANTS_TEST_UTIL_H_
#define GFE_QUIC_HTTP_QUIC_HTTP_CONSTANTS_TEST_UTIL_H_

#include <cstdint>
#include <vector>

#include "common/quic/http/quic_http_constants.h"

namespace gfe_quic {
namespace test {

// Returns a vector of all supported frame types.
std::vector<QuicHttpFrameType> AllQuicHttpFrameTypes();

// Returns a vector of all supported frame flags for the specified
// frame type. Empty if the type is unknown.
std::vector<QuicHttpFrameFlag> AllQuicHttpFrameFlagsForFrameType(
    QuicHttpFrameType type);

// Returns a vector of all supported RST_STREAM and GOAWAY error codes.
std::vector<QuicHttpErrorCode> AllQuicHttpErrorCodes();

// Returns a vector of all supported parameters in SETTINGS frames.
std::vector<QuicHttpSettingsParameter> AllQuicHttpSettingsParameters();

// Returns a mask of flags supported for the specified frame type. Returns
// zero for unknown frame types.
uint8_t KnownFlagsMaskForFrameType(QuicHttpFrameType type);

// Returns a mask of flag bits known to be invalid for the frame type.
// For unknown frame types, the mask is zero; i.e., we don't know that any
// are invalid.
uint8_t InvalidFlagMaskForFrameType(QuicHttpFrameType type);

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_QUIC_HTTP_CONSTANTS_TEST_UTIL_H_
