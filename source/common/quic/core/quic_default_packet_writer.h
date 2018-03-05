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

#ifndef GFE_QUIC_CORE_QUIC_DEFAULT_PACKET_WRITER_H_
#define GFE_QUIC_CORE_QUIC_DEFAULT_PACKET_WRITER_H_

#include "base/macros.h"
#include "common/quic/core/quic_packet_writer.h"
#include "common/quic/platform/api/quic_socket_address.h"

namespace gfe_quic {

struct WriteResult;

// Default packet writer which wraps QuicSocketUtils WritePacket.
class QuicDefaultPacketWriter : public QuicPacketWriter {
 public:
  explicit QuicDefaultPacketWriter(int fd);
  ~QuicDefaultPacketWriter() override;

  // QuicPacketWriter
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

  void set_fd(int fd) { fd_ = fd; }

 protected:
  void set_write_blocked(bool is_blocked);
  int fd() { return fd_; }

 private:
  int fd_;
  bool write_blocked_;

  DISALLOW_COPY_AND_ASSIGN(QuicDefaultPacketWriter);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_DEFAULT_PACKET_WRITER_H_
