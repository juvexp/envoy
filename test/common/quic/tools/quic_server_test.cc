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

#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>

#include "common/quic/core/crypto/quic_random.h"
#include "common/quic/core/quic_epoll_alarm_factory.h"
#include "common/quic/core/quic_utils.h"
#include "common/quic/core/tls_server_handshaker.h"
#include "common/quic/platform/api/quic_arraysize.h"
#include "common/quic/platform/api/quic_flags.h"
#include "common/quic/platform/api/quic_logging.h"
#include "common/quic/platform/api/quic_socket_address.h"
#include "test/common/quic/platform/api/quic_test.h"
#include "test/common/quic/platform/api/quic_test_loopback.h"
#include "test/common/quic/test_tools/crypto_test_utils.h"
#include "test/common/quic/test_tools/mock_quic_dispatcher.h"
#include "test/common/quic/test_tools/quic_server_peer.h"
#include "common/quic/tools/quic_server.h"
#include "common/quic/tools/quic_simple_crypto_server_stream_helper.h"
#include "net/util/netutil.h"

namespace gfe_quic {
namespace test {

using ::testing::_;

namespace {

class QuicServerTest : public QuicTest {};

TEST_F(QuicServerTest, TestDroppedPackets) {
  int port = PickUnusedPortOrDie();

  QuicSocketAddress server_address(TestLoopback(), port);
  QuicHttpResponseCache response_cache;
  QuicServer server(crypto_test_utils::ProofSourceForTesting(),
                    &response_cache);

  server.CreateUDPSocketAndListen(server_address);
  ASSERT_TRUE(QuicServerPeer::SetSmallSocket(&server));

  if (!server.overflow_supported()) {
    QUIC_LOG(WARNING) << "Overflow not supported.  Not testing.";
    return;
  }

  ASSERT_EQ(0, server.packets_dropped());
  int fd = socket(
      AddressFamilyUnderTest() == IpAddressFamily::IP_V4 ? AF_INET : AF_INET6,
      SOCK_DGRAM | SOCK_NONBLOCK, IPPROTO_UDP);
  ASSERT_LT(0, fd);

  char buf[1024];
  memset(buf, 0, QUIC_ARRAYSIZE(buf));
  sockaddr_storage storage = server_address.generic_address();

  for (int i = 0; i < 200; ++i) {
    int rc = sendto(fd, buf, QUIC_ARRAYSIZE(buf), 0,
                    reinterpret_cast<sockaddr*>(&storage), sizeof(storage));
    if (rc < 0) {
      QUIC_DLOG(INFO) << errno << " " << strerror(errno);
    }
    if (i % 25 == 0) {
      // Loop, to pick up packets_dropped from the kernel
      server.WaitForEvents();
    }
  }
  EXPECT_LT(1, server.packets_dropped());
  EXPECT_GT(201, server.packets_dropped());
}

class MockQuicSimpleDispatcher : public QuicSimpleDispatcher {
 public:
  MockQuicSimpleDispatcher(
      const QuicConfig& config,
      const QuicCryptoServerConfig* crypto_config,
      QuicVersionManager* version_manager,
      std::unique_ptr<QuicConnectionHelperInterface> helper,
      std::unique_ptr<QuicCryptoServerStream::Helper> session_helper,
      std::unique_ptr<QuicAlarmFactory> alarm_factory,
      QuicHttpResponseCache* response_cache)
      : QuicSimpleDispatcher(config,
                             crypto_config,
                             version_manager,
                             std::move(helper),
                             std::move(session_helper),
                             std::move(alarm_factory),
                             response_cache) {}
  ~MockQuicSimpleDispatcher() override {}

  MOCK_METHOD0(OnCanWrite, void());
  MOCK_CONST_METHOD0(HasPendingWrites, bool());
  MOCK_CONST_METHOD0(HasChlosBuffered, bool());
  MOCK_METHOD1(ProcessBufferedChlos, void(size_t));
};

class TestQuicServer : public QuicServer {
 public:
  TestQuicServer()
      : QuicServer(crypto_test_utils::ProofSourceForTesting(),
                   &response_cache_) {}

  ~TestQuicServer() override {}

  MockQuicSimpleDispatcher* mock_dispatcher() { return mock_dispatcher_; }

 protected:
  QuicDispatcher* CreateQuicDispatcher() override {
    mock_dispatcher_ = new MockQuicSimpleDispatcher(
        config(), &crypto_config(), version_manager(),
        std::unique_ptr<QuicEpollConnectionHelper>(
            new QuicEpollConnectionHelper(epoll_server(),
                                          QuicAllocator::BUFFER_POOL)),
        std::unique_ptr<QuicCryptoServerStream::Helper>(
            new QuicSimpleCryptoServerStreamHelper(QuicRandom::GetInstance())),
        std::unique_ptr<QuicEpollAlarmFactory>(
            new QuicEpollAlarmFactory(epoll_server())),
        &response_cache_);
    return mock_dispatcher_;
  }

  MockQuicSimpleDispatcher* mock_dispatcher_ = nullptr;
  QuicHttpResponseCache response_cache_;
};

class QuicServerEpollInTest : public QuicTest {
 public:
  QuicServerEpollInTest()
      : port_(PickUnusedPortOrDie()), server_address_(TestLoopback(), port_) {}

