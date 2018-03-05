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

#ifndef GFE_SPDY_CORE_PRIORITY_WRITE_SCHEDULER_H_
#define GFE_SPDY_CORE_PRIORITY_WRITE_SCHEDULER_H_

#include <algorithm>
#include <cstdint>
#include <deque>
#include <tuple>
#include <unordered_map>
#include <utility>

#include "base/logging.h"
#include "common/quic/spdy_utils/core/spdy_bug_tracker.h"
#include "common/quic/spdy_utils/core/spdy_protocol.h"
#include "common/quic/spdy_utils/core/write_scheduler.h"
#include "common/quic/spdy_utils/platform/api/spdy_string.h"
#include "common/quic/spdy_utils/platform/api/spdy_string_utils.h"

namespace gfe_spdy {

namespace test {
template <typename StreamIdType>
class PriorityWriteSchedulerPeer;
}

// WriteScheduler implementation that manages the order in which streams are
// written using the SPDY priority scheme described at:
// https://www.chromium.org/spdy/spdy-protocol/spdy-protocol-draft3-1#TOC-2.3.3-Stream-priority
//
// Internally, PriorityWriteScheduler consists of 8 PriorityInfo objects, one
// for each priority value.  Each PriorityInfo contains a list of streams of
// that priority that are ready to write, as well as a timestamp of the last
// I/O event that occurred for a stream of that priority.
//
// DO NOT USE. Deprecated.
template <typename StreamIdType>
class PriorityWriteScheduler : public WriteScheduler<StreamIdType> {
 public:
  using typename WriteScheduler<StreamIdType>::StreamPrecedenceType;

  // Creates scheduler with no streams.
  PriorityWriteScheduler() = default;

  void RegisterStream(StreamIdType stream_id,
                      const StreamPrecedenceType& precedence) override {
    // TODO: verify |precedence.is_spdy3_priority() == true| once
    //   Http2PriorityWriteScheduler enabled for HTTP/2.

    // parent_id not used here, but may as well validate it.  However,
    // parent_id may legitimately not be registered yet--see b/15676312.
    StreamIdType parent_id = precedence.parent_id();
    VLOG_IF(1, parent_id != kHttp2RootStreamId && !StreamRegistered(parent_id))
        << "Parent stream " << parent_id << " not registered";

    if (stream_id == kHttp2RootStreamId) {
      SPDY_BUG << "Stream " << kHttp2RootStreamId << " already registered";
      return;
    }
    StreamInfo stream_info = {precedence.spdy3_priority(), stream_id, false};
    bool inserted =
        stream_infos_.insert(std::make_pair(stream_id, stream_info)).second;
    SPDY_BUG_IF(!inserted) << "Stream " << stream_id << " already registered";
  }

  void UnregisterStream(StreamIdType stream_id) override {
    auto it = stream_infos_.find(stream_id);
    if (it == stream_infos_.end()) {
      SPDY_BUG << "Stream " << stream_id << " not registered";
      return;
    }
    StreamInfo& stream_info = it->second;
    if (stream_info.ready) {
      bool erased =
          Erase(&priority_infos_[stream_info.priority].ready_list, stream_info);
      DCHECK(erased);
    }
    stream_infos_.erase(it);
  }

  bool StreamRegistered(StreamIdType stream_id) const override {
    return stream_infos_.find(stream_id) != stream_infos_.end();
  }

  StreamPrecedenceType GetStreamPrecedence(
      StreamIdType stream_id) const override {
    auto it = stream_infos_.find(stream_id);
    if (it == stream_infos_.end()) {
      VLOG(1) << "Stream " << stream_id << " not registered";
      return StreamPrecedenceType(kV3LowestPriority);
    }
    return StreamPrecedenceType(it->second.priority);
  }

