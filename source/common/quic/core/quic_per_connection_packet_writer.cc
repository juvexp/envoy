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

#include "common/quic/core/quic_per_connection_packet_writer.h"

namespace gfe_quic {

QuicPerConnectionPacketWriter::QuicPerConnectionPacketWriter(
    QuicPacketWriter* shared_writer)
    : shared_writer_(shared_writer) {}

QuicPerConnectionPacketWriter::~QuicPerConnectionPacketWriter() {}

WriteResult QuicPerConnectionPacketWriter::WritePacket(
    const char* buffer,
    size_t buf_len,
    const QuicIpAddress& self_address,
    const QuicSocketAddress& peer_address,
    PerPacketOptions* options) {
  return shared_writer_->WritePacket(buffer, buf_len, self_address,
                                     peer_address, options);
}

bool QuicPerConnectionPacketWriter::IsWriteBlockedDataBuffered() const {
  return shared_writer_->IsWriteBlockedDataBuffered();
}

bool QuicPerConnectionPacketWriter::IsWriteBlocked() const {
  return shared_writer_->IsWriteBlocked();
}

void QuicPerConnectionPacketWriter::SetWritable() {
  shared_writer_->SetWritable();
}

QuicByteCount QuicPerConnectionPacketWriter::GetMaxPacketSize(
    const QuicSocketAddress& peer_address) const {
  return shared_writer_->GetMaxPacketSize(peer_address);
}

}  // namespace gfe_quic
