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

#ifndef GFE_QUIC_TEST_TOOLS_SIMULATOR_QUEUE_H_
#define GFE_QUIC_TEST_TOOLS_SIMULATOR_QUEUE_H_

#include "common/quic/core/quic_alarm.h"
#include "test/common/quic/test_tools/simulator/link.h"

namespace gfe_quic {
namespace simulator {

// A finitely sized queue which egresses packets onto a constrained link.  The
// capacity of the queue is measured in bytes as opposed to packets.
class Queue : public Actor, public UnconstrainedPortInterface {
 public:
  class ListenerInterface {
   public:
    virtual ~ListenerInterface();

    // Called whenever a packet is removed from the queue.
    virtual void OnPacketDequeued() = 0;
  };

  Queue(Simulator* simulator, string name, QuicByteCount capacity);
  ~Queue() override;

  void set_tx_port(ConstrainedPortInterface* port);

  void AcceptPacket(std::unique_ptr<Packet> packet) override;

  void Act() override;

  inline QuicByteCount capacity() const { return capacity_; }
  inline QuicByteCount bytes_queued() const { return bytes_queued_; }
  inline QuicPacketCount packets_queued() const { return queue_.size(); }

  inline void set_listener_interface(ListenerInterface* listener) {
    listener_ = listener;
  }

  // Enables packet aggregation on the queue.  Packet aggregation makes the
  // queue bundle packets up until they reach certain size.  When the
  // aggregation is enabled, the packets are not dequeued until the total size
  // of packets in the queue reaches |aggregation_threshold|.  The packets are
  // automatically flushed from the queue if the oldest packet has been in it
  // for |aggregation_timeout|.
  //
  // This method may only be called when the queue is empty.  Once enabled,
  // aggregation cannot be disabled.
  void EnableAggregation(QuicByteCount aggregation_threshold,
                         QuicTime::Delta aggregation_timeout);

 private:
  typedef uint64_t AggregationBundleNumber;

  // In order to implement packet aggregation, each packet is tagged with a
  // bundle number.  The queue keeps a bundle counter, and whenever a bundle is
  // ready, it increments the number of the current bundle.  Only the packets
  // outside of the current bundle are allowed to leave the queue.
  struct EnqueuedPacket {
    EnqueuedPacket(std::unique_ptr<Packet> packet,
                   AggregationBundleNumber bundle);
    EnqueuedPacket(EnqueuedPacket&& other);
    ~EnqueuedPacket();

    std::unique_ptr<Packet> packet;
    AggregationBundleNumber bundle;
  };

  // Alarm handler for aggregation timeout.
  class AggregationAlarmDelegate : public QuicAlarm::Delegate {
   public:
    explicit AggregationAlarmDelegate(Queue* queue);

    void OnAlarm() override;

   private:
    Queue* queue_;
  };

  inline bool IsAggregationEnabled() const {
    return aggregation_threshold_ > 0;
  }

  // Increment the bundle counter and reset the bundle state.  This causes all
  // packets currently in the bundle to be flushed onto the link.
  void NextBundle();

  void ScheduleNextPacketDequeue();

  const QuicByteCount capacity_;
  QuicByteCount bytes_queued_;

  QuicByteCount aggregation_threshold_;
  QuicTime::Delta aggregation_timeout_;
  // The number of the current aggregation bundle.  Monotonically increasing.
  // All packets in the previous bundles are allowed to leave the queue, and
  // none of the packets in the current one are.
  AggregationBundleNumber current_bundle_;
  // Size of the current bundle.  Whenever it exceeds |aggregation_threshold_|,
  // the next bundle is created.
  QuicByteCount current_bundle_bytes_;
  // Alarm responsible for flushing the current bundle upon timeout.  Set when
  // the first packet in the bundle is enqueued.
  std::unique_ptr<QuicAlarm> aggregation_timeout_alarm_;

  ConstrainedPortInterface* tx_port_;
  QuicQueue<EnqueuedPacket> queue_;

  ListenerInterface* listener_;

  DISALLOW_COPY_AND_ASSIGN(Queue);
};

}  // namespace simulator
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_SIMULATOR_QUEUE_H_