  void StartListening() {
    server_.CreateUDPSocketAndListen(server_address_);
    ASSERT_TRUE(QuicServerPeer::SetSmallSocket(&server_));

    if (!server_.overflow_supported()) {
      QUIC_LOG(WARNING) << "Overflow not supported.  Not testing.";
      return;
    }
  }

 protected:
  int port_;
  QuicSocketAddress server_address_;
  TestQuicServer server_;
};

// Tests that if dispatcher has CHLOs waiting for connection creation, EPOLLIN
// event should try to create connections for them. And set epoll mask with
// EPOLLIN if there are still CHLOs remaining at the end of epoll event.
TEST_F(QuicServerEpollInTest, ProcessBufferedCHLOsOnEpollin) {
  // Given an EPOLLIN event, try to create session for buffered CHLOs. In first
  // event, dispatcher can't create session for all of CHLOs. So listener should
  // register another EPOLLIN event by itself. Even without new packet arrival,
  // the rest CHLOs should be process in next epoll event.
  StartListening();
  bool more_chlos = true;
  MockQuicSimpleDispatcher* dispatcher_ = server_.mock_dispatcher();
  DCHECK(dispatcher_ != nullptr);
  EXPECT_CALL(*dispatcher_, OnCanWrite()).Times(testing::AnyNumber());
  EXPECT_CALL(*dispatcher_, ProcessBufferedChlos(_)).Times(2);
  EXPECT_CALL(*dispatcher_, HasPendingWrites()).Times(testing::AnyNumber());
  // Expect there are still CHLOs buffered after 1st event. But not any more
  // after 2nd event.
  EXPECT_CALL(*dispatcher_, HasChlosBuffered())
      .WillOnce(testing::Return(true))
      .WillOnce(
          DoAll(testing::Assign(&more_chlos, false), testing::Return(false)));

  // Send a packet to trigger epoll event.
  int fd = socket(
      AddressFamilyUnderTest() == IpAddressFamily::IP_V4 ? AF_INET : AF_INET6,
      SOCK_DGRAM | SOCK_NONBLOCK, IPPROTO_UDP);
  ASSERT_LT(0, fd);

  char buf[1024];
  memset(buf, 0, QUIC_ARRAYSIZE(buf));
  sockaddr_storage storage = server_address_.generic_address();
  int rc = sendto(fd, buf, QUIC_ARRAYSIZE(buf), 0,
                  reinterpret_cast<sockaddr*>(&storage), sizeof(storage));
  if (rc < 0) {
    QUIC_DLOG(INFO) << errno << " " << strerror(errno);
  }

  while (more_chlos) {
    server_.WaitForEvents();
  }
}

class QuicServerDispatchPacketTest : public QuicTest {
 public:
  QuicServerDispatchPacketTest()
      : crypto_config_("blah",
                       QuicRandom::GetInstance(),
                       crypto_test_utils::ProofSourceForTesting(),
                       TlsServerHandshaker::CreateSslCtx()),
        version_manager_(AllSupportedVersions()),
        dispatcher_(
            config_,
            &crypto_config_,
            &version_manager_,
            std::unique_ptr<QuicEpollConnectionHelper>(
                new QuicEpollConnectionHelper(&eps_,
                                              QuicAllocator::BUFFER_POOL)),
            std::unique_ptr<QuicCryptoServerStream::Helper>(
                new QuicSimpleCryptoServerStreamHelper(
                    QuicRandom::GetInstance())),
            std::unique_ptr<QuicEpollAlarmFactory>(
                new QuicEpollAlarmFactory(&eps_)),
            &response_cache_) {
    dispatcher_.InitializeWithWriter(new QuicDefaultPacketWriter(1234));
  }

  void DispatchPacket(const QuicReceivedPacket& packet) {
    QuicSocketAddress client_addr, server_addr;
    dispatcher_.ProcessPacket(server_addr, client_addr, packet);
  }

 protected:
  QuicConfig config_;
  QuicCryptoServerConfig crypto_config_;
  QuicVersionManager version_manager_;
  gfe2::EpollServer eps_;
  QuicHttpResponseCache response_cache_;
  MockQuicDispatcher dispatcher_;
};

TEST_F(QuicServerDispatchPacketTest, DispatchPacket) {
  // clang-format off
  unsigned char valid_packet[] = {
    // public flags (8 byte connection_id)
    0x3C,
    // connection_id
    0x10, 0x32, 0x54, 0x76,
    0x98, 0xBA, 0xDC, 0xFE,
    // packet number
    0xBC, 0x9A, 0x78, 0x56,
    0x34, 0x12,
    // private flags
    0x00
  };
  // clang-format on
  QuicReceivedPacket encrypted_valid_packet(
      reinterpret_cast<char*>(valid_packet), QUIC_ARRAYSIZE(valid_packet),
      QuicTime::Zero(), false);

  EXPECT_CALL(dispatcher_, ProcessPacket(_, _, _)).Times(1);
  DispatchPacket(encrypted_valid_packet);
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
