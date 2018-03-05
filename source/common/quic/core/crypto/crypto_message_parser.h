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

#ifndef GFE_QUIC_CORE_CRYPTO_CRYPTO_MESSAGE_PARSER_H_
#define GFE_QUIC_CORE_CRYPTO_CRYPTO_MESSAGE_PARSER_H_

#include "common/quic/core/quic_error_codes.h"
#include "common/quic/core/quic_types.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {

class QUIC_EXPORT_PRIVATE CryptoMessageParser {
 public:
  virtual ~CryptoMessageParser() {}

  virtual QuicErrorCode error() const = 0;
  virtual const QuicString& error_detail() const = 0;

  // Processes input data, which must be delivered in order. Returns
  // false if there was an error, and true otherwise.
  virtual bool ProcessInput(QuicStringPiece input, Perspective perspective) = 0;

  // Returns the number of bytes of buffered input data remaining to be
  // parsed.
  virtual size_t InputBytesRemaining() const = 0;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_CRYPTO_MESSAGE_PARSER_H_
