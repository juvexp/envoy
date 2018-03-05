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

#ifndef GFE_QUIC_TEST_TOOLS_QUIC_SENT_PACKET_MANAGER_PEER_H_
#define GFE_QUIC_TEST_TOOLS_QUIC_SENT_PACKET_MANAGER_PEER_H_

#include "base/macros.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_sent_packet_manager.h"

namespace gfe_quic {

class SendAlgorithmInterface;

namespace test {

class QuicSentPacketManagerPeer {
 public:
  static size_t GetMaxTailLossProbes(
      QuicSentPacketManager* sent_packet_manager);

  static void SetMaxTailLossProbes(QuicSentPacketManager* sent_packet_manager,
                                   size_t max_tail_loss_probes);

  static bool GetEnableHalfRttTailLossProbe(
      QuicSentPacketManager* sent_packet_manager);

  static bool GetUseNewRto(QuicSentPacketManager* sent_packet_manager);

  static void SetPerspective(QuicSentPacketManager* sent_packet_manager,
                             Perspective perspective);

  static SendAlgorithmInterface* GetSendAlgorithm(
      const QuicSentPacketManager& sent_packet_manager);

  static void SetSendAlgorithm(QuicSentPacketManager* sent_packet_manager,
                               SendAlgorithmInterface* send_algorithm);

  static const LossDetectionInterface* GetLossAlgorithm(
      QuicSentPacketManager* sent_packet_manager);

  static void SetLossAlgorithm(QuicSentPacketManager* sent_packet_manager,
                               LossDetectionInterface* loss_detector);

  static RttStats* GetRttStats(QuicSentPacketManager* sent_packet_manager);

  static bool HasPendingPackets(
      const QuicSentPacketManager* sent_packet_manager);

  // Returns true if |packet_number| is a retransmission of a packet.
  static bool IsRetransmission(QuicSentPacketManager* sent_packet_manager,
                               QuicPacketNumber packet_number);

  static void MarkForRetransmission(QuicSentPacketManager* sent_packet_manager,
                                    QuicPacketNumber packet_number,
                                    TransmissionType transmission_type);

  static QuicTime::Delta GetRetransmissionDelay(
      const QuicSentPacketManager* sent_packet_manager);
  static QuicTime::Delta GetTailLossProbeDelay(
      const QuicSentPacketManager* sent_packet_manager);

  static bool HasUnackedCryptoPackets(
      const QuicSentPacketManager* sent_packet_manager);

  static size_t GetNumRetransmittablePackets(
      const QuicSentPacketManager* sent_packet_manager);

  static QuicByteCount GetBytesInFlight(
      const QuicSentPacketManager* sent_packet_manager);

  static void SetConsecutiveRtoCount(QuicSentPacketManager* sent_packet_manager,
                                     size_t count);

  static void SetConsecutiveTlpCount(QuicSentPacketManager* sent_packet_manager,
                                     size_t count);

  static QuicSustainedBandwidthRecorder& GetBandwidthRecorder(
      QuicSentPacketManager* sent_packet_manager);

  static void SetUsingPacing(QuicSentPacketManager* sent_packet_manager,
                             bool using_pacing);

  static bool UsingPacing(const QuicSentPacketManager* sent_packet_manager);

  static bool IsUnacked(QuicSentPacketManager* sent_packet_manager,
                        QuicPacketNumber packet_number);

  static bool HasRetransmittableFrames(
      QuicSentPacketManager* sent_packet_manager,
      QuicPacketNumber packet_number);

  static QuicUnackedPacketMap* GetUnackedPacketMap(
      QuicSentPacketManager* sent_packet_manager);

  static void DisablePacerBursts(QuicSentPacketManager* sent_packet_manager);

  static void SetNextPacedPacketTime(QuicSentPacketManager* sent_packet_manager,
                                     QuicTime time);

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicSentPacketManagerPeer);
};

}  // namespace test

}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_QUIC_SENT_PACKET_MANAGER_PEER_H_
