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

#include "test/common/quic/http/quic_http_constants_test_util.h"

namespace gfe_quic {
namespace test {

std::vector<QuicHttpFrameType> AllQuicHttpFrameTypes() {
  // clang-format off
  return {
      QuicHttpFrameType::DATA,
      QuicHttpFrameType::HEADERS,
      QuicHttpFrameType::QUIC_HTTP_PRIORITY,
      QuicHttpFrameType::RST_STREAM,
      QuicHttpFrameType::SETTINGS,
      QuicHttpFrameType::PUSH_PROMISE,
      QuicHttpFrameType::PING,
      QuicHttpFrameType::GOAWAY,
      QuicHttpFrameType::WINDOW_UPDATE,
      QuicHttpFrameType::CONTINUATION,
      QuicHttpFrameType::ALTSVC,
  };
  // clang-format on
}

std::vector<QuicHttpFrameFlag> AllQuicHttpFrameFlagsForFrameType(
    QuicHttpFrameType type) {
  // clang-format off
  switch (type) {
    case QuicHttpFrameType::DATA:
      return {
          QuicHttpFrameFlag::QUIC_HTTP_END_STREAM,
          QuicHttpFrameFlag::QUIC_HTTP_PADDED,
      };
    case QuicHttpFrameType::HEADERS:
      return {
          QuicHttpFrameFlag::QUIC_HTTP_END_STREAM,
          QuicHttpFrameFlag::QUIC_HTTP_END_HEADERS,
          QuicHttpFrameFlag::QUIC_HTTP_PADDED,
          QuicHttpFrameFlag::QUIC_HTTP_PRIORITY,
      };
    case QuicHttpFrameType::SETTINGS:
      return {
          QuicHttpFrameFlag::QUIC_HTTP_ACK,
      };
    case QuicHttpFrameType::PUSH_PROMISE:
      return {
          QuicHttpFrameFlag::QUIC_HTTP_END_HEADERS,
          QuicHttpFrameFlag::QUIC_HTTP_PADDED,
      };
    case QuicHttpFrameType::PING:
      return {
          QuicHttpFrameFlag::QUIC_HTTP_ACK,
      };
    case QuicHttpFrameType::CONTINUATION:
      return {
          QuicHttpFrameFlag::QUIC_HTTP_END_HEADERS,
      };
    default:
      return {};
  }
  // clang-format on
}

std::vector<QuicHttpErrorCode> AllQuicHttpErrorCodes() {
  // clang-format off
  return {
      QuicHttpErrorCode::HTTP2_NO_ERROR,
      QuicHttpErrorCode::PROTOCOL_ERROR,
      QuicHttpErrorCode::INTERNAL_ERROR,
      QuicHttpErrorCode::FLOW_CONTROL_ERROR,
      QuicHttpErrorCode::SETTINGS_TIMEOUT,
      QuicHttpErrorCode::STREAM_CLOSED,
      QuicHttpErrorCode::FRAME_SIZE_ERROR,
      QuicHttpErrorCode::REFUSED_STREAM,
      QuicHttpErrorCode::CANCEL,
      QuicHttpErrorCode::COMPRESSION_ERROR,
      QuicHttpErrorCode::CONNECT_ERROR,
      QuicHttpErrorCode::ENHANCE_YOUR_CALM,
      QuicHttpErrorCode::INADEQUATE_SECURITY,
      QuicHttpErrorCode::HTTP_1_1_REQUIRED,
  };
  // clang-format on
}

std::vector<QuicHttpSettingsParameter> AllQuicHttpSettingsParameters() {
  // clang-format off
  return {
      QuicHttpSettingsParameter::HEADER_TABLE_SIZE,
      QuicHttpSettingsParameter::ENABLE_PUSH,
      QuicHttpSettingsParameter::MAX_CONCURRENT_STREAMS,
      QuicHttpSettingsParameter::INITIAL_WINDOW_SIZE,
      QuicHttpSettingsParameter::MAX_FRAME_SIZE,
      QuicHttpSettingsParameter::MAX_HEADER_LIST_SIZE,
  };
  // clang-format on
}

// Returns a mask of flags supported for the specified frame type. Returns
// zero for unknown frame types.
uint8_t KnownFlagsMaskForFrameType(QuicHttpFrameType type) {
  switch (type) {
    case QuicHttpFrameType::DATA:
      return QuicHttpFrameFlag::QUIC_HTTP_END_STREAM |
             QuicHttpFrameFlag::QUIC_HTTP_PADDED;
    case QuicHttpFrameType::HEADERS:
      return QuicHttpFrameFlag::QUIC_HTTP_END_STREAM |
             QuicHttpFrameFlag::QUIC_HTTP_END_HEADERS |
             QuicHttpFrameFlag::QUIC_HTTP_PADDED |
             QuicHttpFrameFlag::QUIC_HTTP_PRIORITY;
    case QuicHttpFrameType::QUIC_HTTP_PRIORITY:
      return 0x00;
    case QuicHttpFrameType::RST_STREAM:
      return 0x00;
    case QuicHttpFrameType::SETTINGS:
      return QuicHttpFrameFlag::QUIC_HTTP_ACK;
    case QuicHttpFrameType::PUSH_PROMISE:
      return QuicHttpFrameFlag::QUIC_HTTP_END_HEADERS |
             QuicHttpFrameFlag::QUIC_HTTP_PADDED;
    case QuicHttpFrameType::PING:
      return QuicHttpFrameFlag::QUIC_HTTP_ACK;
    case QuicHttpFrameType::GOAWAY:
      return 0x00;
    case QuicHttpFrameType::WINDOW_UPDATE:
      return 0x00;
    case QuicHttpFrameType::CONTINUATION:
      return QuicHttpFrameFlag::QUIC_HTTP_END_HEADERS;
    case QuicHttpFrameType::ALTSVC:
      return 0x00;
    default:
      return 0x00;
  }
}

uint8_t InvalidFlagMaskForFrameType(QuicHttpFrameType type) {
  if (IsSupportedQuicHttpFrameType(type)) {
    return ~KnownFlagsMaskForFrameType(type);
  }
  return 0x00;
}

}  // namespace test
}  // namespace gfe_quic
