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

#ifndef GFE_QUIC_HTTP_TEST_TOOLS_QUIC_HTTP_FRAME_PARTS_COLLECTOR_LISTENER_H_
#define GFE_QUIC_HTTP_TEST_TOOLS_QUIC_HTTP_FRAME_PARTS_COLLECTOR_LISTENER_H_

// QuicHttpFramePartsCollectorListener extends QuicHttpFramePartsCollector with
// an implementation of every method of QuicHttpFrameDecoderListener; it is
// essentially the union of all the Listener classes in the tests of the
// payload decoders (i.e. in ./payload_decoders/*_test.cc files), with the
// addition of the OnFrameHeader method.
// QuicHttpFramePartsCollectorListener supports tests of QuicHttpFrameDecoder.

#include <stddef.h>

#include <cstdint>

#include "common/quic/http/quic_http_constants.h"
#include "common/quic/http/quic_http_structures.h"
#include "test/common/quic/http/test_tools/quic_http_frame_parts_collector.h"

namespace gfe_quic {
namespace test {

class QuicHttpFramePartsCollectorListener : public QuicHttpFramePartsCollector {
 public:
  QuicHttpFramePartsCollectorListener() {}
  ~QuicHttpFramePartsCollectorListener() override {}

  // TODO: Remove OnFrameHeader once done with supporting
  // SpdyFramer's exact states.
  bool OnFrameHeader(const QuicHttpFrameHeader& header) override;
  void OnDataStart(const QuicHttpFrameHeader& header) override;
  void OnDataPayload(const char* data, size_t len) override;
  void OnDataEnd() override;
  void OnHeadersStart(const QuicHttpFrameHeader& header) override;
  void OnHeadersPriority(const QuicHttpPriorityFields& priority) override;
  void OnHpackFragment(const char* data, size_t len) override;
  void OnHeadersEnd() override;
  void OnPriorityFrame(const QuicHttpFrameHeader& header,
                       const QuicHttpPriorityFields& priority_fields) override;
  void OnContinuationStart(const QuicHttpFrameHeader& header) override;
  void OnContinuationEnd() override;
  void OnPadLength(size_t pad_length) override;
  void OnPadding(const char* padding, size_t skipped_length) override;
  void OnRstStream(const QuicHttpFrameHeader& header,
                   QuicHttpErrorCode error_code) override;
  void OnSettingsStart(const QuicHttpFrameHeader& header) override;
  void OnSetting(const QuicHttpSettingFields& setting_fields) override;
  void OnSettingsEnd() override;
  void OnSettingsAck(const QuicHttpFrameHeader& header) override;
  void OnPushPromiseStart(const QuicHttpFrameHeader& header,
                          const QuicHttpPushPromiseFields& promise,
                          size_t total_padding_length) override;
  void OnPushPromiseEnd() override;
  void OnPing(const QuicHttpFrameHeader& header,
              const QuicHttpPingFields& ping) override;
  void OnPingAck(const QuicHttpFrameHeader& header,
                 const QuicHttpPingFields& ping) override;
  void OnGoAwayStart(const QuicHttpFrameHeader& header,
                     const QuicHttpGoAwayFields& goaway) override;
  void OnGoAwayOpaqueData(const char* data, size_t len) override;
  void OnGoAwayEnd() override;
  void OnWindowUpdate(const QuicHttpFrameHeader& header,
                      uint32_t window_size_increment) override;
  void OnAltSvcStart(const QuicHttpFrameHeader& header,
                     size_t origin_length,
                     size_t value_length) override;
  void OnAltSvcOriginData(const char* data, size_t len) override;
  void OnAltSvcValueData(const char* data, size_t len) override;
  void OnAltSvcEnd() override;
  void OnUnknownStart(const QuicHttpFrameHeader& header) override;
  void OnUnknownPayload(const char* data, size_t len) override;
  void OnUnknownEnd() override;
  void OnPaddingTooLong(const QuicHttpFrameHeader& header,
                        size_t missing_length) override;
  void OnFrameSizeError(const QuicHttpFrameHeader& header) override;
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_TEST_TOOLS_QUIC_HTTP_FRAME_PARTS_COLLECTOR_LISTENER_H_
