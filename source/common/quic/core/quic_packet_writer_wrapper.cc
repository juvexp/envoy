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

#include "common/quic/core/quic_packet_writer_wrapper.h"

#include "common/quic/core/quic_types.h"

namespace gfe_quic {

QuicPacketWriterWrapper::QuicPacketWriterWrapper() {}

QuicPacketWriterWrapper::~QuicPacketWriterWrapper() {}

WriteResult QuicPacketWriterWrapper::WritePacket(
    const char* buffer,
    size_t buf_len,
    const QuicIpAddress& self_address,
    const QuicSocketAddress& peer_address,
    PerPacketOptions* options) {
  return writer_->WritePacket(buffer, buf_len, self_address, peer_address,
                              options);
}

bool QuicPacketWriterWrapper::IsWriteBlockedDataBuffered() const {
  return writer_->IsWriteBlockedDataBuffered();
}

bool QuicPacketWriterWrapper::IsWriteBlocked() const {
  return writer_->IsWriteBlocked();
}

void QuicPacketWriterWrapper::SetWritable() {
  writer_->SetWritable();
}

QuicByteCount QuicPacketWriterWrapper::GetMaxPacketSize(
    const QuicSocketAddress& peer_address) const {
  return writer_->GetMaxPacketSize(peer_address);
}

void QuicPacketWriterWrapper::set_writer(QuicPacketWriter* writer) {
  writer_.reset(writer);
}

}  // namespace gfe_quic
