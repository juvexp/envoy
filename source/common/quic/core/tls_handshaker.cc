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

#include "common/quic/core/tls_handshaker.h"

#include "common/quic/core/quic_crypto_stream.h"
#include "common/quic/core/tls_client_handshaker.h"
#include "common/quic/platform/api/quic_arraysize.h"
#include "common/quic/platform/api/quic_singleton.h"
#include "third_party/openssl/ssl.h"

namespace gfe_quic {

namespace {

const char kClientLabel[] = "EXPORTER-QUIC client 1-RTT Secret";
const char kServerLabel[] = "EXPORTER-QUIC server 1-RTT Secret";

}  // namespace

namespace {

class SslIndexSingleton {
 public:
  static SslIndexSingleton* GetInstance() {
    return QuicSingleton<SslIndexSingleton>::get();
  }

  int HandshakerIndex() { return ssl_ex_data_index_handshaker_; }

 private:
  SslIndexSingleton() {
    ssl_ex_data_index_handshaker_ =
        SSL_get_ex_new_index(0, nullptr, nullptr, nullptr, nullptr);
    CHECK_LE(0, ssl_ex_data_index_handshaker_);
  }

  friend QuicSingletonFriend<SslIndexSingleton>;

  int ssl_ex_data_index_handshaker_;

  DISALLOW_COPY_AND_ASSIGN(SslIndexSingleton);
};

}  // namespace

// static
TlsHandshaker* TlsHandshaker::HandshakerFromSsl(const SSL* ssl) {
  return reinterpret_cast<TlsHandshaker*>(SSL_get_ex_data(
      ssl, SslIndexSingleton::GetInstance()->HandshakerIndex()));
}

const EVP_MD* TlsHandshaker::Prf() {
  return EVP_get_digestbynid(
      SSL_CIPHER_get_prf_nid(SSL_get_current_cipher(ssl())));
}

bool TlsHandshaker::DeriveSecrets(std::vector<uint8_t>* client_secret_out,
                                  std::vector<uint8_t>* server_secret_out) {
  size_t hash_len = EVP_MD_size(Prf());
  client_secret_out->resize(hash_len);
  server_secret_out->resize(hash_len);
  return (SSL_export_keying_material(
              ssl(), client_secret_out->data(), hash_len, kClientLabel,
              QUIC_ARRAYSIZE(kClientLabel) - 1, nullptr, 0, 0) == 1) &&
         (SSL_export_keying_material(
              ssl(), server_secret_out->data(), hash_len, kServerLabel,
              QUIC_ARRAYSIZE(kServerLabel) - 1, nullptr, 0, 0) == 1);
}

std::unique_ptr<QuicEncrypter> TlsHandshaker::CreateEncrypter(
    const std::vector<uint8_t>& pp_secret) {
  std::unique_ptr<QuicEncrypter> encrypter =
      QuicEncrypter::CreateFromCipherSuite(
          SSL_CIPHER_get_id(SSL_get_current_cipher(ssl())));
  CryptoUtils::SetKeyAndIV(Prf(), pp_secret, encrypter.get());
  return encrypter;
}

std::unique_ptr<QuicDecrypter> TlsHandshaker::CreateDecrypter(
    const std::vector<uint8_t>& pp_secret) {
  std::unique_ptr<QuicDecrypter> decrypter =
      QuicDecrypter::CreateFromCipherSuite(
          SSL_CIPHER_get_id(SSL_get_current_cipher(ssl())));
  CryptoUtils::SetKeyAndIV(Prf(), pp_secret, decrypter.get());
  return decrypter;
}

// static
bssl::UniquePtr<SSL_CTX> TlsHandshaker::CreateSslCtx() {
  bssl::UniquePtr<SSL_CTX> ssl_ctx(SSL_CTX_new(TLS_with_buffers_method()));
  SSL_CTX_set_min_proto_version(ssl_ctx.get(), TLS1_3_VERSION);
  SSL_CTX_set_max_proto_version(ssl_ctx.get(), TLS1_3_VERSION);
  return ssl_ctx;
}

TlsHandshaker::TlsHandshaker(QuicCryptoStream* stream,
                             QuicSession* session,
                             SSL_CTX* ssl_ctx)
    : stream_(stream), session_(session), bio_adapter_(this) {
  ssl_.reset(SSL_new(ssl_ctx));
  SSL_set_ex_data(ssl(), SslIndexSingleton::GetInstance()->HandshakerIndex(),
                  this);

  // Set BIO for ssl_.
  BIO* bio = bio_adapter_.bio();
  BIO_up_ref(bio);
  SSL_set0_rbio(ssl(), bio);
  BIO_up_ref(bio);
  SSL_set0_wbio(ssl(), bio);
}

TlsHandshaker::~TlsHandshaker() {}

void TlsHandshaker::OnDataAvailableForBIO() {
  AdvanceHandshake();
}

void TlsHandshaker::OnDataReceivedFromBIO(const QuicStringPiece& data) {
  // TODO: Call NeuterUnencryptedPackets when encryption keys are set.
  stream_->WriteCryptoData(data);
}

CryptoMessageParser* TlsHandshaker::crypto_message_parser() {
  return &bio_adapter_;
}

}  // namespace gfe_quic
