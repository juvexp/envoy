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

#include "common/quic/core/quic_headers_stream.h"

#include "common/quic/core/quic_spdy_session.h"
#include "common/quic/platform/api/quic_arraysize.h"
#include "common/quic/platform/api/quic_flag_utils.h"
#include "common/quic/platform/api/quic_flags.h"

namespace gfe_quic {

QuicHeadersStream::CompressedHeaderInfo::CompressedHeaderInfo(
    QuicStreamOffset headers_stream_offset,
    QuicStreamOffset full_length,
    QuicReferenceCountedPointer<QuicAckListenerInterface> ack_listener)
    : headers_stream_offset(headers_stream_offset),
      full_length(full_length),
      unacked_length(full_length),
      ack_listener(std::move(ack_listener)) {}

QuicHeadersStream::CompressedHeaderInfo::CompressedHeaderInfo(
    const CompressedHeaderInfo& other) = default;

QuicHeadersStream::CompressedHeaderInfo::~CompressedHeaderInfo() {}

QuicHeadersStream::QuicHeadersStream(QuicSpdySession* session)
    : QuicStream(kHeadersStreamId, session, /*is_static=*/true),
      spdy_session_(session) {
  // The headers stream is exempt from connection level flow control.
  DisableConnectionFlowControlForThisStream();
}

QuicHeadersStream::~QuicHeadersStream() {}

void QuicHeadersStream::OnDataAvailable() {
  char buffer[1024];
  struct iovec iov;
  QuicTime timestamp(QuicTime::Zero());
  while (true) {
    iov.iov_base = buffer;
    iov.iov_len = QUIC_ARRAYSIZE(buffer);
    if (!sequencer()->GetReadableRegion(&iov, &timestamp)) {
      // No more data to read.
      break;
    }
    if (spdy_session_->ProcessHeaderData(iov, timestamp) != iov.iov_len) {
      // Error processing data.
      return;
    }
    sequencer()->MarkConsumed(iov.iov_len);
    MaybeReleaseSequencerBuffer();
  }
}

void QuicHeadersStream::MaybeReleaseSequencerBuffer() {
  if (spdy_session_->ShouldReleaseHeadersStreamSequencerBuffer()) {
    sequencer()->ReleaseBufferIfEmpty();
  }
}

bool QuicHeadersStream::OnStreamFrameAcked(QuicStreamOffset offset,
                                           QuicByteCount data_length,
                                           bool fin_acked,
                                           QuicTime::Delta ack_delay_time) {
  QuicIntervalSet<QuicStreamOffset> newly_acked(offset, offset + data_length);
  newly_acked.Difference(bytes_acked());
  for (const auto& acked : newly_acked) {
    QuicStreamOffset acked_offset = acked.min();
    QuicByteCount acked_length = acked.max() - acked.min();
    for (CompressedHeaderInfo& header : unacked_headers_) {
      if (acked_offset < header.headers_stream_offset) {
        // This header frame offset belongs to headers with smaller offset, stop
        // processing.
        break;
      }

      if (acked_offset >= header.headers_stream_offset + header.full_length) {
        // This header frame belongs to headers with larger offset.
        continue;
      }

      QuicByteCount header_offset = acked_offset - header.headers_stream_offset;
      QuicByteCount header_length =
          std::min(acked_length, header.full_length - header_offset);

      if (header.unacked_length < header_length) {
        QUIC_BUG << "Unsent stream data is acked. unacked_length: "
                 << header.unacked_length << " acked_length: " << header_length;
        CloseConnectionWithDetails(QUIC_INTERNAL_ERROR,
                                   "Unsent stream data is acked");
        return false;
      }
      if (header.ack_listener != nullptr && header_length > 0) {
        header.ack_listener->OnPacketAcked(header_length, ack_delay_time);
      }
      header.unacked_length -= header_length;
      acked_offset += header_length;
      acked_length -= header_length;
    }
  }
  // Remove headers which are fully acked. Please note, header frames can be
  // acked out of order, but unacked_headers_ is cleaned up in order.
  while (!unacked_headers_.empty() &&
         unacked_headers_.front().unacked_length == 0) {
    unacked_headers_.pop_front();
  }
  return QuicStream::OnStreamFrameAcked(offset, data_length, fin_acked,
                                        ack_delay_time);
}

void QuicHeadersStream::OnStreamFrameRetransmitted(QuicStreamOffset offset,
                                                   QuicByteCount data_length,
                                                   bool /*fin_retransmitted*/) {
  QuicStream::OnStreamFrameRetransmitted(offset, data_length, false);
  for (CompressedHeaderInfo& header : unacked_headers_) {
    if (offset < header.headers_stream_offset) {
      // This header frame offset belongs to headers with smaller offset, stop
      // processing.
      break;
    }

    if (offset >= header.headers_stream_offset + header.full_length) {
      // This header frame belongs to headers with larger offset.
      continue;
    }

    QuicByteCount header_offset = offset - header.headers_stream_offset;
    QuicByteCount retransmitted_length =
        std::min(data_length, header.full_length - header_offset);
    if (header.ack_listener != nullptr && retransmitted_length > 0) {
      header.ack_listener->OnPacketRetransmitted(retransmitted_length);
    }
    offset += retransmitted_length;
    data_length -= retransmitted_length;
  }
}

void QuicHeadersStream::OnDataBuffered(
    QuicStreamOffset offset,
    QuicByteCount data_length,
    const QuicReferenceCountedPointer<QuicAckListenerInterface>& ack_listener) {
  // Populate unacked_headers_.
  if (!unacked_headers_.empty() &&
      (offset == unacked_headers_.back().headers_stream_offset +
                     unacked_headers_.back().full_length) &&
      ack_listener == unacked_headers_.back().ack_listener) {
    // Try to combine with latest inserted entry if they belong to the same
    // header (i.e., having contiguous offset and the same ack listener).
    unacked_headers_.back().full_length += data_length;
    unacked_headers_.back().unacked_length += data_length;
  } else {
    unacked_headers_.push_back(
        CompressedHeaderInfo(offset, data_length, ack_listener));
  }
}

}  // namespace gfe_quic
