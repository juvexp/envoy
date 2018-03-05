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

#ifndef GFE_QUIC_CORE_QUIC_PER_CONNECTION_PACKET_WRITER_H_
#define GFE_QUIC_CORE_QUIC_PER_CONNECTION_PACKET_WRITER_H_

#include "base/macros.h"
#include "common/quic/core/quic_connection.h"
#include "common/quic/core/quic_packet_writer.h"

namespace gfe_quic {

// A connection-specific packet writer that wraps a shared writer.
class QuicPerConnectionPacketWriter : public QuicPacketWriter {
 public:
  // Does not take ownership of |shared_writer|.
  explicit QuicPerConnectionPacketWriter(QuicPacketWriter* shared_writer);
  ~QuicPerConnectionPacketWriter() override;

  // Default implementation of the QuicPacketWriter interface: Passes everything
  // to |shared_writer_|.
  WriteResult WritePacket(const char* buffer,
                          size_t buf_len,
                          const QuicIpAddress& self_address,
                          const QuicSocketAddress& peer_address,
                          PerPacketOptions* options) override;
  bool IsWriteBlockedDataBuffered() const override;
  bool IsWriteBlocked() const override;
  void SetWritable() override;
  QuicByteCount GetMaxPacketSize(
      const QuicSocketAddress& peer_address) const override;

 private:
  QuicPacketWriter* shared_writer_;  // Not owned.

  DISALLOW_COPY_AND_ASSIGN(QuicPerConnectionPacketWriter);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_PER_CONNECTION_PACKET_WRITER_H_
