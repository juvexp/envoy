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

// Decodes the packet HandshakeFailureReason from the chromium histogram
// Net.QuicClientHelloRejectReasons

#include <iostream>

#include "base/commandlineflags.h"
#include "base/init_google.h"
#include "common/quic/core/crypto/crypto_handshake.h"
#include "common/quic/core/crypto/crypto_utils.h"
#include "common/quic/platform/api/quic_text_utils.h"

using gfe_quic::HandshakeFailureReason;
using gfe_quic::CryptoUtils;
using gfe_quic::MAX_FAILURE_REASON;
using std::cerr;
using std::cout;

int main(int argc, char* argv[]) {
  InitGoogle(argv[0], &argc, &argv, true);

  if (argc != 2) {
    std::cerr << "Missing argument (Usage: " << argv[0] << " <packed_reason>\n";
    return 1;
  }

  uint32_t packed_error = 0;
  if (!gfe_quic::QuicTextUtils::StringToUint32(argv[1], &packed_error)) {
    std::cerr << "Unable to parse: " << argv[1] << "\n";
    return 2;
  }

  for (int i = 1; i < MAX_FAILURE_REASON; ++i) {
    if ((packed_error & (1 << (i - 1))) == 0) {
      continue;
    }
    HandshakeFailureReason reason = static_cast<HandshakeFailureReason>(i);
    std::cout << CryptoUtils::HandshakeFailureReasonToString(reason) << "\n";
  }
  return 0;
}
