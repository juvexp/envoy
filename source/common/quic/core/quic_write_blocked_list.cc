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

#include "common/quic/core/quic_write_blocked_list.h"

#include "common/quic/platform/api/quic_flag_utils.h"
#include "common/quic/platform/api/quic_flags.h"

namespace gfe_quic {

QuicWriteBlockedList::QuicWriteBlockedList(bool register_static_streams)
    : last_priority_popped_(0),
      crypto_stream_blocked_(false),
      headers_stream_blocked_(false),
      register_static_streams_(register_static_streams) {
  if (register_static_streams_) {
    QUIC_FLAG_COUNT(gfe2_reloadable_flag_quic_register_static_streams);
  }
  memset(batch_write_stream_id_, 0, sizeof(batch_write_stream_id_));
  memset(bytes_left_for_batch_write_, 0, sizeof(bytes_left_for_batch_write_));
}

QuicWriteBlockedList::~QuicWriteBlockedList() {}

}  // namespace gfe_quic
