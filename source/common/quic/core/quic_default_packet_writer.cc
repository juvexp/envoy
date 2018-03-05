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

#include "common/quic/core/quic_default_packet_writer.h"

#include "common/quic/platform/impl/quic_socket_utils.h"

namespace gfe_quic {

QuicDefaultPacketWriter::QuicDefaultPacketWriter(int fd)
    : fd_(fd), write_blocked_(false) {}

QuicDefaultPacketWriter::~QuicDefaultPacketWriter() {}

WriteResult QuicDefaultPacketWriter::WritePacket(
    const char* buffer,
    size_t buf_len,
    const QuicIpAddress& self_address,
    const QuicSocketAddress& peer_address,
    PerPacketOptions* options) {
  DCHECK(!write_blocked_);
  DCHECK(nullptr == options)
      << "QuicDefaultPacketWriter does not accept any options.";
  WriteResult result = QuicSocketUtils::WritePacket(fd_, buffer, buf_len,
                                                    self_address, peer_address);
  if (result.status == WRITE_STATUS_BLOCKED) {
    write_blocked_ = true;
  }
  return result;
}

bool QuicDefaultPacketWriter::IsWriteBlockedDataBuffered() const {
  return false;
}

bool QuicDefaultPacketWriter::IsWriteBlocked() const {
  return write_blocked_;
}

void QuicDefaultPacketWriter::SetWritable() {
  write_blocked_ = false;
}

QuicByteCount QuicDefaultPacketWriter::GetMaxPacketSize(
    const QuicSocketAddress& peer_address) const {
  return kMaxPacketSize;
}

void QuicDefaultPacketWriter::set_write_blocked(bool is_blocked) {
  write_blocked_ = is_blocked;
}

}  // namespace gfe_quic
