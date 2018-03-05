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

#include "common/quic/tools/quic_client.h"

#include <memory>

#include "file/base/path.h"
#include "file/util/linux_fileops.h"
#include "gfe/gfe2/base/epoll_server.h"
#include "test/common/quic/platform/api/quic_test.h"
#include "test/common/quic/platform/api/quic_test_loopback.h"
#include "common/quic/platform/api/quic_text_utils.h"
#include "test/common/quic/test_tools/crypto_test_utils.h"
#include "test/common/quic/test_tools/quic_client_peer.h"
#include "net/util/netutil.h"
#include "testing/base/public/test_utils.h"

using gfe2::EpollServer;

namespace gfe_quic {
namespace test {
namespace {

const char* kPathToFds = "/proc/self/fd";

// Counts the number of open sockets for the current process.
size_t NumOpenSocketFDs() {
  std::vector<string> fd_entries;
  string error_message;

  CHECK(file_util::LinuxFileOps::ListDirectoryEntries(kPathToFds, &fd_entries,
                                                      &error_message));

  size_t socket_count = 0;
  for (const string& entry : fd_entries) {
    if (entry == "." || entry == "..") {
      continue;
    }

    string fd_path =
        file_util::LinuxFileOps::ReadLink(file::JoinPath(kPathToFds, entry));
    if (QuicTextUtils::StartsWith(fd_path, "socket:")) {
      socket_count++;
    }
  }

  return socket_count;
}

// Creates a new QuicClient and Initializes it. Caller is responsible for
// deletion.
QuicClient* CreateAndInitializeQuicClient(EpollServer* eps, uint16_t port) {
  QuicSocketAddress server_address(QuicSocketAddress(TestLoopback(), port));
  QuicServerId server_id("hostname", server_address.port(),
                         PRIVACY_MODE_DISABLED);
  ParsedQuicVersionVector versions = AllSupportedVersions();
  QuicClient* client =
      new QuicClient(server_address, server_id, versions, eps,
                     crypto_test_utils::ProofVerifierForTesting());
  EXPECT_TRUE(client->Initialize());
  return client;
}

class QuicClientTest : public QuicTest {};

TEST_F(QuicClientTest, DoNotLeakSocketFDs) {
  // Make sure that the QuicClient doesn't leak socket FDs. Doing so could cause
  // port exhaustion in long running processes which repeatedly create clients.

  // Record initial number of FDs, after creation of EpollServer.
  EpollServer eps;
  size_t number_of_open_fds = NumOpenSocketFDs();

  // Create a number of clients, initialize them, and verify this has resulted
  // in additional FDs being opened.
  const int kNumClients = 50;
  for (int i = 0; i < kNumClients; ++i) {
    std::unique_ptr<QuicClient> client(
        CreateAndInitializeQuicClient(&eps, PickUnusedPortOrDie()));

    // Initializing the client will create a new FD.
    EXPECT_LT(number_of_open_fds, NumOpenSocketFDs());
  }

  // The FDs created by the QuicClients should now be closed.
  EXPECT_EQ(number_of_open_fds, NumOpenSocketFDs());
}

TEST_F(QuicClientTest, CreateAndCleanUpUDPSockets) {
  EpollServer eps;
  size_t number_of_open_fds = NumOpenSocketFDs();

  std::unique_ptr<QuicClient> client(
      CreateAndInitializeQuicClient(&eps, PickUnusedPortOrDie()));
  EXPECT_EQ(number_of_open_fds + 1, NumOpenSocketFDs());
  // Create more UDP sockets.
  EXPECT_TRUE(QuicClientPeer::CreateUDPSocketAndBind(client.get()));
  EXPECT_EQ(number_of_open_fds + 2, NumOpenSocketFDs());
  EXPECT_TRUE(QuicClientPeer::CreateUDPSocketAndBind(client.get()));
  EXPECT_EQ(number_of_open_fds + 3, NumOpenSocketFDs());

  // Clean up UDP sockets.
  QuicClientPeer::CleanUpUDPSocket(client.get(), client->GetLatestFD());
  EXPECT_EQ(number_of_open_fds + 2, NumOpenSocketFDs());
  QuicClientPeer::CleanUpUDPSocket(client.get(), client->GetLatestFD());
  EXPECT_EQ(number_of_open_fds + 1, NumOpenSocketFDs());
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
