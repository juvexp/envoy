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

// A class to read incoming QUIC packets from the UDP socket.

#ifndef GFE_QUIC_CORE_QUIC_PACKET_READER_H_
#define GFE_QUIC_CORE_QUIC_PACKET_READER_H_

// Include here to guarantee this header gets included (for MSG_WAITFORONE)
// regardless of how the below transitive header include set may change.
#include <sys/socket.h>

#include "base/macros.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_process_packet_interface.h"
#include "common/quic/platform/api/quic_clock.h"
#include "common/quic/platform/api/quic_socket_address.h"
#include "common/quic/platform/impl/quic_socket_utils.h"

#if __GOOGLE_GRTE_VERSION__ >= 20120225L
#define MMSG_MORE 1
#elif defined(MSG_WAITFORONE)
#define MMSG_MORE 1
#else
#define MMSG_MORE 0
#endif

namespace gfe_quic {

#if MMSG_MORE
// Read in larger batches to minimize recvmmsg overhead.
const int kNumPacketsPerReadMmsgCall = 16;
#endif

namespace test {
class QuicServerPeer;
}  // namespace test

class QuicDispatcher;

class QuicPacketReader {
 public:
  QuicPacketReader();

  virtual ~QuicPacketReader();

  // Reads a number of packets from the given fd, and then passes them off to
  // the PacketProcessInterface.  Returns true if there may be additional
  // packets available on the socket.
  // Populates |packets_dropped| if it is non-null and the socket is configured
  // to track dropped packets and some packets are read.
  // If the socket has timestamping enabled, the per packet timestamps will be
  // passed to the processor. Otherwise, |clock| will be used.
  virtual bool ReadAndDispatchPackets(int fd,
                                      int port,
                                      const QuicClock& clock,
                                      ProcessPacketInterface* processor,
                                      QuicPacketCount* packets_dropped);

 private:
  // Initialize the internal state of the reader.
  void Initialize();

  // Reads and dispatches many packets using recvmmsg.
  bool ReadAndDispatchManyPackets(int fd,
                                  int port,
                                  const QuicClock& clock,
                                  ProcessPacketInterface* processor,
                                  QuicPacketCount* packets_dropped);

  // Reads and dispatches a single packet using recvmsg.
  static bool ReadAndDispatchSinglePacket(int fd,
                                          int port,
                                          const QuicClock& clock,
                                          ProcessPacketInterface* processor,
                                          QuicPacketCount* packets_dropped);

  // Storage only used when recvmmsg is available.
#if MMSG_MORE
  // TODO: change it to be a pointer to avoid the allocation on the stack
  // from exceeding maximum allowed frame size.
  // packets_ and mmsg_hdr_ are used to supply cbuf and buf to the recvmmsg
  // call.
  struct PacketData {
    iovec iov;
    // raw_address is used for address information provided by the recvmmsg
    // call on the packets.
    struct sockaddr_storage raw_address;
    // cbuf is used for ancillary data from the kernel on recvmmsg.
    char cbuf[QuicSocketUtils::kSpaceForCmsg];
    // buf is used for the data read from the kernel on recvmmsg.
    char buf[kMaxPacketSize];
  };
  PacketData packets_[kNumPacketsPerReadMmsgCall];
  mmsghdr mmsg_hdr_[kNumPacketsPerReadMmsgCall];
#endif

  DISALLOW_COPY_AND_ASSIGN(QuicPacketReader);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_PACKET_READER_H_
