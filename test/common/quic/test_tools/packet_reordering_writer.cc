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

#include "test/common/quic/test_tools/packet_reordering_writer.h"

namespace gfe_quic {
namespace test {

PacketReorderingWriter::PacketReorderingWriter() {}

PacketReorderingWriter::~PacketReorderingWriter() {}

WriteResult PacketReorderingWriter::WritePacket(
    const char* buffer,
    size_t buf_len,
    const QuicIpAddress& self_address,
    const QuicSocketAddress& peer_address,
    PerPacketOptions* options) {
  if (!delay_next_) {
    VLOG(2) << "Writing a non-delayed packet";
    WriteResult wr = QuicPacketWriterWrapper::WritePacket(
        buffer, buf_len, self_address, peer_address, options);
    --num_packets_to_wait_;
    if (num_packets_to_wait_ == 0) {
      VLOG(2) << "Writing a delayed packet";
      // It's time to write the delayed packet.
      QuicPacketWriterWrapper::WritePacket(
          delayed_data_.data(), delayed_data_.length(), delayed_self_address_,
          delayed_peer_address_, delayed_options_.get());
    }
    return wr;
  }
  // Still have packet to wait.
  DCHECK_LT(0u, num_packets_to_wait_) << "Only allow one packet to be delayed";
  delayed_data_ = string(buffer, buf_len);
  delayed_self_address_ = self_address;
  delayed_peer_address_ = peer_address;
  if (options != nullptr) {
    delayed_options_.reset(options->Clone());
  }
  delay_next_ = false;
  return WriteResult(WRITE_STATUS_OK, buf_len);
}

void PacketReorderingWriter::SetDelay(size_t num_packets_to_wait) {
  DCHECK_GT(num_packets_to_wait, 0u);
  num_packets_to_wait_ = num_packets_to_wait;
  delay_next_ = true;
}

}  // namespace test
}  // namespace gfe_quic
