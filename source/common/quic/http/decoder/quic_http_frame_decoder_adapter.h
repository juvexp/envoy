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

#ifndef GFE_QUIC_HTTP_DECODER_QUIC_HTTP_FRAME_DECODER_ADAPTER_H_
#define GFE_QUIC_HTTP_DECODER_QUIC_HTTP_FRAME_DECODER_ADAPTER_H_

#include <stddef.h>

#include <cstdint>
#include <memory>

#include "common/quic/http/decoder/quic_http_frame_decoder.h"
#include "common/quic/platform/api/quic_optional.h"
#include "common/quic/platform/api/quic_string_piece.h"
#include "common/quic/spdy_utils/core/hpack/hpack_decoder_adapter.h"
#include "common/quic/spdy_utils/core/hpack/hpack_header_table.h"
#include "common/quic/spdy_utils/core/http2_frame_decoder_adapter.h"
#include "common/quic/spdy_utils/core/spdy_alt_svc_wire_format.h"
#include "common/quic/spdy_utils/core/spdy_framer.h"
#include "common/quic/spdy_utils/core/spdy_headers_handler_interface.h"
#include "common/quic/spdy_utils/core/spdy_protocol.h"

namespace gfe_spdy {

class SpdyFramerVisitorInterface;
class ExtensionVisitorInterface;

}  // namespace gfe_spdy

namespace gfe_quic {

// Adapts SpdyFramer interface to use QuicHttpFrameDecoder.
class SPDY_EXPORT_PRIVATE QuicHttpDecoderAdapter
    : public gfe_quic::QuicHttpFrameDecoderListener {
 public:
  // HTTP2 states.
  enum SpdyState {
    SPDY_ERROR,
    SPDY_READY_FOR_FRAME,  // Framer is ready for reading the next frame.
    SPDY_FRAME_COMPLETE,  // Framer has finished reading a frame, need to reset.
    SPDY_READING_COMMON_HEADER,
    SPDY_CONTROL_FRAME_PAYLOAD,
    SPDY_READ_DATA_FRAME_PADDING_LENGTH,
    SPDY_CONSUME_PADDING,
    SPDY_IGNORE_REMAINING_PAYLOAD,
    SPDY_FORWARD_STREAM_FRAME,
    SPDY_CONTROL_FRAME_BEFORE_HEADER_BLOCK,
    SPDY_CONTROL_FRAME_HEADER_BLOCK,
    SPDY_GOAWAY_FRAME_PAYLOAD,
    SPDY_SETTINGS_FRAME_HEADER,
    SPDY_SETTINGS_FRAME_PAYLOAD,
    SPDY_ALTSVC_FRAME_PAYLOAD,
    SPDY_EXTENSION_FRAME_PAYLOAD,
  };

  // For debugging.
  static const char* StateToString(int state);

  QuicHttpDecoderAdapter();
  ~QuicHttpDecoderAdapter() override;

  // Set callbacks to be called from the framer.  A visitor must be set, or
  // else the framer will likely crash.  It is acceptable for the visitor
  // to do nothing.  If this is called multiple times, only the last visitor
  // will be used.
  void set_visitor(gfe_spdy::SpdyFramerVisitorInterface* visitor);
  gfe_spdy::SpdyFramerVisitorInterface* visitor() const { return visitor_; }

  // Set extension callbacks to be called from the framer or decoder. Optional.
  // If called multiple times, only the last visitor will be used.
  void set_extension_visitor(gfe_spdy::ExtensionVisitorInterface* visitor);

  // Set debug callbacks to be called from the framer. The debug visitor is
  // completely optional and need not be set in order for normal operation.
  // If this is called multiple times, only the last visitor will be used.
  void set_debug_visitor(
      gfe_spdy::SpdyFramerDebugVisitorInterface* debug_visitor);
  gfe_spdy::SpdyFramerDebugVisitorInterface* debug_visitor() const {
    return debug_visitor_;
  }

  // Set debug callbacks to be called from the HPQUIC_HTTP_ACK decoder.
  void SetDecoderHeaderTableDebugVisitor(
      std::unique_ptr<gfe_spdy::HpackHeaderTable::DebugVisitorInterface>
          visitor);

  // Sets whether or not ProcessInput returns after finishing a frame, or
  // continues processing additional frames. Normally ProcessInput processes
  // all input, but this method enables the caller (and visitor) to work with
  // a single frame at a time (or that portion of the frame which is provided
  // as input). Reset() does not change the value of this flag.
  void set_process_single_input_frame(bool v);
  bool process_single_input_frame() const {
    return process_single_input_frame_;
  }

  // Decode the |len| bytes of encoded HTTP/2 starting at |*data|. Returns
  // the number of bytes consumed. It is safe to pass more bytes in than
  // may be consumed. Should process (or otherwise buffer) as much as
  // available, unless process_single_input_frame is true.
  size_t ProcessInput(const char* data, size_t len);

  // Reset the decoder (used just for tests at this time).
  void Reset();

  // Current state of the decoder.
  SpdyState state() const;

  // Current error code (NO_ERROR if state != ERROR).
  gfe_http2::Http2DecoderAdapter::SpdyFramerError spdy_framer_error() const;

  // Has any frame header looked like the start of an HTTP/1.1 (or earlier)
  // response? Used to detect if a backend/server that we sent a request to
  // has responded with an HTTP/1.1 (or earlier) response.
  bool probable_http_response() const;

  gfe_spdy::HpackDecoderAdapter* GetHpackDecoder();

  bool HasError() const;

 private:
  bool OnFrameHeader(const QuicHttpFrameHeader& header) override;
  void OnDataStart(const QuicHttpFrameHeader& header) override;
  void OnDataPayload(const char* data, size_t len) override;
  void OnDataEnd() override;
  void OnHeadersStart(const QuicHttpFrameHeader& header) override;
  void OnHeadersPriority(const QuicHttpPriorityFields& priority) override;
  void OnHpackFragment(const char* data, size_t len) override;
  void OnHeadersEnd() override;
  void OnPriorityFrame(const QuicHttpFrameHeader& header,
                       const QuicHttpPriorityFields& priority) override;
  void OnContinuationStart(const QuicHttpFrameHeader& header) override;
  void OnContinuationEnd() override;
  void OnPadLength(size_t trailing_length) override;
  void OnPadding(const char* padding, size_t skipped_length) override;
  void OnRstStream(const QuicHttpFrameHeader& header,
                   QuicHttpErrorCode http2_error_code) override;
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
                      uint32_t increment) override;
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

