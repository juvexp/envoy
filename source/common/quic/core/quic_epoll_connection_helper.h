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

// The epoll-specific helper for QuicConnection which uses
// EpollAlarm for alarms, and used an int fd_ for writing data.

#ifndef GFE_QUIC_CORE_QUIC_EPOLL_CONNECTION_HELPER_H_
#define GFE_QUIC_CORE_QUIC_EPOLL_CONNECTION_HELPER_H_

#include <sys/types.h>
#include <set>

#include "common/quic/core/quic_buffer_pool_google3.h"
#include "common/quic/core/quic_connection.h"
#include "common/quic/core/quic_default_packet_writer.h"
#include "common/quic/core/quic_packet_writer.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_simple_buffer_allocator.h"
#include "common/quic/core/quic_time.h"
#include "common/quic/platform/api/quic_socket_address.h"
#include "common/quic/platform/impl/quic_epoll_clock.h"

namespace gfe2 {
class EpollServer;
}  // namespace gfe2

namespace gfe_quic {

class QuicRandom;
class AckAlarm;
class RetransmissionAlarm;
class SendAlarm;
class TimeoutAlarm;

// Create a buffer free list using no more than 8 * 8MB memory.
// Google3-specific; Chromium should just use SimpleBufferAllocator.
using QuicStreamFrameBufferAllocator = QuicBufferPool<kMaxPacketSize, 8>;
using QuicStreamBufferAllocator =
    QuicBufferPool<kQuicStreamSendBufferSliceSize, 8>;

enum class QuicAllocator { SIMPLE, BUFFER_POOL };

class QuicEpollConnectionHelper : public QuicConnectionHelperInterface {
 public:
  QuicEpollConnectionHelper(gfe2::EpollServer* eps, QuicAllocator allocator);
  ~QuicEpollConnectionHelper() override;

  // QuicConnectionHelperInterface
  const QuicClock* GetClock() const override;
  QuicRandom* GetRandomGenerator() override;
  QuicBufferAllocator* GetStreamSendBufferAllocator() override;

 private:
  friend class QuicEpollConnectionHelperPeer;

  const QuicEpollClock clock_;
  QuicRandom* random_generator_;
  // Set up allocators.  They take up minimal memory before use.
  // Allocator for stream send buffers.
  QuicStreamBufferAllocator stream_buffer_allocator_;
  SimpleBufferAllocator simple_buffer_allocator_;
  QuicAllocator allocator_type_;

  DISALLOW_COPY_AND_ASSIGN(QuicEpollConnectionHelper);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_EPOLL_CONNECTION_HELPER_H_
