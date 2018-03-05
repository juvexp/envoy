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

// Dumps the contents of a QUIC crypto handshake message in a human readable
// format.
//
// Usage: crypto_message_printer_bin <hex of message>

#include <iostream>

#include "base/commandlineflags.h"
#include "base/init_google.h"
#include "common/quic/core/crypto/crypto_framer.h"
#include "common/quic/core/quic_utils.h"
#include "common/quic/platform/api/quic_flags.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_text_utils.h"

using gfe_quic::Perspective;
using gfe_quic::QuicString;
using std::cerr;
using std::cout;
using std::endl;

DEFINE_FLAG(QuicString,
            perspective,
            "",
            "As endianess of client and server may vary, need to specify "
            "perspective of this message printer.");

namespace gfe_quic {

class CryptoMessagePrinter : public ::gfe_quic::CryptoFramerVisitorInterface {
 public:
  explicit CryptoMessagePrinter(Perspective perspective)
      : perspective_(perspective) {}

  void OnHandshakeMessage(const CryptoHandshakeMessage& message) override {
    cout << message.DebugString(perspective_) << endl;
  }

  void OnError(CryptoFramer* framer) override {
    cerr << "Error code: " << framer->error() << endl;
    cerr << "Error details: " << framer->error_detail() << endl;
  }

  Perspective perspective_;
};

}  // namespace gfe_quic

int main(int argc, char* argv[]) {
  InitGoogle(argv[0], &argc, &argv, true);

  if (argc != 2) {
    cerr << "Usage: " << argv[0]
         << " --perspective=server/client <hex of message>\n";
    return 1;
  }

  if (GetQuicFlag(FLAGS_perspective) != "server" &&
      GetQuicFlag(FLAGS_perspective) != "client") {
    cerr << "perspective must be either server or client\n";
    return 1;
  }

  Perspective perspective = GetQuicFlag(FLAGS_perspective) == "server"
                                ? Perspective::IS_SERVER
                                : Perspective::IS_CLIENT;

  gfe_quic::CryptoMessagePrinter printer(perspective);
  gfe_quic::CryptoFramer framer;
  framer.set_visitor(&printer);
  framer.set_process_truncated_messages(true);
  QuicString input = gfe_quic::QuicTextUtils::HexDecode(argv[1]);
  if (!framer.ProcessInput(input, perspective)) {
    return 1;
  }
  if (framer.InputBytesRemaining() != 0) {
    cerr << "Input partially consumed. " << framer.InputBytesRemaining()
         << " bytes remaining." << endl;
    return 2;
  }
  return 0;
}
