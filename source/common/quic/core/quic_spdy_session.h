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

#ifndef GFE_QUIC_CORE_QUIC_SPDY_SESSION_H_
#define GFE_QUIC_CORE_QUIC_SPDY_SESSION_H_

#include <cstddef>
#include <memory>

#include "base/macros.h"
#include "common/quic/core/quic_header_list.h"
#include "common/quic/core/quic_headers_stream.h"
#include "common/quic/core/quic_session.h"
#include "common/quic/core/quic_spdy_stream.h"
#include "common/quic/http/decoder/quic_http_frame_decoder_adapter.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"
#include "common/quic/spdy_utils/core/http2_frame_decoder_adapter.h"

namespace gfe_quic {

namespace test {
class QuicSpdySessionPeer;
}  // namespace test

// QuicHpackDebugVisitor gathers data used for understanding HPACK HoL
// dynamics.  Specifically, it is to help predict the compression
// penalty of avoiding HoL by chagning how the dynamic table is used.
// In chromium, the concrete instance populates an UMA
// histogram with the data.
class QUIC_EXPORT_PRIVATE QuicHpackDebugVisitor {
 public:
  QuicHpackDebugVisitor();

  virtual ~QuicHpackDebugVisitor();

  // For each HPACK indexed representation processed, |elapsed| is
  // the time since the corresponding entry was added to the dynamic
  // table.
  virtual void OnUseEntry(QuicTime::Delta elapsed) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicHpackDebugVisitor);
};

// A QUIC session with a headers stream.
class QUIC_EXPORT_PRIVATE QuicSpdySession : public QuicSession {
 public:
  // Does not take ownership of |connection| or |visitor|.
  QuicSpdySession(QuicConnection* connection,
                  QuicSession::Visitor* visitor,
                  const QuicConfig& config);

  ~QuicSpdySession() override;

  void Initialize() override;

  // Called by |headers_stream_| when headers with a priority have been
  // received for a stream.  This method will only be called for server streams.
  virtual void OnStreamHeadersPriority(QuicStreamId stream_id,
                                       gfe_spdy::SpdyPriority priority);

  // Called by |headers_stream_| when headers have been completely received
  // for a stream.  |fin| will be true if the fin flag was set in the headers
  // frame.
  virtual void OnStreamHeaderList(QuicStreamId stream_id,
                                  bool fin,
                                  size_t frame_len,
                                  const QuicHeaderList& header_list);

  // Called by |headers_stream_| when push promise headers have been
  // completely received.  |fin| will be true if the fin flag was set
  // in the headers.
  virtual void OnPromiseHeaderList(QuicStreamId stream_id,
                                   QuicStreamId promised_stream_id,
                                   size_t frame_len,
                                   const QuicHeaderList& header_list);

  // Callbed by |headers_stream_| when a PRIORITY frame has been received for a
  // stream. This method will only be called for server streams.
  virtual void OnPriorityFrame(QuicStreamId stream_id,
                               gfe_spdy::SpdyPriority priority);

  // Sends contents of |iov| to hq_deframer_, returns number of bytes processed.
  size_t ProcessHeaderData(const struct iovec& iov, QuicTime timestamp);

  // Writes |headers| for the stream |id| to the dedicated headers stream.
  // If |fin| is true, then no more data will be sent for the stream |id|.
  // If provided, |ack_notifier_delegate| will be registered to be notified when
  // we have seen ACKs for all packets resulting from this call.
  virtual size_t WriteHeaders(
      QuicStreamId id,
      gfe_spdy::SpdyHeaderBlock headers,
      bool fin,
      gfe_spdy::SpdyPriority priority,
      QuicReferenceCountedPointer<QuicAckListenerInterface> ack_listener);

  // Writes a PRIORITY frame the to peer. Returns the size in bytes of the
  // resulting PRIORITY frame for QUIC_VERSION_43 and above. Otherwise, does
  // nothing and returns 0.
  size_t WritePriority(QuicStreamId id,
                       QuicStreamId parent_stream_id,
                       int weight,
                       bool exclusive);

  // Write |headers| for |promised_stream_id| on |original_stream_id| in a
  // PUSH_PROMISE frame to peer.
  // Return the size, in bytes, of the resulting PUSH_PROMISE frame.
  virtual size_t WritePushPromise(QuicStreamId original_stream_id,
                                  QuicStreamId promised_stream_id,
                                  gfe_spdy::SpdyHeaderBlock headers);

  // Sends SETTINGS_MAX_HEADER_LIST_SIZE SETTINGS frame.
  size_t SendMaxHeaderListSize(size_t value);

  QuicHeadersStream* headers_stream() { return headers_stream_.get(); }

  // Called when Head of Line Blocking happens in the headers stream.
  // |delta| indicates how long that piece of data has been blocked.
  virtual void OnHeadersHeadOfLineBlocking(QuicTime::Delta delta);

  void OnConfigNegotiated() override;

  bool server_push_enabled() const { return server_push_enabled_; }

  void UpdateCurMaxTimeStamp(QuicTime timestamp) {
    cur_max_timestamp_ = std::max(timestamp, cur_max_timestamp_);
  }

  // Called by |QuicHeadersStream::UpdateEnableServerPush()| with
  // value from SETTINGS_ENABLE_PUSH.
  void set_server_push_enabled(bool enable) { server_push_enabled_ = enable; }

