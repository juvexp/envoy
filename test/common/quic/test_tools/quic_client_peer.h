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

#ifndef GFE_QUIC_TEST_TOOLS_QUIC_CLIENT_PEER_H_
#define GFE_QUIC_TEST_TOOLS_QUIC_CLIENT_PEER_H_

#include "base/macros.h"

namespace gfe_quic {

class QuicClient;
class QuicCryptoClientConfig;
class QuicPacketWriter;

namespace test {

class QuicClientPeer {
 public:
  static bool CreateUDPSocketAndBind(QuicClient* client);
  static void CleanUpUDPSocket(QuicClient* client, int fd);
  static void SetClientPort(QuicClient* client, int port);
  static void SetWriter(QuicClient* client, QuicPacketWriter* writer);

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicClientPeer);
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_QUIC_CLIENT_PEER_H_
