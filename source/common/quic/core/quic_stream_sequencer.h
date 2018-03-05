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

#ifndef GFE_QUIC_CORE_QUIC_STREAM_SEQUENCER_H_
#define GFE_QUIC_CORE_QUIC_STREAM_SEQUENCER_H_

#include <cstddef>
#include <map>

#include "base/macros.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_stream_sequencer_buffer.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {

namespace test {
class QuicStreamSequencerPeer;
}  // namespace test

class QuicClock;
class QuicStream;

// Buffers frames until we have something which can be passed
// up to the next layer.
class QUIC_EXPORT_PRIVATE QuicStreamSequencer {
 public:
  QuicStreamSequencer(QuicStream* quic_stream, const QuicClock* clock);
  virtual ~QuicStreamSequencer();

  // If the frame is the next one we need in order to process in-order data,
  // ProcessData will be immediately called on the stream until all buffered
  // data is processed or the stream fails to consume data.  Any unconsumed
  // data will be buffered. If the frame is not the next in line, it will be
  // buffered.
  void OnStreamFrame(const QuicStreamFrame& frame);

  // Once data is buffered, it's up to the stream to read it when the stream
  // can handle more data.  The following three functions make that possible.

  // Fills in up to iov_len iovecs with the next readable regions.  Returns the
  // number of iovs used.  Non-destructive of the underlying data.
  int GetReadableRegions(iovec* iov, size_t iov_len) const;

  // Fills in one iovec with the next readable region.  |timestamp| is
  // data arrived at the sequencer, and is used for measuring head of
  // line blocking (HOL).  Returns false if there is no readable
  // region available.
  bool GetReadableRegion(iovec* iov, QuicTime* timestamp) const;

  // Copies the data into the iov_len buffers provided.  Returns the number of
  // bytes read.  Any buffered data no longer in use will be released.
  // TODO: remove this method and instead implement it as a helper method
  // based on GetReadableRegions and MarkConsumed.
  int Readv(const struct iovec* iov, size_t iov_len);

  // Consumes |num_bytes| data.  Used in conjunction with |GetReadableRegions|
  // to do zero-copy reads.
  void MarkConsumed(size_t num_bytes);

  // Returns true if the sequncer has bytes available for reading.
  bool HasBytesToRead() const;

  // Number of bytes available to read.
  size_t ReadableBytes() const;

  // Returns true if the sequencer has delivered the fin.
  bool IsClosed() const;

  // Calls |OnDataAvailable| on |stream_| if there is buffered data that can
  // be processed, and causes |OnDataAvailable| to be called as new data
  // arrives.
  void SetUnblocked();

  // Blocks processing of frames until |SetUnblocked| is called.
  void SetBlockedUntilFlush();

  // Sets the sequencer to discard all incoming data itself and not call
  // |stream_->OnDataAvailable()|.  |stream_->OnFinRead()| will be called
  // automatically when the FIN is consumed (which may be immediately).
  void StopReading();

  // Free the memory of underlying buffer.
  void ReleaseBuffer();

  // Free the memory of underlying buffer when no bytes remain in it.
  void ReleaseBufferIfEmpty();

  // Number of bytes in the buffer right now.
  size_t NumBytesBuffered() const;

  // Number of bytes has been consumed.
  QuicStreamOffset NumBytesConsumed() const;

  int num_frames_received() const { return num_frames_received_; }

  int num_duplicate_frames_received() const {
    return num_duplicate_frames_received_;
  }

  bool ignore_read_data() const { return ignore_read_data_; }

  void set_level_triggered(bool level_triggered) {
    level_triggered_ = level_triggered;
  }

  bool level_triggered() const { return level_triggered_; }

  // Returns string describing internal state.
  const QuicString DebugString() const;

 private:
  friend class test::QuicStreamSequencerPeer;

  // Deletes and records as consumed any buffered data that is now in-sequence.
  // (To be called only after StopReading has been called.)
  void FlushBufferedFrames();

  // Wait until we've seen 'offset' bytes, and then terminate the stream.
  void CloseStreamAtOffset(QuicStreamOffset offset);

  // If we've received a FIN and have processed all remaining data, then inform
  // the stream of FIN, and clear buffers.
  bool MaybeCloseStream();

  // The stream which owns this sequencer.
  QuicStream* stream_;

  // Stores received data in offset order.
  QuicStreamSequencerBuffer buffered_frames_;

  // The offset, if any, we got a stream termination for.  When this many bytes
  // have been processed, the sequencer will be closed.
  QuicStreamOffset close_offset_;

  // If true, the sequencer is blocked from passing data to the stream and will
  // buffer all new incoming data until FlushBufferedFrames is called.
  bool blocked_;

  // Count of the number of frames received.
  int num_frames_received_;

  // Count of the number of duplicate frames received.
  int num_duplicate_frames_received_;

  // Not owned.
  const QuicClock* clock_;

  // If true, all incoming data will be discarded.
  bool ignore_read_data_;

  // If false, only call OnDataAvailable() when it becomes newly unblocked.
  // Otherwise, call OnDataAvailable() when number of readable bytes changes.
  bool level_triggered_;

  DISALLOW_COPY_AND_ASSIGN(QuicStreamSequencer);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_STREAM_SEQUENCER_H_
