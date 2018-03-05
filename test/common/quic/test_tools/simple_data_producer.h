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

#ifndef GFE_QUIC_TEST_TOOLS_SIMPLE_DATA_PRODUCER_H_
#define GFE_QUIC_TEST_TOOLS_SIMPLE_DATA_PRODUCER_H_

#include "common/quic/core/quic_simple_buffer_allocator.h"
#include "common/quic/core/quic_stream_frame_data_producer.h"
#include "common/quic/core/quic_stream_send_buffer.h"
#include "common/quic/platform/api/quic_containers.h"

namespace gfe_quic {

namespace test {

// A simple data producer which copies stream data into a map from stream
// id to send buffer.
class SimpleDataProducer : public QuicStreamFrameDataProducer {
 public:
  SimpleDataProducer();
  ~SimpleDataProducer() override;

  void SaveStreamData(QuicStreamId id,
                      const struct iovec* iov,
                      int iov_count,
                      size_t iov_offset,
                      QuicStreamOffset offset,
                      QuicByteCount data_length);

  // QuicStreamFrameDataProducer
  bool WriteStreamData(QuicStreamId id,
                       QuicStreamOffset offset,
                       QuicByteCount data_length,
                       QuicDataWriter* writer) override;

 private:
  using SendBufferMap =
      QuicUnorderedMap<QuicStreamId, std::unique_ptr<QuicStreamSendBuffer>>;

  SimpleBufferAllocator allocator_;

  SendBufferMap send_buffer_map_;
};

}  // namespace test

}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_SIMPLE_DATA_PRODUCER_H_
