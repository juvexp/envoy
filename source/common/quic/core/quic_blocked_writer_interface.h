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

// This is an interface for all objects that want to be notified that
// the underlying UDP socket is available for writing (not write blocked
// anymore).

#ifndef GFE_QUIC_CORE_QUIC_BLOCKED_WRITER_INTERFACE_H_
#define GFE_QUIC_CORE_QUIC_BLOCKED_WRITER_INTERFACE_H_

#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

class QUIC_EXPORT_PRIVATE QuicBlockedWriterInterface {
 public:
  virtual ~QuicBlockedWriterInterface() {}

  // Called by the PacketWriter when the underlying socket becomes writable
  // so that the BlockedWriter can go ahead and try writing.
  virtual void OnBlockedWriterCanWrite() = 0;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_BLOCKED_WRITER_INTERFACE_H_
