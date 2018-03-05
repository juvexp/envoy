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

// A base class for the toy spdy client, which connects to a specified port and
// sends a QUIC request to that endpoint.

#ifndef GFE_QUIC_TOOLS_QUIC_SPDY_CLIENT_BASE_H_
#define GFE_QUIC_TOOLS_QUIC_SPDY_CLIENT_BASE_H_

#include <string>

#include "base/macros.h"
#include "common/quic/core/crypto/crypto_handshake.h"
#include "common/quic/core/quic_client_push_promise_index.h"
#include "common/quic/core/quic_config.h"
#include "common/quic/core/quic_spdy_client_session.h"
#include "common/quic/core/quic_spdy_client_stream.h"
#include "common/quic/platform/api/quic_socket_address.h"
#include "common/quic/platform/api/quic_string_piece.h"
#include "common/quic/tools/quic_client_base.h"

namespace gfe_quic {

class ProofVerifier;
class QuicServerId;

class QuicSpdyClientBase : public QuicClientBase,
                           public QuicClientPushPromiseIndex::Delegate,
                           public QuicSpdyStream::Visitor {
 public:
  // A ResponseListener is notified when a complete response is received.
  class ResponseListener {
   public:
    ResponseListener() {}
    virtual ~ResponseListener() {}
    virtual void OnCompleteResponse(
        QuicStreamId id,
        const gfe_spdy::SpdyHeaderBlock& response_headers,
        const string& response_body) = 0;
  };

  // The client uses these objects to keep track of any data to resend upon
  // receipt of a stateless reject.  Recall that the client API allows callers
  // to optimistically send data to the server prior to handshake-confirmation.
  // If the client subsequently receives a stateless reject, it must tear down
  // its existing session, create a new session, and resend all previously sent
  // data.  It uses these objects to keep track of all the sent data, and to
  // resend the data upon a subsequent connection.
  class QuicDataToResend {
   public:
    // |headers| may be null, since it's possible to send data without headers.
    QuicDataToResend(std::unique_ptr<gfe_spdy::SpdyHeaderBlock> headers,
                     QuicStringPiece body,
                     bool fin);

    virtual ~QuicDataToResend();

    // Must be overridden by specific classes with the actual method for
    // re-sending data.
    virtual void Resend() = 0;

   protected:
    std::unique_ptr<gfe_spdy::SpdyHeaderBlock> headers_;
    QuicStringPiece body_;
    bool fin_;

   private:
    DISALLOW_COPY_AND_ASSIGN(QuicDataToResend);
  };

  QuicSpdyClientBase(const QuicServerId& server_id,
                     const ParsedQuicVersionVector& supported_versions,
                     const QuicConfig& config,
                     QuicConnectionHelperInterface* helper,
                     QuicAlarmFactory* alarm_factory,
                     std::unique_ptr<NetworkHelper> network_helper,
                     std::unique_ptr<ProofVerifier> proof_verifier);

  ~QuicSpdyClientBase() override;

  // QuicSpdyStream::Visitor
  void OnClose(QuicSpdyStream* stream) override;

  // A spdy session has to call CryptoConnect on top of the regular
  // initialization.
  void InitializeSession() override;

  // Sends an HTTP request and does not wait for response before returning.
  void SendRequest(const gfe_spdy::SpdyHeaderBlock& headers,
                   QuicStringPiece body,
                   bool fin);

  // Sends an HTTP request and waits for response before returning.
  void SendRequestAndWaitForResponse(const gfe_spdy::SpdyHeaderBlock& headers,
                                     QuicStringPiece body,
                                     bool fin);

  // Sends a request simple GET for each URL in |url_list|, and then waits for
  // each to complete.
  void SendRequestsAndWaitForResponse(const std::vector<string>& url_list);

  // Returns a newly created QuicSpdyClientStream.
  QuicSpdyClientStream* CreateClientStream();

  // Returns a the session used for this client downcasted to a
  // QuicSpdyClientSession.
  QuicSpdyClientSession* client_session();

  QuicClientPushPromiseIndex* push_promise_index() {
    return &push_promise_index_;
  }

  bool CheckVary(const gfe_spdy::SpdyHeaderBlock& client_request,
                 const gfe_spdy::SpdyHeaderBlock& promise_request,
                 const gfe_spdy::SpdyHeaderBlock& promise_response) override;
  void OnRendezvousResult(QuicSpdyStream*) override;

  // If the crypto handshake has not yet been confirmed, adds the data to the
  // queue of data to resend if the client receives a stateless reject.
  // Otherwise, deletes the data.
  void MaybeAddQuicDataToResend(
      std::unique_ptr<QuicDataToResend> data_to_resend);

  void set_store_response(bool val) { store_response_ = val; }

  size_t latest_response_code() const;
  const string& latest_response_headers() const;
  const string& preliminary_response_headers() const;
  const gfe_spdy::SpdyHeaderBlock& latest_response_header_block() const;
  const string& latest_response_body() const;
  const string& latest_response_trailers() const;

  void set_response_listener(std::unique_ptr<ResponseListener> listener) {
    response_listener_ = std::move(listener);
  }

 protected:
  int GetNumSentClientHellosFromSession() override;
  int GetNumReceivedServerConfigUpdatesFromSession() override;

  // Takes ownership of |connection|.
  std::unique_ptr<QuicSession> CreateQuicClientSession(
      QuicConnection* connection) override;

  // If the crypto handshake has not yet been confirmed, adds the data to the
  // queue of data to resend if the client receives a stateless reject.
  // Otherwise, deletes the data.
  void MaybeAddDataToResend(const gfe_spdy::SpdyHeaderBlock& headers,
                            QuicStringPiece body,
                            bool fin);

  void ClearDataToResend() override;

  void ResendSavedData() override;

  void AddPromiseDataToResend(const gfe_spdy::SpdyHeaderBlock& headers,
                              QuicStringPiece body,
                              bool fin);

 private:
  // Specific QuicClient class for storing data to resend.
  class ClientQuicDataToResend : public QuicDataToResend {
   public:
    ClientQuicDataToResend(std::unique_ptr<gfe_spdy::SpdyHeaderBlock> headers,
                           QuicStringPiece body,
                           bool fin,
                           QuicSpdyClientBase* client)
        : QuicDataToResend(std::move(headers), body, fin), client_(client) {
      DCHECK(headers_);
      DCHECK(client);
    }

    ~ClientQuicDataToResend() override {}

    void Resend() override;

   private:
    QuicSpdyClientBase* client_;

    DISALLOW_COPY_AND_ASSIGN(ClientQuicDataToResend);
  };

  // Index of pending promised streams. Must outlive |session_|.
  QuicClientPushPromiseIndex push_promise_index_;

  // If true, store the latest response code, headers, and body.
  bool store_response_;
  // HTTP response code from most recent response.
  int latest_response_code_;
  // HTTP/2 headers from most recent response.
  string latest_response_headers_;
  // preliminary 100 Continue HTTP/2 headers from most recent response, if any.
  string preliminary_response_headers_;
  // HTTP/2 headers from most recent response.
  gfe_spdy::SpdyHeaderBlock latest_response_header_block_;
  // Body of most recent response.
  string latest_response_body_;
  // HTTP/2 trailers from most recent response.
  string latest_response_trailers_;

  // Listens for full responses.
  std::unique_ptr<ResponseListener> response_listener_;

  // Keeps track of any data that must be resent upon a subsequent successful
  // connection, in case the client receives a stateless reject.
  std::vector<std::unique_ptr<QuicDataToResend>> data_to_resend_on_connect_;

  std::unique_ptr<ClientQuicDataToResend> push_promise_data_to_resend_;

  DISALLOW_COPY_AND_ASSIGN(QuicSpdyClientBase);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_TOOLS_QUIC_SPDY_CLIENT_BASE_H_