  void UpdateStreamPrecedence(StreamIdType stream_id,
                              const StreamPrecedenceType& precedence) override {
    // TODO: verify |precedence.is_spdy3_priority() == true| once
    //   Http2PriorityWriteScheduler enabled for HTTP/2.

    // parent_id not used here, but may as well validate it.  However,
    // parent_id may legitimately not be registered yet--see b/15676312.
    StreamIdType parent_id = precedence.parent_id();
    VLOG_IF(1, parent_id != kHttp2RootStreamId && !StreamRegistered(parent_id))
        << "Parent stream " << parent_id << " not registered";

    auto it = stream_infos_.find(stream_id);
    if (it == stream_infos_.end()) {
      // TODO: add to stream_infos_ on demand--see b/15676312.
      VLOG(1) << "Stream " << stream_id << " not registered";
      return;
    }
    StreamInfo& stream_info = it->second;
    SpdyPriority new_priority = precedence.spdy3_priority();
    if (stream_info.priority == new_priority) {
      return;
    }
    if (stream_info.ready) {
      bool erased =
          Erase(&priority_infos_[stream_info.priority].ready_list, stream_info);
      DCHECK(erased);
      priority_infos_[new_priority].ready_list.push_back(&stream_info);
      ++num_ready_streams_;
    }
    stream_info.priority = new_priority;
  }

  std::vector<StreamIdType> GetStreamChildren(
      StreamIdType stream_id) const override {
    return std::vector<StreamIdType>();
  }

  void RecordStreamEventTime(StreamIdType stream_id,
                             int64_t now_in_usec) override {
    auto it = stream_infos_.find(stream_id);
    if (it == stream_infos_.end()) {
      SPDY_BUG << "Stream " << stream_id << " not registered";
      return;
    }
    PriorityInfo& priority_info = priority_infos_[it->second.priority];
    priority_info.last_event_time_usec =
        std::max(priority_info.last_event_time_usec, now_in_usec);
  }

  int64_t GetLatestEventWithPrecedence(StreamIdType stream_id) const override {
    auto it = stream_infos_.find(stream_id);
    if (it == stream_infos_.end()) {
      SPDY_BUG << "Stream " << stream_id << " not registered";
      return 0;
    }
    int64_t last_event_time_usec = 0;
    const StreamInfo& stream_info = it->second;
    for (SpdyPriority p = kV3HighestPriority; p < stream_info.priority; ++p) {
      last_event_time_usec = std::max(last_event_time_usec,
                                      priority_infos_[p].last_event_time_usec);
    }
    return last_event_time_usec;
  }

  StreamIdType PopNextReadyStream() override {
    return std::get<0>(PopNextReadyStreamAndPrecedence());
  }

  // Returns the next ready stream and its precedence.
  std::tuple<StreamIdType, StreamPrecedenceType>
  PopNextReadyStreamAndPrecedence() override {
    for (SpdyPriority p = kV3HighestPriority; p <= kV3LowestPriority; ++p) {
      ReadyList& ready_list = priority_infos_[p].ready_list;
      if (!ready_list.empty()) {
        StreamInfo* info = ready_list.front();
        ready_list.pop_front();
        --num_ready_streams_;

        DCHECK(stream_infos_.find(info->stream_id) != stream_infos_.end());
        info->ready = false;
        return std::make_tuple(info->stream_id,
                               StreamPrecedenceType(info->priority));
      }
    }
    SPDY_BUG << "No ready streams available";
    return std::make_tuple(0, StreamPrecedenceType(kV3LowestPriority));
  }

  bool ShouldYield(StreamIdType stream_id) const override {
    auto it = stream_infos_.find(stream_id);
    if (it == stream_infos_.end()) {
      SPDY_BUG << "Stream " << stream_id << " not registered";
      return false;
    }

    // If there's a higher priority stream, this stream should yield.
    const StreamInfo& stream_info = it->second;
    for (SpdyPriority p = kV3HighestPriority; p < stream_info.priority; ++p) {
      if (!priority_infos_[p].ready_list.empty()) {
        return true;
      }
    }

    // If this priority level is empty, or this stream is the next up, there's
    // no need to yield.
    const auto& ready_list = priority_infos_[it->second.priority].ready_list;
    if (ready_list.empty() || ready_list.front()->stream_id == stream_id) {
      return false;
    }

    // There are other streams in this priority level which take precedence.
    // Yield.
    return true;
  }

