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

#ifndef GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_STREAM_GENERATOR_H_
#define GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_STREAM_GENERATOR_H_

// Serializes (produces HTTP/2 frames) a sequence of HTTP/2 messages.
//
// The purpose is to produce a stream of bytes that can be decoded by an
// HTTP/2 decoder or endpoint for the purpose of assessing performance of that
// process, but where the requests and reponses don't mean anything, and aren't
// actually processed beyond the level described in the HTTP/2 RFC (i.e. the
// endpoint might check that stream ids are valid and might build representation
// of header blocks, but not actually process the request). Flow control frames
// (WINDOW_UPDATES) are included in the output stream, but there is no way to
// respond to flow control messages that the endpoint being measured might
// normally choose to emit.

#include <stddef.h>

#include <memory>
#include <vector>

#include "test/common/quic/http/tools/quic_http_message_interface.h"
#include "test/common/quic/http/tools/quic_http_stream_id_generator.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_test_random.h"
#include "common/quic/spdy_utils/core/spdy_framer.h"

namespace gfe_quic {
namespace test {

class SerializerInterface {
 public:
  virtual ~SerializerInterface() {}

  // Serializes a message into HTTP/2 frames. num_window_updates is the number
  // of window updates to also serialize, simulating flow-control messages
  // sent to a peer.
  virtual void SerializeMessage(const MessageInterface& message,
                                size_t num_window_updates) = 0;

  // Get the HTTP/2 frames that have been produced by SerializeMessage.
  virtual const std::vector<QuicString>& serialized_frames() const = 0;

  // Discard the HTTP/2 frames; does not reset other state, such as the sequence
  // of stream ids.
  virtual void clear_serialized_frames() = 0;
};

// Create a serializer of messages, using the provided class instances.
std::unique_ptr<SerializerInterface> CreateSerializer(
    StreamIdGenerator* id_generator,
    ::gfe_spdy::SpdyFramer* framer,
    QuicTestRandomBase* rng);

// Create a serializer of messages, creating its own stream id and random
// number generators.
std::unique_ptr<SerializerInterface> CreateSerializer(QuicTestRandomBase* rng);

// Serialize a single message using the provided class instances.
std::vector<QuicString> SerializeMessage(const MessageInterface& message,
                                         size_t num_window_updates,
                                         StreamIdGenerator* id_generator,
                                         ::gfe_spdy::SpdyFramer* framer,
                                         QuicTestRandomBase* rng);

// Serialize and (legally) interleave the frames of the messages (i.e. stream id
// N will not be used before stream id M is first used, for all M < N).
//
std::vector<QuicString> SerializeMessages(
    const std::vector<std::unique_ptr<MessageInterface>> messages,
    size_t total_num_window_updates,
    StreamIdGenerator* id_generator,
    ::gfe_spdy::SpdyFramer* framer,
    QuicTestRandomBase* rng);

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_STREAM_GENERATOR_H_
