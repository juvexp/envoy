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

#ifndef GFE_QUIC_CORE_QUIC_PACKET_WRITER_WRAPPER_H_
#define GFE_QUIC_CORE_QUIC_PACKET_WRITER_WRAPPER_H_

#include <memory>

#include "base/macros.h"
#include "common/quic/core/quic_packet_writer.h"

namespace gfe_quic {

// Wraps a writer object to allow dynamically extending functionality. Use
// cases: replace writer while dispatcher and connections hold on to the
// wrapper; mix in monitoring; mix in mocks in unit tests.
class QuicPacketWriterWrapper : public QuicPacketWriter {
 public:
  QuicPacketWriterWrapper();
  ~QuicPacketWriterWrapper() override;

  // Default implementation of the QuicPacketWriter interface. Passes everything
  // to |writer_|.
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

  // Takes ownership of |writer|.
  void set_writer(QuicPacketWriter* writer);

  virtual void set_peer_address(const QuicSocketAddress& peer_address) {}

  QuicPacketWriter* writer() { return writer_.get(); }

 private:
  std::unique_ptr<QuicPacketWriter> writer_;

  DISALLOW_COPY_AND_ASSIGN(QuicPacketWriterWrapper);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_PACKET_WRITER_WRAPPER_H_
