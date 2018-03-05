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

#ifndef GFE_QUIC_HTTP_TEST_TOOLS_QUIC_HTTP_FRAME_PARTS_COLLECTOR_H_
#define GFE_QUIC_HTTP_TEST_TOOLS_QUIC_HTTP_FRAME_PARTS_COLLECTOR_H_

// QuicHttpFramePartsCollector is a base class for QuicHttpFrameDecoderListener
// implementations that create one QuicHttpFrameParts instance for each decoded
// frame.

#include <stddef.h>
#include <memory>
#include <vector>

#include "common/quic/http/decoder/quic_http_frame_decoder_listener.h"
#include "test/common/quic/http/decoder/quic_http_frame_decoder_listener_test_util.h"
#include "common/quic/http/quic_http_structures.h"
#include "test/common/quic/http/test_tools/quic_http_frame_parts.h"

namespace gfe_quic {
namespace test {

class QuicHttpFramePartsCollector : public FailingQuicHttpFrameDecoderListener {
 public:
  // Toss out the collected data.
  void Reset();

  // Returns true if has started recording the info for a frame and has not yet
  // finished doing so.
  bool IsInProgress() const { return current_frame_ != nullptr; }

  // Returns the QuicHttpFrameParts instance into which we're currently
  // recording callback info if IsInProgress, else nullptr.
  const QuicHttpFrameParts* current_frame() const {
    return current_frame_.get();
  }

  // Returns the number of completely collected QuicHttpFrameParts instances.
  size_t size() const { return collected_frames_.size(); }

  // Returns the n'th frame, where 0 is the oldest of the collected frames,
  // and n==size() is the frame currently being collected, if there is one.
  // Returns nullptr if the requested index is not valid.
  const QuicHttpFrameParts* frame(size_t n) const;

 protected:
  // In support of OnFrameHeader, set the header that we expect to be used in
  // the next call.
  // TODO: Remove ExpectFrameHeader et al. once done with supporting
  // SpdyFramer's exact states.
  void ExpectFrameHeader(const QuicHttpFrameHeader& header);

  // For use in implementing On*Start methods of QuicHttpFrameDecoderListener,
  // returns a QuicHttpFrameParts instance, which will be newly created if
  // IsInProgress==false (which the caller should ensure), else will be the
  // current_frame(); never returns nullptr.
  // If called when IsInProgress==true, a test failure will be recorded.
  QuicHttpFrameDecoderListener* StartFrame(const QuicHttpFrameHeader& header);

  // For use in implementing On* callbacks, such as OnPingAck, that are the only
  // call expected for the frame being decoded; not for On*Start methods.
  // Returns a QuicHttpFrameParts instance, which will be newly created if
  // IsInProgress==false (which the caller should ensure), else will be the
  // current_frame(); never returns nullptr.
  // If called when IsInProgress==true, a test failure will be recorded.
  QuicHttpFrameDecoderListener* StartAndEndFrame(
      const QuicHttpFrameHeader& header);

  // If IsInProgress==true, returns the QuicHttpFrameParts into which the
  // current frame is being recorded; else records a test failure and returns
  // failing_listener_, which will record a test failure when any of its
  // On* methods is called.
  QuicHttpFrameDecoderListener* CurrentFrame();

  // For use in implementing On*End methods, pushes the current frame onto
  // the vector of completed frames, and returns a pointer to it for recording
  // the info in the final call. If IsInProgress==false, records a test failure
  // and returns failing_listener_, which will record a test failure when any
  // of its On* methods is called.
  QuicHttpFrameDecoderListener* EndFrame();

  // For use in implementing OnPaddingTooLong and OnFrameSizeError, is
  // equivalent to EndFrame() if IsInProgress==true, else equivalent to
  // StartAndEndFrame().
  QuicHttpFrameDecoderListener* FrameError(const QuicHttpFrameHeader& header);

 private:
  // Returns the mutable QuicHttpFrameParts instance into which we're currently
  // recording callback info if IsInProgress, else nullptr.
  QuicHttpFrameParts* current_frame() { return current_frame_.get(); }

  // If expected header is set, verify that it matches the header param.
  // TODO: Remove TestExpectedHeader et al. once done
  // with supporting SpdyFramer's exact states.
  void TestExpectedHeader(const QuicHttpFrameHeader& header);

  std::unique_ptr<QuicHttpFrameParts> current_frame_;
  std::vector<std::unique_ptr<QuicHttpFrameParts>> collected_frames_;
  FailingQuicHttpFrameDecoderListener failing_listener_;

  // TODO: Remove expected_header_ et al. once done with supporting
  // SpdyFramer's exact states.
  QuicHttpFrameHeader expected_header_;
  bool expected_header_set_ = false;
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_TEST_TOOLS_QUIC_HTTP_FRAME_PARTS_COLLECTOR_H_
