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

#ifndef GFE_QUIC_CORE_STATELESS_REJECTOR_H_
#define GFE_QUIC_CORE_STATELESS_REJECTOR_H_

#include "common/quic/core/crypto/crypto_framer.h"
#include "common/quic/core/crypto/quic_crypto_server_config.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {

// The StatelessRejector receives CHLO messages and generates an SREJ
// message in response, if the CHLO can be statelessly rejected.
class StatelessRejector {
 public:
  enum State {
    UNKNOWN,      // State has not yet been determined
    UNSUPPORTED,  // Stateless rejects are not supported
    FAILED,       // There was an error processing the CHLO.
    ACCEPTED,     // The CHLO was accepted
    REJECTED,     // The CHLO was rejected.
  };

  StatelessRejector(QuicTransportVersion version,
                    const QuicTransportVersionVector& versions,
                    const QuicCryptoServerConfig* crypto_config,
                    QuicCompressedCertsCache* compressed_certs_cache,
                    const QuicClock* clock,
                    QuicRandom* random,
                    QuicByteCount chlo_packet_size,
                    const QuicSocketAddress& client_address,
                    const QuicSocketAddress& server_address);

  ~StatelessRejector();

  // Called when |chlo| is received for |connection_id|.
  void OnChlo(QuicTransportVersion version,
              QuicConnectionId connection_id,
              QuicConnectionId server_designated_connection_id,
              const CryptoHandshakeMessage& chlo);

  class ProcessDoneCallback {
   public:
    virtual ~ProcessDoneCallback() = default;
    virtual void Run(std::unique_ptr<StatelessRejector> rejector) = 0;
  };

  // Perform processing to determine whether the CHLO received in OnChlo should
  // be statelessly rejected, and invoke the callback once a decision has been
  // made.
  static void Process(std::unique_ptr<StatelessRejector> rejector,
                      std::unique_ptr<ProcessDoneCallback> done_cb);

  // Returns the state of the rejector after OnChlo() has been called.
  State state() const { return state_; }

  // Returns the error code when state() returns FAILED.
  QuicErrorCode error() const { return error_; }

  // Returns the error details when state() returns FAILED.
  QuicString error_details() const { return error_details_; }

  // Returns the connection ID.
  QuicConnectionId connection_id() const { return connection_id_; }

  // Returns the SREJ message when state() returns REJECTED.
  const CryptoHandshakeMessage& reply() const { return *reply_; }

 private:
  // Helper class which is passed in to
  // QuicCryptoServerConfig::ValidateClientHello.
  class ValidateCallback;
  friend class ValidateCallback;

  class ProcessClientHelloCallback;
  friend class ProcessClientHelloCallback;

  void ProcessClientHello(
      QuicReferenceCountedPointer<ValidateClientHelloResultCallback::Result>
          result,
      std::unique_ptr<StatelessRejector> rejector,
      std::unique_ptr<StatelessRejector::ProcessDoneCallback> done_cb);

  void ProcessClientHelloDone(
      QuicErrorCode error,
      const QuicString& error_details,
      std::unique_ptr<CryptoHandshakeMessage> message,
      std::unique_ptr<StatelessRejector> rejector,
      std::unique_ptr<StatelessRejector::ProcessDoneCallback> done_cb);

  State state_;
  QuicErrorCode error_;
  QuicString error_details_;
  QuicTransportVersion version_;
  QuicTransportVersionVector versions_;
  QuicConnectionId connection_id_;
  QuicConnectionId server_designated_connection_id_;
  QuicByteCount chlo_packet_size_;
  QuicSocketAddress client_address_;
  QuicSocketAddress server_address_;
  const QuicClock* clock_;
  QuicRandom* random_;
  const QuicCryptoServerConfig* crypto_config_;
  QuicCompressedCertsCache* compressed_certs_cache_;
  CryptoHandshakeMessage chlo_;
  std::unique_ptr<CryptoHandshakeMessage> reply_;
  CryptoFramer crypto_framer_;
  QuicReferenceCountedPointer<QuicSignedServerConfig> signed_config_;
  QuicReferenceCountedPointer<QuicCryptoNegotiatedParameters> params_;

  DISALLOW_COPY_AND_ASSIGN(StatelessRejector);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_STATELESS_REJECTOR_H_
