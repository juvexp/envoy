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

#include "test/common/quic/test_tools/simulator/queue.h"

#include "common/quic/platform/api/quic_logging.h"
#include "test/common/quic/test_tools/simulator/simulator.h"

namespace gfe_quic {
namespace simulator {

Queue::ListenerInterface::~ListenerInterface() {}

Queue::Queue(Simulator* simulator, string name, QuicByteCount capacity)
    : Actor(simulator, name),
      capacity_(capacity),
      bytes_queued_(0),
      aggregation_threshold_(0),
      aggregation_timeout_(QuicTime::Delta::Infinite()),
      current_bundle_(0),
      current_bundle_bytes_(0),
      listener_(nullptr) {
  aggregation_timeout_alarm_.reset(simulator_->GetAlarmFactory()->CreateAlarm(
      new AggregationAlarmDelegate(this)));
}

Queue::~Queue() {}

void Queue::set_tx_port(ConstrainedPortInterface* port) {
  tx_port_ = port;
}

void Queue::AcceptPacket(std::unique_ptr<Packet> packet) {
  if (packet->size + bytes_queued_ > capacity_) {
    QUIC_DVLOG(1) << "Queue [" << name() << "] has received a packet from ["
                  << packet->source << "] to [" << packet->destination
                  << "] which is over capacity.  Dropping it.";
    QUIC_DVLOG(1) << "Queue size: " << bytes_queued_ << " out of " << capacity_
                  << ".  Packet size: " << packet->size;
    return;
  }

  bytes_queued_ += packet->size;
  queue_.emplace(std::move(packet), current_bundle_);

  if (IsAggregationEnabled()) {
    current_bundle_bytes_ += queue_.front().packet->size;
    if (!aggregation_timeout_alarm_->IsSet()) {
      aggregation_timeout_alarm_->Set(clock_->Now() + aggregation_timeout_);
    }
    if (current_bundle_bytes_ >= aggregation_threshold_) {
      NextBundle();
    }
  }

  ScheduleNextPacketDequeue();
}

void Queue::Act() {
  DCHECK(!queue_.empty());
  if (tx_port_->TimeUntilAvailable().IsZero()) {
    DCHECK(bytes_queued_ >= queue_.front().packet->size);
    bytes_queued_ -= queue_.front().packet->size;

    tx_port_->AcceptPacket(std::move(queue_.front().packet));
    queue_.pop();
    if (listener_ != nullptr) {
      listener_->OnPacketDequeued();
    }
  }

  ScheduleNextPacketDequeue();
}

void Queue::EnableAggregation(QuicByteCount aggregation_threshold,
                              QuicTime::Delta aggregation_timeout) {
  DCHECK_EQ(bytes_queued_, 0u);
  DCHECK_GT(aggregation_threshold, 0u);
  DCHECK(!aggregation_timeout.IsZero());
  DCHECK(!aggregation_timeout.IsInfinite());

  aggregation_threshold_ = aggregation_threshold;
  aggregation_timeout_ = aggregation_timeout;
}

Queue::AggregationAlarmDelegate::AggregationAlarmDelegate(Queue* queue)
    : queue_(queue) {}

void Queue::AggregationAlarmDelegate::OnAlarm() {
  queue_->NextBundle();
  queue_->ScheduleNextPacketDequeue();
}

Queue::EnqueuedPacket::EnqueuedPacket(std::unique_ptr<Packet> packet,
                                      AggregationBundleNumber bundle)
    : packet(std::move(packet)), bundle(bundle) {}

Queue::EnqueuedPacket::EnqueuedPacket(EnqueuedPacket&& other) = default;

Queue::EnqueuedPacket::~EnqueuedPacket() = default;

void Queue::NextBundle() {
  current_bundle_++;
  current_bundle_bytes_ = 0;
  aggregation_timeout_alarm_->Cancel();
}

void Queue::ScheduleNextPacketDequeue() {
  if (queue_.empty()) {
    DCHECK_EQ(bytes_queued_, 0u);
    return;
  }

  if (IsAggregationEnabled() && queue_.front().bundle == current_bundle_) {
    return;
  }

  Schedule(clock_->Now() + tx_port_->TimeUntilAvailable());
}

}  // namespace simulator
}  // namespace gfe_quic
