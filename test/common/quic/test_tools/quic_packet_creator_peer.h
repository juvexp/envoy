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

#ifndef GFE_QUIC_TEST_TOOLS_QUIC_PACKET_CREATOR_PEER_H_
#define GFE_QUIC_TEST_TOOLS_QUIC_PACKET_CREATOR_PEER_H_

#include "base/macros.h"
#include "common/quic/core/quic_packets.h"

namespace gfe_quic {
class QuicFramer;
class QuicPacketCreator;

namespace test {

class QuicPacketCreatorPeer {
 public:
  static bool SendVersionInPacket(QuicPacketCreator* creator);

  static void SetSendVersionInPacket(QuicPacketCreator* creator,
                                     bool send_version_in_packet);
  static void SetPacketNumberLength(
      QuicPacketCreator* creator,
      QuicPacketNumberLength packet_number_length);
  static QuicPacketNumberLength GetPacketNumberLength(
      QuicPacketCreator* creator);
  static void SetPacketNumber(QuicPacketCreator* creator, QuicPacketNumber s);
  static void FillPacketHeader(QuicPacketCreator* creator,
                               QuicPacketHeader* header);
  static void CreateStreamFrame(QuicPacketCreator* creator,
                                QuicStreamId id,
                                size_t write_length,
                                size_t iov_offset,
                                QuicStreamOffset offset,
                                bool fin,
                                QuicFrame* frame);
  static SerializedPacket SerializeAllFrames(QuicPacketCreator* creator,
                                             const QuicFrames& frames,
                                             char* buffer,
                                             size_t buffer_len);
  static OwningSerializedPacketPointer SerializeConnectivityProbingPacket(
      QuicPacketCreator* creator);
  static EncryptionLevel GetEncryptionLevel(QuicPacketCreator* creator);
  static QuicFramer* framer(QuicPacketCreator* creator);

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicPacketCreatorPeer);
};

}  // namespace test

}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_QUIC_PACKET_CREATOR_PEER_H_
