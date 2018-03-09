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

#include "test/common/quic/http/tools/quic_http_stream_generator.h"

#include <algorithm>
#include <cstdint>
#include <utility>

#include "base/commandlineflags.h"
#include "base/logging.h"
#include "common/quic/http/quic_http_constants.h"
#include "test/common/quic/http/tools/quic_http_random_util.h"
#include "common/quic/platform/api/quic_bug_tracker.h"
#include "common/quic/platform/api/quic_ptr_util.h"
#include "common/quic/platform/api/quic_string_piece.h"
#include "common/quic/spdy_utils/core/spdy_framer.h"
#include "common/quic/spdy_utils/core/spdy_header_block.h"
#include "common/quic/spdy_utils/core/spdy_protocol.h"

// TODO: Extract padding decisions, etc. out into some policy
// API, or similar.
DEFINE_double(stream_generator_data_padding_frac, 0.1, "Fraction of DATA");

using ::gfe_quic::QuicMakeUnique;
using ::gfe_spdy::SpdyDataIR;
using ::gfe_spdy::SpdyFrameIR;
using ::gfe_spdy::SpdyFramer;
using ::gfe_spdy::SpdyHeaderBlock;
using ::gfe_spdy::SpdyHeadersIR;
using ::gfe_spdy::SpdyPingIR;
using ::gfe_spdy::SpdyPushPromiseIR;
using ::gfe_spdy::SpdySerializedFrame;
using ::gfe_spdy::SpdyWindowUpdateIR;

namespace gfe_quic {
namespace test {
namespace {

typedef MessageInterface::NameAndValue NameAndValue;
typedef MessageInterface::NamesAndValues NamesAndValues;

// BaseSerializer implements SerializerInterface, but in turn depends on
// sub-classes to provide it with a StreamIdGenerator, SpdyFramer (for
// serializing frames, especially HPQUIC_HTTP_ACK containing frames), and a
// random number generator.
//
// To simulate flow control, we generate window update messages in proportion
// to the amount of data to be sent (it should in proportion to the amount
// of data received, but we don't have that info). If the message is a request,
// we put the window updates after the message on the assumption the client will
// receive the response, and thus need to send window updates, after the entire
// request has been sent. And we do the opposite if serializing a response.
//
// To collect RTT stats, GFE sends a PING to the client at the end of every
// response. We simulate that by serializing a PING after the end of a
// serialized response, and a PING QUIC_HTTP_ACK after the end of a serialized
// request.
class BaseSerializer : public SerializerInterface {
 public:
  BaseSerializer() {}
  ~BaseSerializer() override {}

  // Functions to be implemented by sub-classes, providing classes that support
  // serialization:

  virtual StreamIdGenerator* id_generator() = 0;
  virtual SpdyFramer* framer() = 0;
  virtual QuicTestRandomBase* rng() = 0;

  // Functions implementing SerializerInterface:

  void SerializeMessage(const MessageInterface& message,
                        size_t num_window_updates) override {
    MessageVisitor mv(this, id_generator()->NextClientId(), num_window_updates);
    message.Visit(&mv);
  }
  const std::vector<QuicString>& serialized_frames() const override {
    return serialized_frames_;
  }
  void clear_serialized_frames() override { serialized_frames_.clear(); }

 protected:
  // Serialize a request message into frames, plus simulated flow control and
  // an RTT reporting PING QUIC_HTTP_ACK afterwards.
  void SerializeRequestMessage(const RequestMessageInterface& message,
                               uint32_t client_id,
                               size_t num_window_updates) {
    if (!SerializeMessageHeaders(client_id, message, true)) {
      SerializeMessageBodyAndTrailers(client_id, message);
    }
    GenerateWindowUpdates(client_id, data_frame_limit_, num_window_updates);
    // To collect RTT stats, GFE sends a PING to the client at the end of every
    // response. Simulate that activity by responding to that expected PING
    // with an QUIC_HTTP_ACK.
    GeneratePingAck();
  }

