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

// Copyright 2009 Google Inc. All Rights Reserved.
// Author: ygi@google.com (Yossi Gilad)

#ifndef GFE_SPDY_CORE_MOCK_SPDY_FRAMER_VISITOR_H_
#define GFE_SPDY_CORE_MOCK_SPDY_FRAMER_VISITOR_H_

#include <cstdint>
#include <memory>

#include "common/quic/spdy_utils/core/http2_frame_decoder_adapter.h"
#include "test/common/quic/spdy_utils/core/spdy_test_utils.h"
#include "common/quic/spdy_utils/platform/api/spdy_ptr_util.h"
#include "common/quic/spdy_utils/platform/api/spdy_string_piece.h"
#include "gmock/gmock.h"

namespace gfe_spdy {

namespace test {

class MockSpdyFramerVisitor : public SpdyFramerVisitorInterface {
 public:
  MockSpdyFramerVisitor();
  ~MockSpdyFramerVisitor() override;

  MOCK_METHOD1(OnError,
               void(gfe_http2::Http2DecoderAdapter::SpdyFramerError error));
  MOCK_METHOD3(OnDataFrameHeader,
               void(SpdyStreamId stream_id, size_t length, bool fin));
  MOCK_METHOD3(OnStreamFrameData,
               void(SpdyStreamId stream_id, const char* data, size_t len));
  MOCK_METHOD1(OnStreamEnd, void(SpdyStreamId stream_id));
  MOCK_METHOD2(OnStreamPadLength, void(SpdyStreamId stream_id, size_t value));
  MOCK_METHOD2(OnStreamPadding, void(SpdyStreamId stream_id, size_t len));
  MOCK_METHOD1(OnHeaderFrameStart,
               SpdyHeadersHandlerInterface*(SpdyStreamId stream_id));
  MOCK_METHOD1(OnHeaderFrameEnd, void(SpdyStreamId stream_id));
  MOCK_METHOD2(OnRstStream,
               void(SpdyStreamId stream_id, SpdyErrorCode error_code));
  MOCK_METHOD0(OnSettings, void());
  MOCK_METHOD2(OnSettingOld, void(SpdyKnownSettingsId id, uint32_t value));
  MOCK_METHOD2(OnSetting, void(SpdySettingsId id, uint32_t value));
  MOCK_METHOD2(OnPing, void(SpdyPingId unique_id, bool is_ack));
  MOCK_METHOD0(OnSettingsEnd, void());
  MOCK_METHOD2(OnGoAway,
               void(SpdyStreamId last_accepted_stream_id,
                    SpdyErrorCode error_code));
  MOCK_METHOD7(OnHeaders,
               void(SpdyStreamId stream_id,
                    bool has_priority,
                    int weight,
                    SpdyStreamId parent_stream_id,
                    bool exclusive,
                    bool fin,
                    bool end));
  MOCK_METHOD2(OnWindowUpdate,
               void(SpdyStreamId stream_id, int delta_window_size));
  MOCK_METHOD3(OnPushPromise,
               void(SpdyStreamId stream_id,
                    SpdyStreamId promised_stream_id,
                    bool end));
  MOCK_METHOD2(OnContinuation, void(SpdyStreamId stream_id, bool end));
  MOCK_METHOD3(OnAltSvc,
               void(SpdyStreamId stream_id,
                    SpdyStringPiece origin,
                    const SpdyAltSvcWireFormat::AlternativeServiceVector&
                        altsvc_vector));
  MOCK_METHOD4(OnPriority,
               void(SpdyStreamId stream_id,
                    SpdyStreamId parent_stream_id,
                    int weight,
                    bool exclusive));
  MOCK_METHOD2(OnUnknownFrame,
               bool(SpdyStreamId stream_id, uint8_t frame_type));

  void DelegateHeaderHandling() {
    ON_CALL(*this, OnHeaderFrameStart(testing::_))
        .WillByDefault(testing::Invoke(
            this, &MockSpdyFramerVisitor::ReturnTestHeadersHandler));
    ON_CALL(*this, OnHeaderFrameEnd(testing::_))
        .WillByDefault(testing::Invoke(
            this, &MockSpdyFramerVisitor::ResetTestHeadersHandler));
  }

  SpdyHeadersHandlerInterface* ReturnTestHeadersHandler(
      SpdyStreamId /* stream_id */) {
    if (headers_handler_ == nullptr) {
      headers_handler_ = SpdyMakeUnique<TestHeadersHandler>();
    }
    return headers_handler_.get();
  }

  void ResetTestHeadersHandler(SpdyStreamId /* stream_id */) {
    headers_handler_.reset();
  }

  std::unique_ptr<SpdyHeadersHandlerInterface> headers_handler_;
};

}  // namespace test

}  // namespace gfe_spdy

#endif  // GFE_SPDY_CORE_MOCK_SPDY_FRAMER_VISITOR_H_
