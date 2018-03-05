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

#include "common/quic/core/crypto/crypto_handshake.h"

#include "common/quic/core/crypto/common_cert_set.h"
#include "common/quic/core/crypto/key_exchange.h"
#include "common/quic/core/crypto/quic_decrypter.h"
#include "common/quic/core/crypto/quic_encrypter.h"

namespace gfe_quic {

QuicCryptoNegotiatedParameters::QuicCryptoNegotiatedParameters()
    : key_exchange(0),
      aead(0),
      token_binding_key_param(0),
      sct_supported_by_client(false) {}

QuicCryptoNegotiatedParameters::~QuicCryptoNegotiatedParameters() {}

CrypterPair::CrypterPair() {}

CrypterPair::~CrypterPair() {}

// static
const char QuicCryptoConfig::kInitialLabel[] = "QUIC key expansion";

// static
const char QuicCryptoConfig::kCETVLabel[] = "QUIC CETV block";

// static
const char QuicCryptoConfig::kForwardSecureLabel[] =
    "QUIC forward secure key expansion";

QuicCryptoConfig::QuicCryptoConfig()
    : common_cert_sets(CommonCertSets::GetInstanceQUIC()) {}

QuicCryptoConfig::~QuicCryptoConfig() {}

}  // namespace gfe_quic
