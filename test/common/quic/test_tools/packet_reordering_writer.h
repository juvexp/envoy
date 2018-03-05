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

#ifndef GFE_QUIC_TEST_TOOLS_PACKET_REORDERING_WRITER_H_
#define GFE_QUIC_TEST_TOOLS_PACKET_REORDERING_WRITER_H_

#include "common/quic/core/quic_packet_writer_wrapper.h"

namespace gfe_quic {

namespace test {

// This packet writer allows delaying writing the next packet after
// SetDelay(num_packets_to_wait)
// is called and buffer this packet and write it after it writes next
// |num_packets_to_wait| packets. It doesn't support delaying a packet while
// there is already a packet delayed.
class PacketReorderingWriter : public QuicPacketWriterWrapper {
 public:
  PacketReorderingWriter();

  ~PacketReorderingWriter() override;

  WriteResult WritePacket(const char* buffer,
                          size_t buf_len,
                          const QuicIpAddress& self_address,
                          const QuicSocketAddress& peer_address,
                          PerPacketOptions* options) override;

  void SetDelay(size_t num_packets_to_wait);

 private:
  bool delay_next_ = false;
  size_t num_packets_to_wait_ = 0;
  string delayed_data_;
  QuicIpAddress delayed_self_address_;
  QuicSocketAddress delayed_peer_address_;
  std::unique_ptr<PerPacketOptions> delayed_options_;
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_PACKET_REORDERING_WRITER_H_
