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

#ifndef GFE_QUIC_TEST_TOOLS_QUIC_FRAMER_PEER_H_
#define GFE_QUIC_TEST_TOOLS_QUIC_FRAMER_PEER_H_

#include "base/macros.h"
#include "common/quic/core/crypto/quic_encrypter.h"
#include "common/quic/core/quic_framer.h"
#include "common/quic/core/quic_packets.h"

namespace gfe_quic {

namespace test {

class QuicFramerPeer {
 public:
  static QuicPacketNumber CalculatePacketNumberFromWire(
      QuicFramer* framer,
      QuicPacketNumberLength packet_number_length,
      QuicPacketNumber last_packet_number,
      QuicPacketNumber packet_number);
  static void SetLastSerializedConnectionId(QuicFramer* framer,
                                            QuicConnectionId connection_id);
  static void SetLargestPacketNumber(QuicFramer* framer,
                                     QuicPacketNumber packet_number);
  static void SetPerspective(QuicFramer* framer, Perspective perspective);

  // SwapCrypters exchanges the state of the crypters of |framer1| with
  // |framer2|.
  static void SwapCrypters(QuicFramer* framer1, QuicFramer* framer2);

  static QuicEncrypter* GetEncrypter(QuicFramer* framer, EncryptionLevel level);

  // IETF defined frame append/process methods.
  static bool ProcessIetfStreamFrame(QuicFramer* framer,
                                     QuicDataReader* reader,
                                     uint8_t frame_type,
                                     QuicStreamFrame* frame);
  static bool AppendIetfStreamFrame(QuicFramer* framer,
                                    const QuicStreamFrame& frame,
                                    bool last_frame_in_packet,
                                    QuicDataWriter* writer);

  static bool AppendIetfConnectionCloseFrame(
      QuicFramer* framer,
      const QuicConnectionCloseFrame& frame,
      QuicDataWriter* writer);
  static bool AppendIetfConnectionCloseFrame(
      QuicFramer* framer,
      const QuicIetfTransportErrorCodes code,
      const string& phrase,
      QuicDataWriter* writer);
  static bool AppendIetfApplicationCloseFrame(
      QuicFramer* framer,
      const QuicConnectionCloseFrame& frame,
      QuicDataWriter* writer);
  static bool AppendIetfApplicationCloseFrame(QuicFramer* framer,
                                              const uint16_t code,
                                              const string& phrase,
                                              QuicDataWriter* writer);
  static bool ProcessIetfConnectionCloseFrame(QuicFramer* framer,
                                              QuicDataReader* reader,
                                              const uint8_t frame_type,
                                              QuicConnectionCloseFrame* frame);
  static bool ProcessIetfApplicationCloseFrame(QuicFramer* framer,
                                               QuicDataReader* reader,
                                               const uint8_t frame_type,
                                               QuicConnectionCloseFrame* frame);
  static bool ProcessIetfAckFrame(QuicFramer* framer,
                                  QuicDataReader* reader,
                                  uint8_t frame_type,
                                  QuicAckFrame* ack_frame);
  static bool AppendIetfAckFrameAndTypeByte(QuicFramer* framer,
                                            const QuicAckFrame& frame,
                                            QuicDataWriter* writer);
  static bool AppendIetfResetStreamFrame(QuicFramer* framer,
                                         const QuicRstStreamFrame& frame,
                                         QuicDataWriter* writer);
  static bool ProcessIetfResetStreamFrame(QuicFramer* framer,
                                          QuicDataReader* reader,
                                          QuicRstStreamFrame* frame);

  // Add/remove IETF-Format padding.
  static bool AppendIetfPaddingFrame(QuicFramer* framer,
                                     const QuicPaddingFrame& frame,
                                     QuicDataWriter* writer);
  static void ProcessIetfPaddingFrame(QuicFramer* framer,
                                      QuicDataReader* reader,
                                      QuicPaddingFrame* frame);

  static bool ProcessIetfPathChallengeFrame(QuicFramer* framer,
                                            QuicDataReader* reader,
                                            QuicPathChallengeFrame* frame);
  static bool ProcessIetfPathResponseFrame(QuicFramer* framer,
                                           QuicDataReader* reader,
                                           QuicPathResponseFrame* frame);

  static bool AppendIetfPathChallengeFrameAndTypeByte(
      QuicFramer* framer,
      const QuicPathChallengeFrame& frame,
      QuicDataWriter* writer);
  static bool AppendIetfPathResponseFrameAndTypeByte(
      QuicFramer* framer,
      const QuicPathResponseFrame& frame,
      QuicDataWriter* writer);

  static bool ProcessIetfStopSendingFrame(
      QuicFramer* framer,
      QuicDataReader* reader,
      QuicStopSendingFrame* stop_sending_frame);
  static bool AppendIetfStopSendingFrameAndTypeByte(
      QuicFramer* framer,
      const QuicStopSendingFrame& stop_sending_frame,
      QuicDataWriter* writer);

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicFramerPeer);
};

}  // namespace test

}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_QUIC_FRAMER_PEER_H_
