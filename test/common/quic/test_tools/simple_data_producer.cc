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

#include "test/common/quic/test_tools/simple_data_producer.h"

#include "common/quic/platform/api/quic_flags.h"
#include "common/quic/platform/api/quic_map_util.h"
#include "common/quic/platform/api/quic_ptr_util.h"

namespace gfe_quic {

namespace test {

SimpleDataProducer::SimpleDataProducer() {}

SimpleDataProducer::~SimpleDataProducer() {}

void SimpleDataProducer::SaveStreamData(QuicStreamId id,
                                        const struct iovec* iov,
                                        int iov_count,
                                        size_t iov_offset,
                                        QuicStreamOffset offset,
                                        QuicByteCount data_length) {
  if (data_length == 0) {
    return;
  }
  if (!QuicContainsKey(send_buffer_map_, id)) {
    send_buffer_map_[id] = QuicMakeUnique<QuicStreamSendBuffer>(&allocator_);
  }
  send_buffer_map_[id]->SaveStreamData(iov, iov_count, iov_offset, data_length);
}

bool SimpleDataProducer::WriteStreamData(QuicStreamId id,
                                         QuicStreamOffset offset,
                                         QuicByteCount data_length,
                                         QuicDataWriter* writer) {
  return send_buffer_map_[id]->WriteStreamData(offset, data_length, writer);
}

}  // namespace test

}  // namespace gfe_quic
