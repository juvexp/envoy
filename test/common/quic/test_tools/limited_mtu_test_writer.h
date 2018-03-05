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

#ifndef GFE_QUIC_TEST_TOOLS_LIMITED_MTU_TEST_WRITER_H_
#define GFE_QUIC_TEST_TOOLS_LIMITED_MTU_TEST_WRITER_H_

#include "base/macros.h"
#include "common/quic/core/quic_packet_writer_wrapper.h"
#include "common/quic/core/quic_packets.h"

namespace gfe_quic {
namespace test {

// Simulates a connection over a link with fixed MTU.  Drops packets which
// exceed the MTU and passes the rest of them as-is.
class LimitedMtuTestWriter : public QuicPacketWriterWrapper {
 public:
  explicit LimitedMtuTestWriter(QuicByteCount mtu);
  ~LimitedMtuTestWriter() override;

  // Inherited from QuicPacketWriterWrapper.
  WriteResult WritePacket(const char* buffer,
                          size_t buf_len,
                          const QuicIpAddress& self_address,
                          const QuicSocketAddress& peer_address,
                          PerPacketOptions* options) override;

 private:
  QuicByteCount mtu_;

  DISALLOW_COPY_AND_ASSIGN(LimitedMtuTestWriter);
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_LIMITED_MTU_TEST_WRITER_H_
