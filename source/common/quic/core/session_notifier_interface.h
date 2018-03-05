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

#ifndef GFE_QUIC_CORE_SESSION_NOTIFIER_INTERFACE_H_
#define GFE_QUIC_CORE_SESSION_NOTIFIER_INTERFACE_H_

#include "common/quic/core/frames/quic_frame.h"
#include "common/quic/core/quic_time.h"

namespace gfe_quic {

// Pure virtual class to be notified when a packet containing a frame is acked
// or lost.
class QUIC_EXPORT_PRIVATE SessionNotifierInterface {
 public:
  virtual ~SessionNotifierInterface() {}

  // Called when |frame| is acked. Returns true if any new data gets acked,
  // returns false otherwise.
  virtual bool OnFrameAcked(const QuicFrame& frame,
                            QuicTime::Delta ack_delay_time) = 0;

  // Called when |frame| is retransmitted.
  virtual void OnStreamFrameRetransmitted(const QuicStreamFrame& frame) = 0;

  // Called when |frame| is considered as lost.
  virtual void OnFrameLost(const QuicFrame& frame) = 0;

  // Called to retransmit |frames| with transmission |type|.
  virtual void RetransmitFrames(const QuicFrames& frames,
                                TransmissionType type) = 0;

  // Returns true if |frame| is outstanding and waiting to be acked.
  virtual bool IsFrameOutstanding(const QuicFrame& frame) const = 0;

  // Returns true if crypto stream is waiting for acks.
  virtual bool HasPendingCryptoData() const = 0;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_SESSION_NOTIFIER_INTERFACE_H_