  // Serialized a response message into frames, preceded by simulated flow
  // control and followed by an RTT gathering PING.
  void SerializeResponseMessage(const ResponseMessageInterface& message,
                                uint32_t client_id,
                                size_t num_window_updates) {
    GenerateWindowUpdates(client_id, data_frame_limit_, num_window_updates);
    if (message.pushed_messages().empty()) {
      if (!SerializeMessageHeaders(client_id, message, true)) {
        SerializeMessageBodyAndTrailers(client_id, message);
      }
    } else {
      SerializeMessageHeaders(client_id, message, false);
      for (const auto& child : message.pushed_messages()) {
        uint32_t promised_stream_id = id_generator()->NextServerId();
        SerializePushedMessage(client_id, promised_stream_id, *child);
      }
      SerializeMessageBodyAndTrailers(client_id, message);
    }
    // To collect RTT stats, GFE sends a PING to the client at the end of every
    // response. Simulate that.
    GeneratePing();
  }

 private:
  class MessageVisitor : public MessageVisitorInterface {
   public:
    MessageVisitor(BaseSerializer* serializer,
                   uint32_t client_id,
                   size_t num_window_updates)
        : serializer_(serializer),
          client_id_(client_id),
          num_window_updates_(num_window_updates) {}

    void VisitRequestMessage(const RequestMessageInterface& message) override {
      serializer_->SerializeRequestMessage(message, client_id_,
                                           num_window_updates_);
    }

    void VisitResponseMessage(
        const ResponseMessageInterface& message) override {
      serializer_->SerializeResponseMessage(message, client_id_,
                                            num_window_updates_);
    }

    void VisitPushedMessage(const PushedMessageInterface& message) override {
      QUIC_BUG << "Shouldn't be serializing a pushed message that isn't "
                  "part of a response";
    }

   private:
    BaseSerializer* serializer_;
    uint32_t client_id_;
    size_t num_window_updates_;
  };

  // Serialize the leading HEADERS of a message using the supplied stream id.
  // may_end_stream is true if this function is allowed to emit a HEADERS frame
  // with the QUIC_HTTP_END_STREAM flag; it is false for response messages with
  // nested pushed messages in order to allow for serializing all of the nested
  // pushed messages before sending an QUIC_HTTP_END_STREAM on the parent
  // message.
  bool SerializeMessageHeaders(uint32_t stream_id,
                               const MessageInterface& message,
                               bool may_end_stream) {
    bool end_stream =
        may_end_stream && message.body().empty() && message.trailers().empty();
    SerializeHeaders(stream_id, message.headers(), end_stream);
    return end_stream;
  }

  // Serialize the body and trailing headers of a message, using the specified
  // stream id, and setting the QUIC_HTTP_END_STREAM flag on the last frame of
  // the stream (either the last DATA frame, or the last frame containing the
  // trailers).
  void SerializeMessageBodyAndTrailers(uint32_t stream_id,
                                       const MessageInterface& message) {
    bool end_stream = message.trailers().empty();
    SerializeData(stream_id, message.body(), end_stream);
    if (!end_stream) {
      SerializeHeaders(stream_id, message.trailers(), true /* end_stream */);
    }
  }

  // Serialize the provided headers to the specified stream, setting the
  // QUIC_HTTP_END_STREAM flag if end_stream is true.
  void SerializeHeaders(uint32_t stream_id,
                        const NamesAndValues& headers,
                        bool end_stream) {
    SpdyHeaderBlock header_block;
    for (const NameAndValue& nv : headers) {
      header_block.AppendValueOrAddHeader(nv.first, nv.second);
    }
    // Note that no padding is added. We could gather stats on real traffic to
    // decide whether to add any padding to HEADERS frames.
    SpdyHeadersIR ir(stream_id);
    ir.set_header_block(std::move(header_block));
    ir.set_fin(end_stream);
    SerializeFrame(ir);
  }

  // Serialize a pushed message, first sending the PUSH_PROMISE with the request
  // headers, and then the response message.
  void SerializePushedMessage(uint32_t parent_stream_id,
                              uint32_t promised_stream_id,
                              const PushedMessageInterface& message) {
    SpdyHeaderBlock header_block;
    for (const NameAndValue& nv : message.pushed_request_headers()) {
      header_block.insert(std::make_pair(nv.first, nv.second));
    }
    // Note that no padding is added. We could gather stats on real traffic to
    // decide whether to add any padding to PUSH_PROMISE frames.
    gfe_spdy::SpdyPushPromiseIR ir(parent_stream_id, promised_stream_id);
    ir.set_header_block(std::move(header_block));
    SerializeFrame(ir);
    if (!SerializeMessageHeaders(promised_stream_id, message, true)) {
      SerializeMessageBodyAndTrailers(promised_stream_id, message);
    }
  }