  void MarkStreamReady(StreamIdType stream_id, bool add_to_front) override {
    auto it = stream_infos_.find(stream_id);
    if (it == stream_infos_.end()) {
      SPDY_BUG << "Stream " << stream_id << " not registered";
      return;
    }
    StreamInfo& stream_info = it->second;
    if (stream_info.ready) {
      return;
    }
    ReadyList& ready_list = priority_infos_[stream_info.priority].ready_list;
    if (add_to_front) {
      ready_list.push_front(&stream_info);
    } else {
      ready_list.push_back(&stream_info);
    }
    ++num_ready_streams_;
    stream_info.ready = true;
  }

  void MarkStreamNotReady(StreamIdType stream_id) override {
    auto it = stream_infos_.find(stream_id);
    if (it == stream_infos_.end()) {
      SPDY_BUG << "Stream " << stream_id << " not registered";
      return;
    }
    StreamInfo& stream_info = it->second;
    if (!stream_info.ready) {
      return;
    }
    bool erased =
        Erase(&priority_infos_[stream_info.priority].ready_list, stream_info);
    DCHECK(erased);
    stream_info.ready = false;
  }

  // Returns true iff the number of ready streams is non-zero.
  bool HasReadyStreams() const override { return num_ready_streams_ > 0; }

  // Returns the number of ready streams.
  size_t NumReadyStreams() const override { return num_ready_streams_; }

  SpdyString DebugString() const override {
    return SpdyStrCat(
        "PriorityWriteScheduler {num_streams=", stream_infos_.size(),
        " num_ready_streams=", NumReadyStreams(), "}");
  }

  // This function is used for debugging and test only. Returns true if a stream
  // is ready.
  bool IsStreamReady(StreamIdType stream_id) const {
    auto it = stream_infos_.find(stream_id);
    if (it == stream_infos_.end()) {
      DLOG(INFO) << "Stream " << stream_id << " not registered";
      return false;
    }
    return it->second.ready;
  }

 private:
  friend class test::PriorityWriteSchedulerPeer<StreamIdType>;

  // State kept for all registered streams. All ready streams have ready = true
  // and should be present in priority_infos_[priority].ready_list.
  struct StreamInfo {
    SpdyPriority priority;
    StreamIdType stream_id;
    bool ready;
  };

  // 0(1) size lookup, 0(1) insert at front or back.
  typedef std::deque<StreamInfo*> ReadyList;

  // State kept for each priority level.
  struct PriorityInfo {
    // IDs of streams that are ready to write.
    ReadyList ready_list;
    // Time of latest write event for stream of this priority, in microseconds.
    int64_t last_event_time_usec = 0;
  };

  typedef std::unordered_map<StreamIdType, StreamInfo> StreamInfoMap;

  // Erases first occurrence (which should be the only one) of |info| in
  // |ready_list|, returning true if found (and erased), or false otherwise.
  // Decrements |num_ready_streams_| if an entry is erased.
  bool Erase(ReadyList* ready_list, const StreamInfo& info) {
    auto it = std::find(ready_list->begin(), ready_list->end(), &info);
    if (it == ready_list->end()) {
      return false;
    }
    ready_list->erase(it);
    --num_ready_streams_;
    return true;
  }

  // Number of ready streams.
  size_t num_ready_streams_ = 0;
  // Per-priority state, including ready lists.
  PriorityInfo priority_infos_[kV3LowestPriority + 1];
  // StreamInfos for all registered streams.
  StreamInfoMap stream_infos_;
};

}  // namespace gfe_spdy

#endif  // GFE_SPDY_CORE_PRIORITY_WRITE_SCHEDULER_H_