  size_t ProcessInputFrame(const char* data, size_t len);

  void DetermineSpdyState(QuicHttpDecodeStatus status);
  void ResetBetweenFrames();

  // ResetInternal is called from the constructor, and during tests, but not
  // otherwise (i.e. not between every frame).
  void ResetInternal();

  void set_spdy_state(SpdyState v);

  void SetSpdyErrorAndNotify(
      gfe_http2::Http2DecoderAdapter::SpdyFramerError error);

  const QuicHttpFrameHeader& frame_header() const;

  uint32_t stream_id() const;
  QuicHttpFrameType frame_type() const;

  size_t remaining_total_payload() const;

  bool IsReadingPaddingLength();
  bool IsSkippingPadding();
  bool IsDiscardingPayload();
  // Called from OnXyz or OnXyzStart methods to decide whether it is OK to
  // handle the callback.
  bool IsOkToStartFrame(const QuicHttpFrameHeader& header);
  bool HasRequiredStreamId(uint32_t stream_id);

  bool HasRequiredStreamId(const QuicHttpFrameHeader& header);

  bool HasRequiredStreamIdZero(uint32_t stream_id);

  bool HasRequiredStreamIdZero(const QuicHttpFrameHeader& header);

  void ReportReceiveCompressedFrame(const QuicHttpFrameHeader& header);

  void CommonStartHpackBlock();

  // SpdyFramer calls HandleControlFrameHeadersData even if there are zero
  // fragment bytes in the first frame, so do the same.
  void MaybeAnnounceEmptyFirstHpackFragment();
  void CommonHpackFragmentEnd();