  // Serialize a DATA frame. Currently this chooses to add some padding
  // sometimes, based on seeing that GFE does some in some circumstances
  // (search for EnableCrimeMitigation). But this should be pulled out into
  // some kind of configurable option, and be based on a production traffic
  // model.
  // TODO: Extract padding decisions, etc. out into some policy
  // API, or similar.
  void SerializeOneDataFrame(uint32_t stream_id,
                             QuicStringPiece data,
                             bool end_stream) {
    CHECK_LE(data.size(), data_frame_limit_);
    SpdyDataIR ir(stream_id);
    ir.SetDataShallow(data);
    ir.set_fin(end_stream);
    // We try to pad one in 10 frames, an arbitrary choice.
    int padding_len = 0;
    if (data.size() < data_frame_limit_ &&
        rng()->RandFloat() < FLAGS_stream_generator_data_padding_frac) {
      int limit = data_frame_limit_ - data.size();
      limit = std::min(256, limit);
      padding_len = rng()->Uniform(limit) + 1;
      ir.set_padding_len(padding_len);
    }
    SerializeFrame(ir);
  }

  // Serializes data into DATA frames whose payload is no larger than
  // data_frame_limit_. Sets the QUIC_HTTP_END_STREAM flag on the last frame if
  // the end_stream param is true.
  void SerializeData(uint32_t stream_id,
                     QuicStringPiece data,
                     bool end_stream) {
    while (data.size() > data_frame_limit_) {
      QuicStringPiece prefix = data.substr(0, data_frame_limit_);
      SerializeOneDataFrame(stream_id, prefix, false /* end_stream */);
      data.remove_prefix(data_frame_limit_);
    }
    if (!data.empty() || end_stream) {
      SerializeOneDataFrame(stream_id, data, end_stream);
    }
  }

  void GeneratePing() {
    SpdyPingIR ir(rng()->Rand64());
    ir.set_is_ack(false);
    SerializeFrame(ir);
  }

  void GeneratePingAck() {
    SpdyPingIR ir(rng()->Rand64());
    ir.set_is_ack(true);
    SerializeFrame(ir);
  }

  void GenerateWindowUpdates(uint32_t stream_id,
                             uint32_t increment,
                             size_t num) {
    while (num-- > 0) {
      GenerateWindowUpdate(stream_id, increment);
      GenerateWindowUpdate(0, increment);
    }
  }
  void GenerateWindowUpdate(uint32_t stream_id, uint32_t increment) {
    SpdyWindowUpdateIR ir(stream_id, increment);
    SerializeFrame(ir);
  }

  // Serialize a SpdyFrameIR and append to the vector of such serialized frames.
  void SerializeFrame(const SpdyFrameIR& frame) {
    SpdySerializedFrame sf(framer()->SerializeFrame(frame));
    serialized_frames_.emplace_back(sf.data(), sf.size());
  }

  // The frames that have been serialized.
  std::vector<QuicString> serialized_frames_;

  // Maximum payload size for DATA frames.
  const size_t data_frame_limit_ = QuicHttpSettingsInfo::DefaultMaxFrameSize();
};

// Serializer requires caller to provide the stream id generator, framer
// and random number generator, and makes them available to the base.
class Serializer : public BaseSerializer {
 public:
  Serializer(StreamIdGenerator* id_generator,
             SpdyFramer* framer,
             QuicTestRandomBase* rng)
      : id_generator_(id_generator), framer_(framer), rng_(rng) {}
  ~Serializer() override {}

  StreamIdGenerator* id_generator() override { return id_generator_; }
  SpdyFramer* framer() override { return framer_; }
  QuicTestRandomBase* rng() override { return rng_; }

