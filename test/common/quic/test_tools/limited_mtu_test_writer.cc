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

#include "test/common/quic/test_tools/limited_mtu_test_writer.h"

namespace gfe_quic {
namespace test {

LimitedMtuTestWriter::LimitedMtuTestWriter(QuicByteCount mtu) : mtu_(mtu) {}

LimitedMtuTestWriter::~LimitedMtuTestWriter() {}

WriteResult LimitedMtuTestWriter::WritePacket(
    const char* buffer,
    size_t buf_len,
    const QuicIpAddress& self_address,
    const QuicSocketAddress& peer_address,
    PerPacketOptions* options) {
  if (buf_len > mtu_) {
    // Drop the packet.
    return WriteResult(WRITE_STATUS_OK, buf_len);
  }

  return QuicPacketWriterWrapper::WritePacket(buffer, buf_len, self_address,
                                              peer_address, options);
}

}  // namespace test
}  // namespace gfe_quic
