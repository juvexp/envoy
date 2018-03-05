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

// SpdyNoOpVisitor implements several of the visitor and handler interfaces
// to make it easier to write tests that need to provide instances. Other
// interfaces can be added as needed.

#ifndef GFE_SPDY_CORE_SPDY_NO_OP_VISITOR_H_
#define GFE_SPDY_CORE_SPDY_NO_OP_VISITOR_H_

#include <cstdint>

#include "common/quic/spdy_utils/core/http2_frame_decoder_adapter.h"
#include "common/quic/spdy_utils/core/spdy_protocol.h"
#include "common/quic/spdy_utils/platform/api/spdy_string_piece.h"

namespace gfe_spdy {
namespace test {

class SpdyNoOpVisitor : public SpdyFramerVisitorInterface,
                        public SpdyFramerDebugVisitorInterface,
                        public SpdyHeadersHandlerInterface {
 public:
  SpdyNoOpVisitor();
  ~SpdyNoOpVisitor() override;

  // SpdyFramerVisitorInterface methods:
  void OnError(gfe_http2::Http2DecoderAdapter::SpdyFramerError error) override {
  }
  SpdyHeadersHandlerInterface* OnHeaderFrameStart(
      SpdyStreamId stream_id) override;
  void OnHeaderFrameEnd(SpdyStreamId stream_id) override {}
  void OnDataFrameHeader(SpdyStreamId stream_id,
                         size_t length,
                         bool fin) override {}
  void OnStreamFrameData(SpdyStreamId stream_id,
                         const char* data,
                         size_t len) override {}
  void OnStreamEnd(SpdyStreamId stream_id) override {}
  void OnStreamPadding(SpdyStreamId stream_id, size_t len) override {}
  void OnRstStream(SpdyStreamId stream_id, SpdyErrorCode error_code) override {}
  void OnSettingOld(SpdyKnownSettingsId id, uint32_t value) override {}
  void OnSetting(SpdySettingsId id, uint32_t value) override {}
  void OnPing(SpdyPingId unique_id, bool is_ack) override {}
  void OnSettingsEnd() override {}
  void OnSettingsAck() override {}
  void OnGoAway(SpdyStreamId last_accepted_stream_id,
                SpdyErrorCode error_code) override {}
  void OnHeaders(SpdyStreamId stream_id,
                 bool has_priority,
                 int weight,
                 SpdyStreamId parent_stream_id,
                 bool exclusive,
                 bool fin,
                 bool end) override {}
  void OnWindowUpdate(SpdyStreamId stream_id, int delta_window_size) override {}
  void OnPushPromise(SpdyStreamId stream_id,
                     SpdyStreamId promised_stream_id,
                     bool end) override {}
  void OnContinuation(SpdyStreamId stream_id, bool end) override {}
  void OnAltSvc(SpdyStreamId stream_id,
                SpdyStringPiece origin,
                const SpdyAltSvcWireFormat::AlternativeServiceVector&
                    altsvc_vector) override {}
  void OnPriority(SpdyStreamId stream_id,
                  SpdyStreamId parent_stream_id,
                  int weight,
                  bool exclusive) override {}
  bool OnUnknownFrame(SpdyStreamId stream_id, uint8_t frame_type) override;

  // SpdyFramerDebugVisitorInterface methods:
  void OnSendCompressedFrame(SpdyStreamId stream_id,
                             SpdyFrameType type,
                             size_t payload_len,
                             size_t frame_len) override {}
  void OnReceiveCompressedFrame(SpdyStreamId stream_id,
                                SpdyFrameType type,
                                size_t frame_len) override {}

  // SpdyHeadersHandlerInterface methods:
  void OnHeaderBlockStart() override {}
  void OnHeader(SpdyStringPiece key, SpdyStringPiece value) override {}
  void OnHeaderBlockEnd(size_t /* uncompressed_header_bytes */,
                        size_t /* compressed_header_bytes */) override {}
};

}  // namespace test
}  // namespace gfe_spdy

#endif  // GFE_SPDY_CORE_SPDY_NO_OP_VISITOR_H_
