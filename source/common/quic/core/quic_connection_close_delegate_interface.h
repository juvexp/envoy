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

#ifndef GFE_QUIC_CORE_QUIC_CONNECTION_CLOSE_DELEGATE_INTERFACE_H_
#define GFE_QUIC_CORE_QUIC_CONNECTION_CLOSE_DELEGATE_INTERFACE_H_

#include "common/quic/core/quic_error_codes.h"
#include "common/quic/core/quic_types.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {

// Pure virtual class to close connection on unrecoverable errors.
class QUIC_EXPORT_PRIVATE QuicConnectionCloseDelegateInterface {
 public:
  virtual ~QuicConnectionCloseDelegateInterface() {}

  // Called when an unrecoverable error is encountered.
  virtual void OnUnrecoverableError(QuicErrorCode error,
                                    const QuicString& error_details,
                                    ConnectionCloseSource source) = 0;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_CONNECTION_CLOSE_DELEGATE_INTERFACE_H_
