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

#include "common/quic/core/quic_epoll_connection_helper.h"

#include <errno.h>
#include <sys/socket.h>

#include "gfe/gfe2/base/epoll_server.h"
#include "common/quic/core/crypto/quic_random.h"
#include "common/quic/core/quic_utils.h"
#include "common/quic/platform/api/quic_socket_address.h"
#include "common/quic/platform/impl/quic_socket_utils.h"

namespace gfe_quic {

QuicEpollConnectionHelper::QuicEpollConnectionHelper(
    gfe2::EpollServer* epoll_server,
    QuicAllocator type)
    : clock_(epoll_server),
      random_generator_(QuicRandom::GetInstance()),
      allocator_type_(type) {}

QuicEpollConnectionHelper::~QuicEpollConnectionHelper() {}

const QuicClock* QuicEpollConnectionHelper::GetClock() const {
  return &clock_;
}

QuicRandom* QuicEpollConnectionHelper::GetRandomGenerator() {
  return random_generator_;
}

QuicBufferAllocator* QuicEpollConnectionHelper::GetStreamSendBufferAllocator() {
  if (allocator_type_ == QuicAllocator::BUFFER_POOL) {
    return &stream_buffer_allocator_;
  } else {
    DCHECK(allocator_type_ == QuicAllocator::SIMPLE);
    return &simple_buffer_allocator_;
  }
}

}  // namespace gfe_quic
