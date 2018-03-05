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

#ifndef GFE_QUIC_CORE_QUIC_SPDY_CLIENT_STREAM_H_
#define GFE_QUIC_CORE_QUIC_SPDY_CLIENT_STREAM_H_

#include <sys/types.h>

#include "base/macros.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_spdy_stream.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"
#include "common/quic/spdy_utils/core/spdy_framer.h"

namespace gfe_quic {

class QuicSpdyClientSession;

// All this does right now is send an SPDY request, and aggregate the
// SPDY response.
class QuicSpdyClientStream : public QuicSpdyStream {
 public:
  QuicSpdyClientStream(QuicStreamId id, QuicSpdyClientSession* session);
  ~QuicSpdyClientStream() override;

  // Override the base class to parse and store headers.
  void OnInitialHeadersComplete(bool fin,
                                size_t frame_len,
                                const QuicHeaderList& header_list) override;

  // Override the base class to parse and store trailers.
  void OnTrailingHeadersComplete(bool fin,
                                 size_t frame_len,
                                 const QuicHeaderList& header_list) override;

  // Override the base class to handle creation of the push stream.
  void OnPromiseHeaderList(QuicStreamId promised_id,
                           size_t frame_len,
                           const QuicHeaderList& header_list) override;

  // QuicStream implementation called by the session when there's data for us.
  void OnDataAvailable() override;

  // Serializes the headers and body, sends it to the server, and
  // returns the number of bytes sent.
  size_t SendRequest(gfe_spdy::SpdyHeaderBlock headers,
                     QuicStringPiece body,
                     bool fin);

  // Returns the response data.
  const QuicString& data() { return data_; }

  // Returns whatever headers have been received for this stream.
  const gfe_spdy::SpdyHeaderBlock& response_headers() {
    return response_headers_;
  }

  const gfe_spdy::SpdyHeaderBlock& preliminary_headers() {
    return preliminary_headers_;
  }

  size_t header_bytes_read() const { return header_bytes_read_; }

  size_t header_bytes_written() const { return header_bytes_written_; }

  int response_code() const { return response_code_; }

  // While the server's SetPriority shouldn't be called externally, the creator
  // of client-side streams should be able to set the priority.
  using QuicSpdyStream::SetPriority;

 private:
  // The parsed headers received from the server.
  gfe_spdy::SpdyHeaderBlock response_headers_;

  // The parsed content-length, or -1 if none is specified.
  int64_t content_length_;
  int response_code_;
  QuicString data_;
  size_t header_bytes_read_;
  size_t header_bytes_written_;

  QuicSpdyClientSession* session_;

  // These preliminary headers are used for the 100 Continue headers
  // that may arrive before the response headers when the request has
  // Expect: 100-continue.
  bool has_preliminary_headers_;
  gfe_spdy::SpdyHeaderBlock preliminary_headers_;

  DISALLOW_COPY_AND_ASSIGN(QuicSpdyClientStream);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_SPDY_CLIENT_STREAM_H_
