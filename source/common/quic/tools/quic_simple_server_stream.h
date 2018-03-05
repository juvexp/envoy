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

#ifndef GFE_QUIC_TOOLS_QUIC_SIMPLE_SERVER_STREAM_H_
#define GFE_QUIC_TOOLS_QUIC_SIMPLE_SERVER_STREAM_H_

#include <string>

#include "base/macros.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_spdy_server_stream_base.h"
#include "common/quic/platform/api/quic_string_piece.h"
#include "common/quic/tools/quic_http_response_cache.h"
#include "common/quic/spdy_utils/core/spdy_framer.h"

namespace gfe_quic {

namespace test {
class QuicSimpleServerStreamPeer;
}  // namespace test

// All this does right now is aggregate data, and on fin, send an HTTP
// response.
class QuicSimpleServerStream : public QuicSpdyServerStreamBase {
 public:
  QuicSimpleServerStream(QuicStreamId id,
                         QuicSpdySession* session,
                         QuicHttpResponseCache* response_cache);
  ~QuicSimpleServerStream() override;

  // QuicSpdyStream
  void OnInitialHeadersComplete(bool fin,
                                size_t frame_len,
                                const QuicHeaderList& header_list) override;
  void OnTrailingHeadersComplete(bool fin,
                                 size_t frame_len,
                                 const QuicHeaderList& header_list) override;

  // QuicStream implementation called by the sequencer when there is
  // data (or a FIN) to be read.
  void OnDataAvailable() override;

  // Make this stream start from as if it just finished parsing an incoming
  // request whose headers are equivalent to |push_request_headers|.
  // Doing so will trigger this toy stream to fetch response and send it back.
  virtual void PushResponse(gfe_spdy::SpdyHeaderBlock push_request_headers);

  // The response body of error responses.
  static const char* const kErrorResponseBody;
  static const char* const kNotFoundResponseBody;

 protected:
  // Sends a basic 200 response using SendHeaders for the headers and WriteData
  // for the body.
  virtual void SendResponse();

  // Sends a basic 500 response using SendHeaders for the headers and WriteData
  // for the body.
  virtual void SendErrorResponse();

  // Sends a basic 404 response using SendHeaders for the headers and WriteData
  // for the body.
  void SendNotFoundResponse();

  void SendHeadersAndBody(gfe_spdy::SpdyHeaderBlock response_headers,
                          QuicStringPiece body);
  void SendHeadersAndBodyAndTrailers(
      gfe_spdy::SpdyHeaderBlock response_headers,
      QuicStringPiece body,
      gfe_spdy::SpdyHeaderBlock response_trailers);

  gfe_spdy::SpdyHeaderBlock* request_headers() { return &request_headers_; }

  const string& body() { return body_; }

 private:
  friend class test::QuicSimpleServerStreamPeer;

  // The parsed headers received from the client.
  gfe_spdy::SpdyHeaderBlock request_headers_;
  int64_t content_length_;
  string body_;

  QuicHttpResponseCache* response_cache_;  // Not owned.

  DISALLOW_COPY_AND_ASSIGN(QuicSimpleServerStream);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_TOOLS_QUIC_SIMPLE_SERVER_STREAM_H_