 private:
  StreamIdGenerator* id_generator_;
  SpdyFramer* framer_;
  QuicTestRandomBase* rng_;
};

// FramerAndSerializer requires a random number generator, and creates its own
// stream id and random number generators.
class FramerAndSerializer : public BaseSerializer {
 public:
  explicit FramerAndSerializer(QuicTestRandomBase* rng)
      : rng_(rng),
        id_generator_(rng),
        framer_(gfe_spdy::SpdyFramer::ENABLE_COMPRESSION) {}
  ~FramerAndSerializer() override {}

  StreamIdGenerator* id_generator() override { return &id_generator_; }
  SpdyFramer* framer() override { return &framer_; }
  QuicTestRandomBase* rng() override { return rng_; }

 private:
  QuicTestRandomBase* rng_;
  StreamIdGenerator id_generator_;
  SpdyFramer framer_;
};
}  // namespace

std::unique_ptr<SerializerInterface> CreateSerializer(
    StreamIdGenerator* id_generator,
    ::gfe_spdy::SpdyFramer* framer,
    QuicTestRandomBase* rng) {
  return QuicMakeUnique<Serializer>(id_generator, framer, rng);
}

std::unique_ptr<SerializerInterface> CreateSerializer(QuicTestRandomBase* rng) {
  return QuicMakeUnique<FramerAndSerializer>(rng);
}

std::vector<QuicString> SerializeMessage(const MessageInterface& message,
                                         size_t num_window_updates,
                                         StreamIdGenerator* id_generator,
                                         ::gfe_spdy::SpdyFramer* framer,
                                         QuicTestRandomBase* rng) {
  Serializer serializer(id_generator, framer, rng);
  serializer.SerializeMessage(message, num_window_updates);
  return serializer.serialized_frames();
}

std::vector<QuicString> SerializeMessages(
    const std::vector<std::unique_ptr<MessageInterface>> messages,
    size_t total_num_window_updates,
    StreamIdGenerator* id_generator,
    ::gfe_spdy::SpdyFramer* framer,
    QuicTestRandomBase* rng) {
  // Serialize each message into a sequence of frames (strings containing their
  // encoded bytes), and then push each sequence onto the vector sequences.
  Serializer serializer(id_generator, framer, rng);
  std::vector<std::vector<QuicString>> sequences;
  sequences.reserve(messages.size());
  size_t total_frames = 0;
  size_t remaining_messages = messages.size();
  size_t remaining_window_updates = total_num_window_updates;
  for (const auto& mptr : messages) {
    size_t num_window_updates = remaining_window_updates / remaining_messages;
    remaining_window_updates -= num_window_updates;
    --remaining_messages;
    serializer.SerializeMessage(*mptr, num_window_updates);
    sequences.emplace_back(serializer.serialized_frames());
    serializer.clear_serialized_frames();
    total_frames += sequences.back().size();
  }
  // Reverse the sequences so that we can pop from the back.
  for (auto& frames : sequences) {
    reverse(frames.begin(), frames.end());
  }
  // Interleave the sequences, preferring the earlier messages when selecting
  // the next frame. A convenient side-effect of the way SpdyFramer serializes
  // HEADERS is that even if there are CONTINUATION frames they're all output as
  // a single sequence of bytes, so we won't accidently split a CONTINUATION
  // frame from its required predecessor.
  // TODO: This broken w.r.t. PUSH_PROMISE messages, as the random
  // frame selection will not attempt to preserve the creation order of
  // server-initiated streams.
  std::vector<QuicString> result;
  result.reserve(total_frames);
  size_t highest_allowed = 0;
  while (!sequences.empty()) {
    // Select a sequence of frames from which to select the next frame.
    size_t ndx = GenerateRandomSizeSkewedLow(sequences.size(), rng);

    // Ensure that we can't skip over client stream ids by only allowing
    // sequences[N+1] to be used once sequences[N] has been used at least once.
    ndx = std::min(ndx, highest_allowed);
    highest_allowed = std::max(ndx + 1, highest_allowed);
    DCHECK_LT(0, highest_allowed);

    // If we drained the sequence, remove it from consideration.
    if (sequences[ndx].empty()) {
      sequences.erase(sequences.begin() + ndx);
      --highest_allowed;
      continue;
    }
    result.push_back(sequences[ndx].back());
    sequences[ndx].pop_back();
  }

  return result;
}

}  // namespace test
}  // namespace gfe_quic
