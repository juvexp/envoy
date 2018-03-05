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

#ifndef GFE_QUIC_CORE_QUIC_SOCKET_ADDRESS_CODER_H_
#define GFE_QUIC_CORE_QUIC_SOCKET_ADDRESS_CODER_H_

#include <cstdint>

#include "base/macros.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_socket_address.h"
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {

// Serializes and parses a socket address (IP address and port), to be used in
// the kCADR tag in the ServerHello handshake message and the Public Reset
// packet.
class QUIC_EXPORT_PRIVATE QuicSocketAddressCoder {
 public:
  QuicSocketAddressCoder();
  explicit QuicSocketAddressCoder(const QuicSocketAddress& address);
  ~QuicSocketAddressCoder();

  QuicString Encode() const;

  bool Decode(const char* data, size_t length);

  QuicIpAddress ip() const { return address_.host(); }

  uint16_t port() const { return address_.port(); }

 private:
  QuicSocketAddress address_;

  DISALLOW_COPY_AND_ASSIGN(QuicSocketAddressCoder);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_SOCKET_ADDRESS_CODER_H_
