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

#include "base/commandlineflags.h"
#include "common/quic/core/crypto/crypto_framer.h"
#include "common/quic/core/crypto/crypto_handshake_message.h"
#include "common/quic/core/quic_framer.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/platform/api/quic_string_piece.h"
#include "test/common/quic/test_tools/quic_test_utils.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  SetQuicFlag(&FLAGS_minloglevel, 3);

  gfe_quic::QuicFramer framer(gfe_quic::AllSupportedVersions(),
                              gfe_quic::QuicTime::Zero(),
                              gfe_quic::Perspective::IS_SERVER);
  const char* const packet_bytes = reinterpret_cast<const char*>(data);

  // Test the CryptoFramer.
  gfe_quic::QuicStringPiece crypto_input(packet_bytes, size);
  std::unique_ptr<gfe_quic::CryptoHandshakeMessage> handshake_message(
      gfe_quic::CryptoFramer::ParseMessage(crypto_input, framer.perspective()));

  // Test the regular QuicFramer with the same input.
  gfe_quic::test::NoOpFramerVisitor visitor;
  framer.set_visitor(&visitor);
  gfe_quic::QuicEncryptedPacket packet(packet_bytes, size);
  framer.ProcessPacket(packet);

  return 0;
}
