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

#include "common/quic/core/quic_stream_sequencer_buffer.h"

#include "common/quic/platform/api/quic_bug_tracker.h"
#include "common/quic/platform/api/quic_flag_utils.h"
#include "common/quic/platform/api/quic_flags.h"
#include "common/quic/platform/api/quic_logging.h"
#include "common/quic/platform/api/quic_stack_trace.h"
#include "common/quic/platform/api/quic_str_cat.h"
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {
namespace {

size_t CalculateBlockCount(size_t max_capacity_bytes) {
  return (max_capacity_bytes + QuicStreamSequencerBuffer::kBlockSizeBytes - 1) /
         QuicStreamSequencerBuffer::kBlockSizeBytes;
}

// Upper limit of how many gaps allowed in buffer, which ensures a reasonable
// number of iterations needed to find the right gap to fill when a frame
// arrives.
const size_t kMaxNumDataIntervalsAllowed = 2 * kMaxPacketGap;

}  // namespace

QuicStreamSequencerBuffer::Gap::Gap(QuicStreamOffset begin_offset,
                                    QuicStreamOffset end_offset)
    : begin_offset(begin_offset), end_offset(end_offset) {}

QuicStreamSequencerBuffer::FrameInfo::FrameInfo()
    : length(1), timestamp(QuicTime::Zero()) {}

QuicStreamSequencerBuffer::FrameInfo::FrameInfo(size_t length,
                                                QuicTime timestamp)
    : length(length), timestamp(timestamp) {}

QuicStreamSequencerBuffer::QuicStreamSequencerBuffer(size_t max_capacity_bytes)
    : max_buffer_capacity_bytes_(max_capacity_bytes),
      blocks_count_(CalculateBlockCount(max_capacity_bytes)),
      total_bytes_read_(0),
      blocks_(nullptr),
      destruction_indicator_(123456) {
  Clear();
}

QuicStreamSequencerBuffer::~QuicStreamSequencerBuffer() {
  if (destruction_indicator_ != 123456) {
    QUIC_BUG
        << "QuicStreamSequencerBuffer use after free. Current stack trace: "
        << QuicStackTrace();
  }
  Clear();
  destruction_indicator_ = 654321;
}

void QuicStreamSequencerBuffer::Clear() {
  if (destruction_indicator_ != 123456) {
    QUIC_BUG
        << "QuicStreamSequencerBuffer use after free. Current stack trace: "
        << QuicStackTrace();
  }

  if (blocks_ != nullptr) {
    for (size_t i = 0; i < blocks_count_; ++i) {
      if (blocks_[i] != nullptr) {
        RetireBlock(i);
      }
    }
  }
  num_bytes_buffered_ = 0;
  bytes_received_.Clear();
  bytes_received_.Add(0, total_bytes_read_);

  frame_arrival_time_map_.clear();
}

bool QuicStreamSequencerBuffer::RetireBlock(size_t idx) {
  if (blocks_[idx] == nullptr) {
    QUIC_BUG << "Try to retire block twice";
    return false;
  }
  delete blocks_[idx];
  blocks_[idx] = nullptr;
  QUIC_DVLOG(1) << "Retired block with index: " << idx;
  return true;
}

QuicErrorCode QuicStreamSequencerBuffer::OnStreamData(
    QuicStreamOffset starting_offset,
    QuicStringPiece data,
    QuicTime timestamp,
    size_t* const bytes_buffered,
    QuicString* error_details) {
  if (destruction_indicator_ != 123456) {
    QUIC_BUG
        << "QuicStreamSequencerBuffer use after free. Current stack trace: "
        << QuicStackTrace();
  }

  *bytes_buffered = 0;
  size_t size = data.size();
  if (size == 0) {
    *error_details = "Received empty stream frame without FIN.";
    return QUIC_EMPTY_STREAM_FRAME_NO_FIN;
  }
  // Write beyond the current range this buffer is covering.
  if (starting_offset + size > total_bytes_read_ + max_buffer_capacity_bytes_ ||
      starting_offset + size < starting_offset) {
    *error_details = "Received data beyond available range.";
    return QUIC_INTERNAL_ERROR;
  }
  if (GetQuicReloadableFlag(quic_fast_path_on_stream_data) &&
      (bytes_received_.Empty() ||
       starting_offset >= bytes_received_.rbegin()->max() ||
       bytes_received_.IsDisjoint(Interval<QuicStreamOffset>(
           starting_offset, starting_offset + size)))) {
    // Optimization for the typical case, when all data is newly received.
    QUIC_FLAG_COUNT(gfe2_reloadable_flag_quic_fast_path_on_stream_data);
    if (!bytes_received_.Empty() &&
        starting_offset == bytes_received_.rbegin()->max()) {
      // Extend the right edge of last interval.
      // TODO: Encapsulate this into a future version of QuicIntervalSet
      // if this is more efficient than Add.
      const_cast<Interval<QuicPacketNumber>*>(&(*bytes_received_.rbegin()))
          ->SetMax(starting_offset + size);
    } else {
      bytes_received_.Add(starting_offset, starting_offset + size);
      if (bytes_received_.Size() >= kMaxNumDataIntervalsAllowed) {
        // This frame is going to create more intervals than allowed. Stop
        // processing.
        *error_details = "Too many data intervals received for this stream.";
        return QUIC_TOO_MANY_STREAM_DATA_INTERVALS;
      }
    }
    size_t bytes_copy = 0;
    if (!CopyStreamData(starting_offset, data, &bytes_copy, error_details)) {
      return QUIC_STREAM_SEQUENCER_INVALID_STATE;
    }
    *bytes_buffered += bytes_copy;
    frame_arrival_time_map_.insert(
        std::make_pair(starting_offset, FrameInfo(size, timestamp)));
    num_bytes_buffered_ += *bytes_buffered;
    return QUIC_NO_ERROR;
  }
  // Slow path, received data overlaps with received data.
  QuicIntervalSet<QuicStreamOffset> newly_received(starting_offset,
                                                   starting_offset + size);
  newly_received.Difference(bytes_received_);
  if (newly_received.Empty()) {
    return QUIC_NO_ERROR;
  }
  bytes_received_.Add(starting_offset, starting_offset + size);
  if (bytes_received_.Size() >= kMaxNumDataIntervalsAllowed) {
    // This frame is going to create more intervals than allowed. Stop
    // processing.
    *error_details = "Too many data intervals received for this stream.";
    return QUIC_TOO_MANY_STREAM_DATA_INTERVALS;
  }
  for (const auto& interval : newly_received) {
    const QuicStreamOffset copy_offset = interval.min();
    const QuicByteCount copy_length = interval.max() - interval.min();
    size_t bytes_copy = 0;
    if (!CopyStreamData(copy_offset,
                        data.substr(copy_offset - starting_offset, copy_length),
                        &bytes_copy, error_details)) {
      return QUIC_STREAM_SEQUENCER_INVALID_STATE;
    }
    *bytes_buffered += bytes_copy;
    frame_arrival_time_map_.insert(
        std::make_pair(copy_offset, FrameInfo(copy_length, timestamp)));
  }
  num_bytes_buffered_ += *bytes_buffered;
  return QUIC_NO_ERROR;
}

bool QuicStreamSequencerBuffer::CopyStreamData(QuicStreamOffset offset,
                                               QuicStringPiece data,
                                               size_t* bytes_copy,
                                               QuicString* error_details) {
  *bytes_copy = 0;
  size_t source_remaining = data.size();
  if (source_remaining == 0) {
    return true;
  }
  const char* source = data.data();
  // Write data block by block. If corresponding block has not created yet,
  // create it first.
  // Stop when all data are written or reaches the logical end of the buffer.
  while (source_remaining > 0) {
    const size_t write_block_num = GetBlockIndex(offset);
    const size_t write_block_offset = GetInBlockOffset(offset);
    DCHECK_GT(blocks_count_, write_block_num);

    size_t block_capacity = GetBlockCapacity(write_block_num);
    size_t bytes_avail = block_capacity - write_block_offset;

    // If this write meets the upper boundary of the buffer,
    // reduce the available free bytes.
    if (offset + bytes_avail > total_bytes_read_ + max_buffer_capacity_bytes_) {
      bytes_avail = total_bytes_read_ + max_buffer_capacity_bytes_ - offset;
    }

    if (blocks_ == nullptr) {
      blocks_.reset(new BufferBlock*[blocks_count_]());
      for (size_t i = 0; i < blocks_count_; ++i) {
        blocks_[i] = nullptr;
      }
    }

    if (write_block_num >= blocks_count_) {
      *error_details = QuicStrCat(
          "QuicStreamSequencerBuffer error: OnStreamData() exceed array bounds."
          "write offset = ",
          offset, " write_block_num = ", write_block_num,
          " blocks_count_ = ", blocks_count_);
      return false;
    }
    if (blocks_ == nullptr) {
      *error_details =
          "QuicStreamSequencerBuffer error: OnStreamData() blocks_ is null";
      return false;
    }

    // This is added temporarily for debugging b/32178749.
    if (reinterpret_cast<uint64_t>(blocks_.get() + write_block_num) ==
        std::numeric_limits<uint64_t>::max()) {
      QUIC_BUG << "QuicStreamSequencerBuffer::OnStreamData() blocks_ = "
               << blocks_.get() << " blocks_count_ = " << blocks_count_
               << " write_block_num = " << write_block_num
               << " offset = " << offset
               << ", use after free: " << (destruction_indicator_ != 123456)
               << " Current stack trace: " << QuicStackTrace();
    }

    if (blocks_[write_block_num] == nullptr) {
      // TODO: Investigate if using a freelist would improve performance.
      // Same as RetireBlock().
      blocks_[write_block_num] = new BufferBlock();
    }

    const size_t bytes_to_copy =
        std::min<size_t>(bytes_avail, source_remaining);
    char* dest = blocks_[write_block_num]->buffer + write_block_offset;
    QUIC_DVLOG(1) << "Write at offset: " << offset
                  << " length: " << bytes_to_copy;

    if (dest == nullptr || source == nullptr) {
      *error_details = QuicStrCat(
          "QuicStreamSequencerBuffer error: OnStreamData()"
          " dest == nullptr: ",
          (dest == nullptr), " source == nullptr: ", (source == nullptr),
          " Writing at offset ", offset, " Gaps: ", GapsDebugString(),
          " Remaining frames: ", ReceivedFramesDebugString(),
          " total_bytes_read_ = ", total_bytes_read_);
      return false;
    }
    memcpy(dest, source, bytes_to_copy);
    source += bytes_to_copy;
    source_remaining -= bytes_to_copy;
    offset += bytes_to_copy;
    *bytes_copy += bytes_to_copy;
  }
  return true;
}

QuicErrorCode QuicStreamSequencerBuffer::Readv(const iovec* dest_iov,
                                               size_t dest_count,
                                               size_t* bytes_read,
                                               QuicString* error_details) {
  if (destruction_indicator_ != 123456) {
    QUIC_BUG
        << "QuicStreamSequencerBuffer use after free. Current stack trace: "
        << QuicStackTrace();
  }

  *bytes_read = 0;
  for (size_t i = 0; i < dest_count && ReadableBytes() > 0; ++i) {
    char* dest = reinterpret_cast<char*>(dest_iov[i].iov_base);
    size_t dest_remaining = dest_iov[i].iov_len;
    while (dest_remaining > 0 && ReadableBytes() > 0) {
      size_t block_idx = NextBlockToRead();
      size_t start_offset_in_block = ReadOffset();
      size_t block_capacity = GetBlockCapacity(block_idx);
      size_t bytes_available_in_block = std::min<size_t>(
          ReadableBytes(), block_capacity - start_offset_in_block);
      size_t bytes_to_copy =
          std::min<size_t>(bytes_available_in_block, dest_remaining);
      DCHECK_GT(bytes_to_copy, 0u);
      if (blocks_[block_idx] == nullptr || dest == nullptr) {
        *error_details = QuicStrCat(
            "QuicStreamSequencerBuffer error:"
            " Readv() dest == nullptr: ",
            (dest == nullptr), " blocks_[", block_idx,
            "] == nullptr: ", (blocks_[block_idx] == nullptr),
            " Gaps: ", GapsDebugString(),
            " Remaining frames: ", ReceivedFramesDebugString(),
            " total_bytes_read_ = ", total_bytes_read_);
        return QUIC_STREAM_SEQUENCER_INVALID_STATE;
      }
      memcpy(dest, blocks_[block_idx]->buffer + start_offset_in_block,
             bytes_to_copy);
      dest += bytes_to_copy;
      dest_remaining -= bytes_to_copy;
      num_bytes_buffered_ -= bytes_to_copy;
      total_bytes_read_ += bytes_to_copy;
      *bytes_read += bytes_to_copy;

      // Retire the block if all the data is read out and no other data is
      // stored in this block.
      // In case of failing to retire a block which is ready to retire, return
      // immediately.
      if (bytes_to_copy == bytes_available_in_block) {
        bool retire_successfully = RetireBlockIfEmpty(block_idx);
        if (!retire_successfully) {
          *error_details = QuicStrCat(
              "QuicStreamSequencerBuffer error: fail to retire block ",
              block_idx,
              " as the block is already released, total_bytes_read_ = ",
              total_bytes_read_, " Gaps: ", GapsDebugString());
          return QUIC_STREAM_SEQUENCER_INVALID_STATE;
        }
      }
    }
  }

  if (*bytes_read > 0) {
    UpdateFrameArrivalMap(total_bytes_read_);
  }
  return QUIC_NO_ERROR;
}

int QuicStreamSequencerBuffer::GetReadableRegions(struct iovec* iov,
                                                  int iov_count) const {
  if (destruction_indicator_ != 123456) {
    QUIC_BUG
        << "QuicStreamSequencerBuffer use after free. Current stack trace: "
        << QuicStackTrace();
  }

  DCHECK(iov != nullptr);
  DCHECK_GT(iov_count, 0);

  if (ReadableBytes() == 0) {
    iov[0].iov_base = nullptr;
    iov[0].iov_len = 0;
    return 0;
  }

  size_t start_block_idx = NextBlockToRead();
  QuicStreamOffset readable_offset_end = FirstMissingByte() - 1;
  DCHECK_GE(readable_offset_end + 1, total_bytes_read_);
  size_t end_block_offset = GetInBlockOffset(readable_offset_end);
  size_t end_block_idx = GetBlockIndex(readable_offset_end);

  // If readable region is within one block, deal with it seperately.
  if (start_block_idx == end_block_idx && ReadOffset() <= end_block_offset) {
    iov[0].iov_base = blocks_[start_block_idx]->buffer + ReadOffset();
    iov[0].iov_len = ReadableBytes();
    QUIC_DVLOG(1) << "Got only a single block with index: " << start_block_idx;
    return 1;
  }

  // Get first block
  iov[0].iov_base = blocks_[start_block_idx]->buffer + ReadOffset();
  iov[0].iov_len = GetBlockCapacity(start_block_idx) - ReadOffset();
  QUIC_DVLOG(1) << "Got first block " << start_block_idx << " with len "
                << iov[0].iov_len;
  DCHECK_GT(readable_offset_end + 1, total_bytes_read_ + iov[0].iov_len)
      << "there should be more available data";

  // Get readable regions of the rest blocks till either 2nd to last block
  // before gap is met or |iov| is filled. For these blocks, one whole block is
  // a region.
  int iov_used = 1;
  int block_idx = (start_block_idx + iov_used) % blocks_count_;
  while (block_idx != end_block_idx && iov_used < iov_count) {
    DCHECK_NE(nullptr, blocks_[block_idx]);
    iov[iov_used].iov_base = blocks_[block_idx]->buffer;
    iov[iov_used].iov_len = GetBlockCapacity(block_idx);
    QUIC_DVLOG(1) << "Got block with index: " << block_idx;
    ++iov_used;
    block_idx = (start_block_idx + iov_used) % blocks_count_;
  }

  // Deal with last block if |iov| can hold more.
  if (iov_used < iov_count) {
    DCHECK_NE(nullptr, blocks_[block_idx]);
    iov[iov_used].iov_base = blocks_[end_block_idx]->buffer;
    iov[iov_used].iov_len = end_block_offset + 1;
    QUIC_DVLOG(1) << "Got last block with index: " << end_block_idx;
    ++iov_used;
  }
  return iov_used;
}

bool QuicStreamSequencerBuffer::GetReadableRegion(iovec* iov,
                                                  QuicTime* timestamp) const {
  if (destruction_indicator_ != 123456) {
    QUIC_BUG
        << "QuicStreamSequencerBuffer use after free. Current stack trace: "
        << QuicStackTrace();
  }

  if (ReadableBytes() == 0) {
    iov[0].iov_base = nullptr;
    iov[0].iov_len = 0;
    return false;
  }

  size_t start_block_idx = NextBlockToRead();
  iov->iov_base = blocks_[start_block_idx]->buffer + ReadOffset();
  size_t readable_bytes_in_block = std::min<size_t>(
      GetBlockCapacity(start_block_idx) - ReadOffset(), ReadableBytes());
  size_t region_len = 0;
  auto iter = frame_arrival_time_map_.begin();
  *timestamp = iter->second.timestamp;
  QUIC_DVLOG(1) << "Readable bytes in block: " << readable_bytes_in_block;
  for (; iter != frame_arrival_time_map_.end() &&
         region_len + iter->second.length <= readable_bytes_in_block;
       ++iter) {
    if (iter->second.timestamp != *timestamp) {
      // If reaches a frame arrive at another timestamp, stop expanding current
      // region.
      QUIC_DVLOG(1) << "Meet frame with different timestamp.";
      break;
    }
    region_len += iter->second.length;
    QUIC_DVLOG(1) << "Added bytes to region: " << iter->second.length;
  }
  if (iter == frame_arrival_time_map_.end() ||
      iter->second.timestamp == *timestamp) {
    // If encountered the end of readable bytes before reaching a different
    // timestamp.
    QUIC_DVLOG(1) << "Got all readable bytes in first block.";
    region_len = readable_bytes_in_block;
  }
  iov->iov_len = region_len;
  return true;
}

bool QuicStreamSequencerBuffer::MarkConsumed(size_t bytes_used) {
  if (destruction_indicator_ != 123456) {
    QUIC_BUG
        << "QuicStreamSequencerBuffer use after free. Current stack trace: "
        << QuicStackTrace();
  }

  if (bytes_used > ReadableBytes()) {
    return false;
  }
  size_t bytes_to_consume = bytes_used;
  while (bytes_to_consume > 0) {
    size_t block_idx = NextBlockToRead();
    size_t offset_in_block = ReadOffset();
    size_t bytes_available = std::min<size_t>(
        ReadableBytes(), GetBlockCapacity(block_idx) - offset_in_block);
    size_t bytes_read = std::min<size_t>(bytes_to_consume, bytes_available);
    total_bytes_read_ += bytes_read;
    num_bytes_buffered_ -= bytes_read;
    bytes_to_consume -= bytes_read;
    // If advanced to the end of current block and end of buffer hasn't wrapped
    // to this block yet.
    if (bytes_available == bytes_read) {
      RetireBlockIfEmpty(block_idx);
    }
  }
  if (bytes_used > 0) {
    UpdateFrameArrivalMap(total_bytes_read_);
  }
  return true;
}

size_t QuicStreamSequencerBuffer::FlushBufferedFrames() {
  if (destruction_indicator_ != 123456) {
    QUIC_BUG
        << "QuicStreamSequencerBuffer use after free. Current stack trace: "
        << QuicStackTrace();
  }
  size_t prev_total_bytes_read = total_bytes_read_;
  total_bytes_read_ = NextExpectedByte();
  Clear();
  return total_bytes_read_ - prev_total_bytes_read;
}

void QuicStreamSequencerBuffer::ReleaseWholeBuffer() {
  if (destruction_indicator_ != 123456) {
    QUIC_BUG
        << "QuicStreamSequencerBuffer use after free. Current stack trace: "
        << QuicStackTrace();
  }

  Clear();
  blocks_.reset(nullptr);
}

size_t QuicStreamSequencerBuffer::ReadableBytes() const {
  return FirstMissingByte() - total_bytes_read_;
}

bool QuicStreamSequencerBuffer::HasBytesToRead() const {
  return ReadableBytes() > 0;
}

QuicStreamOffset QuicStreamSequencerBuffer::BytesConsumed() const {
  return total_bytes_read_;
}

size_t QuicStreamSequencerBuffer::BytesBuffered() const {
  return num_bytes_buffered_;
}

size_t QuicStreamSequencerBuffer::GetBlockIndex(QuicStreamOffset offset) const {
  return (offset % max_buffer_capacity_bytes_) / kBlockSizeBytes;
}

size_t QuicStreamSequencerBuffer::GetInBlockOffset(
    QuicStreamOffset offset) const {
  return (offset % max_buffer_capacity_bytes_) % kBlockSizeBytes;
}

size_t QuicStreamSequencerBuffer::ReadOffset() const {
  return GetInBlockOffset(total_bytes_read_);
}

size_t QuicStreamSequencerBuffer::NextBlockToRead() const {
  return GetBlockIndex(total_bytes_read_);
}

bool QuicStreamSequencerBuffer::RetireBlockIfEmpty(size_t block_index) {
  DCHECK(ReadableBytes() == 0 || GetInBlockOffset(total_bytes_read_) == 0)
      << "RetireBlockIfEmpty() should only be called when advancing to next "
      << "block or a gap has been reached.";
  // If the whole buffer becomes empty, the last piece of data has been read.
  if (Empty()) {
    return RetireBlock(block_index);
  }

  // Check where the logical end of this buffer is.
  // Not empty if the end of circular buffer has been wrapped to this block.
  if (GetBlockIndex(NextExpectedByte() - 1) == block_index) {
    return true;
  }

  // Read index remains in this block, which means a gap has been reached.
  if (NextBlockToRead() == block_index) {
    if (bytes_received_.Size() > 1) {
      auto it = bytes_received_.begin();
      ++it;
      if (GetBlockIndex(it->min()) == block_index) {
        // Do not retire the block if next data interval is in this block.
        return true;
      }
    } else {
      QUIC_BUG << "Read stopped at where it shouldn't.";
      return false;
    }
  }
  return RetireBlock(block_index);
}

bool QuicStreamSequencerBuffer::Empty() const {
  return bytes_received_.Empty() ||
         (bytes_received_.Size() == 1 && total_bytes_read_ > 0 &&
          bytes_received_.begin()->max() == total_bytes_read_);
}

size_t QuicStreamSequencerBuffer::GetBlockCapacity(size_t block_index) const {
  if ((block_index + 1) == blocks_count_) {
    size_t result = max_buffer_capacity_bytes_ % kBlockSizeBytes;
    if (result == 0) {  // whole block
      result = kBlockSizeBytes;
    }
    return result;
  } else {
    return kBlockSizeBytes;
  }
}

void QuicStreamSequencerBuffer::UpdateFrameArrivalMap(QuicStreamOffset offset) {
  // Get the frame before which all frames should be removed.
  auto next_frame = frame_arrival_time_map_.upper_bound(offset);
  DCHECK(next_frame != frame_arrival_time_map_.begin());
  auto iter = frame_arrival_time_map_.begin();
  while (iter != next_frame) {
    auto erased = *iter;
    iter = frame_arrival_time_map_.erase(iter);
    QUIC_DVLOG(1) << "Removed FrameInfo with offset: " << erased.first
                  << " and length: " << erased.second.length;
    if (erased.first + erased.second.length > offset) {
      // If last frame is partially read out, update this FrameInfo and insert
      // it back.
      auto updated = std::make_pair(
          offset, FrameInfo(erased.first + erased.second.length - offset,
                            erased.second.timestamp));
      QUIC_DVLOG(1) << "Inserted FrameInfo with offset: " << updated.first
                    << " and length: " << updated.second.length;
      frame_arrival_time_map_.insert(updated);
    }
  }
}

QuicString QuicStreamSequencerBuffer::GapsDebugString() {
  return bytes_received_.ToString();
}

QuicString QuicStreamSequencerBuffer::ReceivedFramesDebugString() {
  QuicString current_frames_string;
  for (auto it : frame_arrival_time_map_) {
    QuicStreamOffset current_frame_begin_offset = it.first;
    QuicStreamOffset current_frame_end_offset =
        it.second.length + current_frame_begin_offset;
    current_frames_string.append(QuicStrCat(
        "[", current_frame_begin_offset, ", ", current_frame_end_offset,
        ") receiving time ", it.second.timestamp.ToDebuggingValue()));
  }
  return current_frames_string;
}

QuicStreamOffset QuicStreamSequencerBuffer::FirstMissingByte() const {
  if (bytes_received_.Empty() || bytes_received_.begin()->min() > 0) {
    // Offset 0 is not received yet.
    return 0;
  }
  return bytes_received_.begin()->max();
}

QuicStreamOffset QuicStreamSequencerBuffer::NextExpectedByte() const {
  if (bytes_received_.Empty()) {
    return 0;
  }
  return bytes_received_.rbegin()->max();
}

}  //  namespace gfe_quic