  // The most recently decoded frame header; invalid after we reached the end
  // of that frame.
  QuicHttpFrameHeader frame_header_;

  // If decoding an HPQUIC_HTTP_ACK block that is split across multiple frames,
  // this holds the frame header of the HEADERS or PUSH_PROMISE that started the
  // block.
  QuicHttpFrameHeader hpack_first_frame_header_;

  // Amount of trailing padding. Currently used just as an indicator of whether
  // OnPadLength has been called.
  QuicOptional<size_t> opt_pad_length_;

  // Temporary buffers for the AltSvc fields.
  QuicString alt_svc_origin_;
  QuicString alt_svc_value_;

  // Listener used if we transition to an error state; the listener ignores all
  // the callbacks.
  QuicHttpFrameDecoderNoOpListener no_op_listener_;

  gfe_spdy::SpdyFramerVisitorInterface* visitor_ = nullptr;
  gfe_spdy::SpdyFramerDebugVisitorInterface* debug_visitor_ = nullptr;

  // If non-null, unknown frames and settings are passed to the extension.
  gfe_spdy::ExtensionVisitorInterface* extension_ = nullptr;

  // The HPQUIC_HTTP_ACK decoder to be used for this adapter. User is
  // responsible for clearing if the adapter is to be used for another
  // connection.
  std::unique_ptr<gfe_spdy::HpackDecoderAdapter> hpack_decoder_ = nullptr;

  // The HTTP/2 frame decoder. Accessed via a unique_ptr to allow replacement
  // (e.g. in tests) when Reset() is called.
  std::unique_ptr<QuicHttpFrameDecoder> frame_decoder_;

  // Next frame type expected. Currently only used for CONTINUATION frames,
  // but could be used for detecting whether the first frame is a SETTINGS
  // frame.
  // TODO: Provide means to indicate that decoder should require
  // SETTINGS frame as the first frame.
  QuicHttpFrameType expected_frame_type_;

  // Attempt to duplicate the SpdyState and SpdyFramer::Error values that
  // SpdyFramer sets. Values determined by getting tests to pass.
  SpdyState spdy_state_;
  gfe_http2::Http2DecoderAdapter::SpdyFramerError spdy_framer_error_;

  // The limit on the size of received HTTP/2 payloads as specified in the
  // SETTINGS_MAX_FRAME_SIZE advertised to peer.
  size_t recv_frame_size_limit_ = gfe_spdy::kHttp2DefaultFramePayloadLimit;

  // Has OnFrameHeader been called?
  bool decoded_frame_header_ = false;

  // Have we recorded an QuicHttpFrameHeader for the current frame?
  // We only do so if the decoder will make multiple callbacks for
  // the frame; for example, for PING frames we don't make record
  // the frame header, but for ALTSVC we do.
  bool has_frame_header_ = false;

  // Have we recorded an QuicHttpFrameHeader for the current HPQUIC_HTTP_ACK
  // block? True only for multi-frame HPQUIC_HTTP_ACK blocks.
  bool has_hpack_first_frame_header_ = false;

  // Has OnHeaders() already been called for current HEADERS block? Only
  // meaningful between OnHeadersStart and OnHeadersPriority.
  bool on_headers_called_;

  // Has OnHpackFragment() already been called for current HPQUIC_HTTP_ACK
  // block? SpdyFramer will pass an empty buffer to the HPQUIC_HTTP_ACK decoder
  // if a HEADERS or PUSH_PROMISE has no HPQUIC_HTTP_ACK data in it (e.g. a
  // HEADERS frame with only padding). Detect that condition and replicate the
  // behavior using this field.
  bool on_hpack_fragment_called_;

  // Have we seen a frame header that appears to be an HTTP/1 response?
  bool latched_probable_http_response_ = false;

  // Is expected_frame_type_ set?
  bool has_expected_frame_type_ = false;

  // Is the current frame payload destined for |extension_|?
  bool handling_extension_payload_ = false;

  bool process_single_input_frame_ = false;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_DECODER_QUIC_HTTP_FRAME_DECODER_ADAPTER_H_