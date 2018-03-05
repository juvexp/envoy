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

#ifndef GFE_QUIC_CORE_FRAMES_QUIC_ACK_FRAME_H_
#define GFE_QUIC_CORE_FRAMES_QUIC_ACK_FRAME_H_

#include <ostream>

#include "common/quic/core/quic_types.h"
#include "common/quic/platform/api/quic_containers.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_flags.h"

namespace gfe_quic {

// A sequence of packet numbers where each number is unique. Intended to be used
// in a sliding window fashion, where smaller old packet numbers are removed and
// larger new packet numbers are added, with the occasional random access.
class QUIC_EXPORT_PRIVATE PacketNumberQueue {
 public:
  PacketNumberQueue();
  PacketNumberQueue(const PacketNumberQueue& other);
  PacketNumberQueue(PacketNumberQueue&& other);
  ~PacketNumberQueue();

  PacketNumberQueue& operator=(const PacketNumberQueue& other);
  PacketNumberQueue& operator=(PacketNumberQueue&& other);

  typedef QuicDeque<Interval<QuicPacketNumber>>::const_iterator const_iterator;
  typedef QuicDeque<Interval<QuicPacketNumber>>::const_reverse_iterator
      const_reverse_iterator;

  // Adds |packet_number| to the set of packets in the queue.
  void Add(QuicPacketNumber packet_number);

  // Adds packets between [lower, higher) to the set of packets in the queue. It
  // is undefined behavior to call this with |higher| < |lower|.
  void AddRange(QuicPacketNumber lower, QuicPacketNumber higher);

  // Removes packets with values less than |higher| from the set of packets in
  // the queue. Returns true if packets were removed.
  bool RemoveUpTo(QuicPacketNumber higher);

  // Removes the smallest interval in the queue.
  void RemoveSmallestInterval();

  // Clear this packet number queue.
  void Clear();

  // Returns true if the queue contains |packet_number|.
  bool Contains(QuicPacketNumber packet_number) const;

  // Returns true if the queue is empty.
  bool Empty() const;

  // Returns the minimum packet number stored in the queue. It is undefined
  // behavior to call this if the queue is empty.
  QuicPacketNumber Min() const;

  // Returns the maximum packet number stored in the queue. It is undefined
  // behavior to call this if the queue is empty.
  QuicPacketNumber Max() const;

  // Returns the number of unique packets stored in the queue. Inefficient; only
  // exposed for testing.
  size_t NumPacketsSlow() const;

  // Returns the number of disjoint packet number intervals contained in the
  // queue.
  size_t NumIntervals() const;

  // Returns the length of last interval.
  QuicPacketNumber LastIntervalLength() const;

  // Returns iterators over the packet number intervals.
  const_iterator begin() const;
  const_iterator end() const;
  const_reverse_iterator rbegin() const;
  const_reverse_iterator rend() const;

  friend QUIC_EXPORT_PRIVATE std::ostream& operator<<(
      std::ostream& os,
      const PacketNumberQueue& q);

 private:
  QuicDeque<Interval<QuicPacketNumber>> packet_number_deque_;
};

struct QUIC_EXPORT_PRIVATE QuicAckFrame {
  QuicAckFrame();
  QuicAckFrame(const QuicAckFrame& other);
  ~QuicAckFrame();

  void Clear();

  friend QUIC_EXPORT_PRIVATE std::ostream& operator<<(
      std::ostream& os,
      const QuicAckFrame& ack_frame);

  // The highest packet number we've observed from the peer. When |packets| is
  // not empty, it should always be equal to packets.Max(). The |LargestAcked|
  // function ensures this invariant in debug mode.
  QuicPacketNumber largest_acked;

  // Time elapsed since largest_observed() was received until this Ack frame was
  // sent.
  QuicTime::Delta ack_delay_time;

  // Vector of <packet_number, time> for when packets arrived.
  PacketTimeVector received_packet_times;

  // Set of packets.
  PacketNumberQueue packets;
};

// The highest acked packet number we've observed from the peer. If no packets
// have been observed, return 0.
inline QUIC_EXPORT_PRIVATE QuicPacketNumber
LargestAcked(const QuicAckFrame& frame) {
  DCHECK(frame.packets.Empty() || frame.packets.Max() == frame.largest_acked);
  return frame.largest_acked;
}

// True if the packet number is greater than largest_observed or is listed
// as missing.
// Always returns false for packet numbers less than least_unacked.
QUIC_EXPORT_PRIVATE bool IsAwaitingPacket(
    const QuicAckFrame& ack_frame,
    QuicPacketNumber packet_number,
    QuicPacketNumber peer_least_packet_awaiting_ack);

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_FRAMES_QUIC_ACK_FRAME_H_
