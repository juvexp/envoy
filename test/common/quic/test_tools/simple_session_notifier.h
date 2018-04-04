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

#ifndef GFE_QUIC_TEST_TOOLS_SIMPLE_SESSION_NOTIFIER_H_
#define GFE_QUIC_TEST_TOOLS_SIMPLE_SESSION_NOTIFIER_H_

#include "common/quic/core/session_notifier_interface.h"
#include "gmock/gmock.h"

namespace gfe_quic {

class QuicConnection;

namespace test {

// SimpleSessionNotifier implements the basic functionalities of a session, and
// it manages stream data and control frames.
class SimpleSessionNotifier : public SessionNotifierInterface {
 public:
  explicit SimpleSessionNotifier(QuicConnection* connection);
  ~SimpleSessionNotifier() override;

  // Tries to write stream data and returns data consumed.
  QuicConsumedData WriteOrBufferData(QuicStreamId id,
                                     QuicByteCount data_length,
                                     StreamSendingState state);

  // Tries to write RST_STREAM_FRAME.
  void WriteOrBufferRstStream(QuicStreamId id,
                              QuicRstStreamErrorCode error,
                              QuicStreamOffset bytes_written);

  // Neuters unencrypted data of crypto stream.
  void NeuterUnencryptedData();

  // Called when connection_ becomes writable.
  void OnCanWrite();

  // Returns true if there are 1) unsent control frames and stream data, or 2)
  // lost control frames and stream data.
  bool WillingToWrite() const;

  // Number of sent stream bytes. Please note, this does not count
  // retransmissions.
  QuicByteCount StreamBytesSent() const;

  // Number of stream bytes waiting to be sent for the first time.
  QuicByteCount StreamBytesToSend() const;

  // Returns true if there is any stream data waiting to be sent for the first
  // time.
  bool HasBufferedStreamData() const;

  // Returns true if stream |id| has any outstanding data.
  bool StreamIsWaitingForAcks(QuicStreamId id) const;

  // SessionNotifierInterface methods:
  bool OnFrameAcked(const QuicFrame& frame,
                    QuicTime::Delta ack_delay_time) override;
  void OnStreamFrameRetransmitted(const QuicStreamFrame& frame) override {}
  void OnFrameLost(const QuicFrame& frame) override;
  void RetransmitFrames(const QuicFrames& frames,
                        TransmissionType type) override;
  bool IsFrameOutstanding(const QuicFrame& frame) const override;
  bool HasPendingCryptoData() const override;

 private:
  struct StreamState {
    StreamState();
    ~StreamState();

    // Total number of bytes.
    QuicByteCount bytes_total;
    // Number of sent bytes.
    QuicByteCount bytes_sent;
    // Record of acked offsets.
    QuicIntervalSet<QuicStreamOffset> bytes_acked;
    // Data considered as lost and needs to be retransmitted.
    QuicIntervalSet<QuicStreamOffset> pending_retransmissions;

    bool fin_buffered;
    bool fin_sent;
    bool fin_outstanding;
    bool fin_lost;
  };

  friend std::ostream& operator<<(std::ostream& os, const StreamState& s);

  using StreamMap = QuicUnorderedMap<QuicStreamId, StreamState>;

  void OnStreamDataConsumed(QuicStreamId id,
                            QuicStreamOffset offset,
                            QuicByteCount data_length,
                            bool fin);

  bool OnControlFrameAcked(const QuicFrame& frame);

  void OnControlFrameLost(const QuicFrame& frame);

  bool RetransmitLostControlFrames();

  bool RetransmitLostCryptoData();

  bool RetransmitLostStreamData();

  bool WriteBufferedControlFrames();

  bool IsControlFrameOutstanding(const QuicFrame& frame) const;

  bool HasBufferedControlFrames() const;

  bool HasLostStreamData() const;

  bool StreamHasBufferedData(QuicStreamId id) const;

  QuicDeque<QuicFrame> control_frames_;

  QuicLinkedHashMap<QuicControlFrameId, bool> lost_control_frames_;

  // Id of latest saved control frame. 0 if no control frame has been saved.
  QuicControlFrameId last_control_frame_id_;

  // The control frame at the 0th index of control_frames_.
  QuicControlFrameId least_unacked_;

  // ID of the least unsent control frame.
  QuicControlFrameId least_unsent_;

  StreamMap stream_map_;

  // Transferred crypto bytes according to encryption levels.
  QuicIntervalSet<QuicStreamOffset>
      crypto_bytes_transferred_[NUM_ENCRYPTION_LEVELS];

  QuicConnection* connection_;
};

}  // namespace test

}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_SIMPLE_SESSION_NOTIFIER_H_
