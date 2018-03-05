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

#include "test/common/quic/test_tools/bad_packet_writer.h"

namespace gfe_quic {
namespace test {

BadPacketWriter::BadPacketWriter(size_t packet_causing_write_error,
                                 int error_code)
    : packet_causing_write_error_(packet_causing_write_error),
      error_code_(error_code) {}

BadPacketWriter::~BadPacketWriter() {}

WriteResult BadPacketWriter::WritePacket(const char* buffer,
                                         size_t buf_len,
                                         const QuicIpAddress& self_address,
                                         const QuicSocketAddress& peer_address,
                                         PerPacketOptions* options) {
  if (error_code_ == 0 || packet_causing_write_error_ > 0) {
    if (packet_causing_write_error_ > 0) {
      --packet_causing_write_error_;
    }
    return QuicPacketWriterWrapper::WritePacket(buffer, buf_len, self_address,
                                                peer_address, options);
  }
  // It's time to cause write error.
  int error_code = error_code_;
  error_code_ = 0;
  return WriteResult(WRITE_STATUS_ERROR, error_code);
}

}  // namespace test
}  // namespace gfe_quic
