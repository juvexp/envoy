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

#ifndef GFE_QUIC_TEST_TOOLS_QUIC_SESSION_PEER_H_
#define GFE_QUIC_TEST_TOOLS_QUIC_SESSION_PEER_H_

#include <cstdint>
#include <map>
#include <memory>

#include "base/macros.h"
#include "common/quic/core/quic_packets.h"
#include "common/quic/core/quic_session.h"
#include "common/quic/core/quic_write_blocked_list.h"
#include "common/quic/platform/api/quic_containers.h"

namespace gfe_quic {

class QuicCryptoStream;
class QuicSession;
class QuicStream;

namespace test {

class QuicSessionPeer {
 public:
  static QuicStreamId GetNextOutgoingStreamId(QuicSession* session);
  static void SetNextOutgoingStreamId(QuicSession* session, QuicStreamId id);
  static void SetMaxOpenIncomingStreams(QuicSession* session,
                                        uint32_t max_streams);
  static void SetMaxOpenOutgoingStreams(QuicSession* session,
                                        uint32_t max_streams);
  static QuicCryptoStream* GetMutableCryptoStream(QuicSession* session);
  static QuicWriteBlockedList* GetWriteBlockedStreams(QuicSession* session);
  static QuicStream* GetOrCreateDynamicStream(QuicSession* session,
                                              QuicStreamId stream_id);
  static std::map<QuicStreamId, QuicStreamOffset>&
  GetLocallyClosedStreamsHighestOffset(QuicSession* session);
  static QuicSession::StaticStreamMap& static_streams(QuicSession* session);
  static QuicSession::DynamicStreamMap& dynamic_streams(QuicSession* session);
  static const QuicSession::ClosedStreams& closed_streams(QuicSession* session);
  static QuicSession::ZombieStreamMap& zombie_streams(QuicSession* session);
  static QuicUnorderedSet<QuicStreamId>* GetDrainingStreams(
      QuicSession* session);
  static void ActivateStream(QuicSession* session,
                             std::unique_ptr<QuicStream> stream);

  // Discern the state of a stream.  Exactly one of these should be true at a
  // time for any stream id > 0 (other than the special streams 1 and 3).
  static bool IsStreamClosed(QuicSession* session, QuicStreamId id);
  static bool IsStreamCreated(QuicSession* session, QuicStreamId id);
  static bool IsStreamAvailable(QuicSession* session, QuicStreamId id);
  static bool IsStreamUncreated(QuicSession* session, QuicStreamId id);

  static QuicStream* GetStream(QuicSession* session, QuicStreamId id);
  static bool IsStreamWriteBlocked(QuicSession* session, QuicStreamId id);

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicSessionPeer);
};

}  // namespace test

}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_QUIC_SESSION_PEER_H_