  // Return true if this session wants to release headers stream's buffer
  // aggressively.
  virtual bool ShouldReleaseHeadersStreamSequencerBuffer();

  void CloseConnectionWithDetails(QuicErrorCode error,
                                  const QuicString& details);

  void set_max_inbound_header_list_size(size_t max_inbound_header_list_size) {
    max_inbound_header_list_size_ = max_inbound_header_list_size;
  }

 protected:
  // Override CreateIncomingDynamicStream() and CreateOutgoingDynamicStream()
  // with QuicSpdyStream return type to make sure that all data streams are
  // QuicSpdyStreams.
  QuicSpdyStream* CreateIncomingDynamicStream(QuicStreamId id) override = 0;
  QuicSpdyStream* CreateOutgoingDynamicStream() override = 0;

  QuicSpdyStream* GetSpdyDataStream(const QuicStreamId stream_id);

  // If an incoming stream can be created, return true.
  virtual bool ShouldCreateIncomingDynamicStream(QuicStreamId id) = 0;

  // If an outgoing stream can be created, return true.
  virtual bool ShouldCreateOutgoingDynamicStream() = 0;

  // This was formerly QuicHeadersStream::WriteHeaders.  Needs to be
  // separate from QuicSpdySession::WriteHeaders because tests call
  // this but mock the latter.
  size_t WriteHeadersImpl(
      QuicStreamId id,
      gfe_spdy::SpdyHeaderBlock headers,
      bool fin,
      int weight,
      QuicStreamId parent_stream_id,
      bool exclusive,
      QuicReferenceCountedPointer<QuicAckListenerInterface> ack_listener);

  void OnCryptoHandshakeEvent(CryptoHandshakeEvent event) override;

  bool supports_push_promise() { return supports_push_promise_; }

  // Experimental: force HPACK to use static table and huffman coding
  // only.  Part of exploring improvements related to headers stream
  // induced HOL blocking in QUIC.
  void DisableHpackDynamicTable();

  // Optional, enables instrumentation related to go/quic-hpack.
  void SetHpackEncoderDebugVisitor(
      std::unique_ptr<QuicHpackDebugVisitor> visitor);
  void SetHpackDecoderDebugVisitor(
      std::unique_ptr<QuicHpackDebugVisitor> visitor);

  // Sets the maximum size of the header compression table spdy_framer_ is
  // willing to use to encode header blocks.
  void UpdateHeaderEncoderTableSize(uint32_t value);

  // Called when SETTINGS_ENABLE_PUSH is received, only supported on
  // server side.
  void UpdateEnableServerPush(bool value);

  bool IsConnected() { return connection()->connected(); }

  // Sets how much encoded data the hpack decoder of hq_deframer_ is willing to
  // buffer.
  void set_max_decode_buffer_size_bytes(size_t max_decode_buffer_size_bytes) {
    hq_deframer_.GetHpackDecoder()->set_max_decode_buffer_size_bytes(
        max_decode_buffer_size_bytes);
  }

  void set_max_uncompressed_header_bytes(
      size_t set_max_uncompressed_header_bytes);

 private:
  friend class test::QuicSpdySessionPeer;

  class SpdyFramerVisitor;

  // The following methods are called by the SimpleVisitor.

  // Called when a HEADERS frame has been received.
  void OnHeaders(gfe_spdy::SpdyStreamId stream_id,
                 bool has_priority,
                 gfe_spdy::SpdyPriority priority,
                 bool fin);

  // Called when a PUSH_PROMISE frame has been received.
  void OnPushPromise(gfe_spdy::SpdyStreamId stream_id,
                     gfe_spdy::SpdyStreamId promised_stream_id,
                     bool end);

  // Called when a PRIORITY frame has been received.
  void OnPriority(gfe_spdy::SpdyStreamId stream_id,
                  gfe_spdy::SpdyPriority priority);

  // Called when the complete list of headers is available.
  void OnHeaderList(const QuicHeaderList& header_list);

  // Called when the size of the compressed frame payload is available.
  void OnCompressedFrameSize(size_t frame_len);

  std::unique_ptr<QuicHeadersStream> headers_stream_;

  // The maximum size of a header block that will be accepted from the peer,
  // defined per spec as key + value + overhead per field (uncompressed).
  size_t max_inbound_header_list_size_;

  // Set during handshake. If true, resources in x-associated-content and link
  // headers will be pushed.
  bool server_push_enabled_;

  // Data about the stream whose headers are being processed.
  QuicStreamId stream_id_;
  QuicStreamId promised_stream_id_;
  bool fin_;
  size_t frame_len_;
  size_t uncompressed_frame_len_;

  bool supports_push_promise_;

  // Timestamps used to measure HOL blocking, these are recorded by
  // the sequencer approximate to the time of arrival off the wire.
  // |cur_max_timestamp_| tracks the most recent arrival time of
  // frames for current (at the headers stream level) processed
  // stream's headers, and |prev_max_timestamp_| tracks the most
  // recent arrival time of lower numbered streams.
  QuicTime cur_max_timestamp_;
  QuicTime prev_max_timestamp_;

  gfe_spdy::SpdyFramer spdy_framer_;
  QuicHttpDecoderAdapter hq_deframer_;
  std::unique_ptr<SpdyFramerVisitor> spdy_framer_visitor_;

  DISALLOW_COPY_AND_ASSIGN(QuicSpdySession);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_SPDY_SESSION_H_
