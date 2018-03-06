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

#ifndef GFE_QUIC_CORE_QUIC_WRITE_BLOCKED_LIST_H_
#define GFE_QUIC_CORE_QUIC_WRITE_BLOCKED_LIST_H_

#include <cstddef>
#include <cstdint>

#include "base/macros.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/spdy_utils/core/priority_write_scheduler.h"

namespace gfe_quic {

// Keeps tracks of the QUIC streams that have data to write, sorted by
// priority.  QUIC stream priority order is:
// Crypto stream > Headers stream > Data streams by requested priority.
class QUIC_EXPORT_PRIVATE QuicWriteBlockedList {
 private:
  typedef gfe_spdy::PriorityWriteScheduler<QuicStreamId>
      QuicPriorityWriteScheduler;

 public:
  QuicWriteBlockedList();
  ~QuicWriteBlockedList();

  bool HasWriteBlockedDataStreams() const {
    return priority_write_scheduler_.HasReadyStreams();
  }

  bool HasWriteBlockedSpecialStream() const {
    return crypto_stream_blocked_ || headers_stream_blocked_;
  }

  size_t NumBlockedSpecialStreams() const {
    size_t num_blocked = 0;
    if (crypto_stream_blocked_) {
      ++num_blocked;
    }
    if (headers_stream_blocked_) {
      ++num_blocked;
    }
    return num_blocked;
  }

  size_t NumBlockedStreams() const {
    return NumBlockedSpecialStreams() +
           priority_write_scheduler_.NumReadyStreams();
  }

  bool ShouldYield(QuicStreamId id) const {
    if (id == kCryptoStreamId) {
      return false;  // The crypto stream yields to none.
    }
    if (crypto_stream_blocked_) {
      return true;  // If the crypto stream is blocked, all other streams yield.
    }
    if (id == kHeadersStreamId) {
      return false;  // The crypto stream isn't blocked so headers won't yield.
    }
    if (headers_stream_blocked_) {
      return true;  // All data streams yield to the headers stream.
    }

    return priority_write_scheduler_.ShouldYield(id);
  }

  // Pops the highest priorty stream, special casing crypto and headers streams.
  // Latches the most recently popped data stream for batch writing purposes.
  QuicStreamId PopFront() {
    if (crypto_stream_blocked_) {
      crypto_stream_blocked_ = false;
      return kCryptoStreamId;
    }

    if (headers_stream_blocked_) {
      headers_stream_blocked_ = false;
      return kHeadersStreamId;
    }

    const auto id_and_precedence =
        priority_write_scheduler_.PopNextReadyStreamAndPrecedence();
    const QuicStreamId id = std::get<0>(id_and_precedence);
    const gfe_spdy::SpdyPriority priority =
        std::get<1>(id_and_precedence).spdy3_priority();

    if (!priority_write_scheduler_.HasReadyStreams()) {
      // If no streams are blocked, don't bother latching.  This stream will be
      // the first popped for its priority anyway.
      batch_write_stream_id_[priority] = 0;
      last_priority_popped_ = priority;
    } else if (batch_write_stream_id_[priority] != id) {
      // If newly latching this batch write stream, let it write 16k.
      batch_write_stream_id_[priority] = id;
      bytes_left_for_batch_write_[priority] = 16000;
      last_priority_popped_ = priority;
    }

    return id;
  }

  void RegisterStream(QuicStreamId stream_id, gfe_spdy::SpdyPriority priority) {
    priority_write_scheduler_.RegisterStream(
        stream_id, gfe_spdy::SpdyStreamPrecedence(priority));
  }

  void UnregisterStream(QuicStreamId stream_id) {
    priority_write_scheduler_.UnregisterStream(stream_id);
  }

  void UpdateStreamPriority(QuicStreamId stream_id,
                            gfe_spdy::SpdyPriority new_priority) {
    priority_write_scheduler_.UpdateStreamPrecedence(
        stream_id, gfe_spdy::SpdyStreamPrecedence(new_priority));
  }

  void UpdateBytesForStream(QuicStreamId stream_id, size_t bytes) {
    if (batch_write_stream_id_[last_priority_popped_] == stream_id) {
      // If this was the last data stream popped by PopFront, update the
      // bytes remaining in its batch write.
      bytes_left_for_batch_write_[last_priority_popped_] -=
          static_cast<int32_t>(bytes);
    }
  }

  // Pushes a stream to the back of the list for its priority level *unless* it
  // is latched for doing batched writes in which case it goes to the front of
  // the list for its priority level.
  // Headers and crypto streams are special cased to always resume first.
  void AddStream(QuicStreamId stream_id) {
    if (stream_id == kCryptoStreamId) {
      // TODO Add DCHECK(!crypto_stream_blocked_)
      crypto_stream_blocked_ = true;
      return;
    }

    if (stream_id == kHeadersStreamId) {
      // TODO Add DCHECK(!headers_stream_blocked_);
      headers_stream_blocked_ = true;
      return;
    }
    bool push_front =
        stream_id == batch_write_stream_id_[last_priority_popped_] &&
        bytes_left_for_batch_write_[last_priority_popped_] > 0;
    priority_write_scheduler_.MarkStreamReady(stream_id, push_front);
  }

  // This function is used for debugging and test only. Returns true if stream
  // with |stream_id| is write blocked.
  bool IsStreamBlocked(QuicStreamId stream_id) const {
    if (stream_id == kCryptoStreamId) {
      return crypto_stream_blocked_;
    }

    if (stream_id == kHeadersStreamId) {
      return headers_stream_blocked_;
    }

    return priority_write_scheduler_.IsStreamReady(stream_id);
  }

 private:
  QuicPriorityWriteScheduler priority_write_scheduler_;

  // If performing batch writes, this will be the stream ID of the stream doing
  // batch writes for this priority level.  We will allow this stream to write
  // until it has written kBatchWriteSize bytes, it has no more data to write,
  // or a higher priority stream preempts.
  QuicStreamId batch_write_stream_id_[gfe_spdy::kV3LowestPriority + 1];
  // Set to kBatchWriteSize when we set a new batch_write_stream_id_ for a given
  // priority.  This is decremented with each write the stream does until it is
  // done with its batch write.
  int32_t bytes_left_for_batch_write_[gfe_spdy::kV3LowestPriority + 1];
  // Tracks the last priority popped for UpdateBytesForStream.
  gfe_spdy::SpdyPriority last_priority_popped_;

  bool crypto_stream_blocked_;
  bool headers_stream_blocked_;

  DISALLOW_COPY_AND_ASSIGN(QuicWriteBlockedList);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_WRITE_BLOCKED_LIST_H_
