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

#include "test/common/quic/http/test_tools/quic_http_frame_parts_collector.h"

#include <utility>

#include "base/logging.h"
#include "test/common/quic/http/quic_http_structures_test_util.h"
#include "common/quic/platform/api/quic_ptr_util.h"
#include "gtest/gtest.h"

namespace gfe_quic {
namespace test {

void QuicHttpFramePartsCollector::Reset() {
  current_frame_.reset();
  collected_frames_.clear();
  expected_header_set_ = false;
}

const QuicHttpFrameParts* QuicHttpFramePartsCollector::frame(size_t n) const {
  if (n < size()) {
    return collected_frames_.at(n).get();
  }
  CHECK(n == size());
  return current_frame();
}

void QuicHttpFramePartsCollector::ExpectFrameHeader(
    const QuicHttpFrameHeader& header) {
  EXPECT_FALSE(IsInProgress());
  EXPECT_FALSE(expected_header_set_)
      << "expected_header_: " << expected_header_;
  expected_header_ = header;
  expected_header_set_ = true;
  // OnFrameHeader is called before the flags are scrubbed, but the other
  // methods are called after, so scrub the invalid flags from expected_header_.
  ScrubFlagsOfHeader(&expected_header_);
}

void QuicHttpFramePartsCollector::TestExpectedHeader(
    const QuicHttpFrameHeader& header) {
  if (expected_header_set_) {
    EXPECT_EQ(header, expected_header_);
    expected_header_set_ = false;
  }
}

QuicHttpFrameDecoderListener* QuicHttpFramePartsCollector::StartFrame(
    const QuicHttpFrameHeader& header) {
  TestExpectedHeader(header);
  EXPECT_FALSE(IsInProgress());
  if (current_frame_ == nullptr) {
    current_frame_ = QuicMakeUnique<QuicHttpFrameParts>(header);
  }
  return current_frame();
}

QuicHttpFrameDecoderListener* QuicHttpFramePartsCollector::StartAndEndFrame(
    const QuicHttpFrameHeader& header) {
  TestExpectedHeader(header);
  EXPECT_FALSE(IsInProgress());
  if (current_frame_ == nullptr) {
    current_frame_ = QuicMakeUnique<QuicHttpFrameParts>(header);
  }
  QuicHttpFrameDecoderListener* result = current_frame();
  collected_frames_.push_back(std::move(current_frame_));
  return result;
}

QuicHttpFrameDecoderListener* QuicHttpFramePartsCollector::CurrentFrame() {
  EXPECT_TRUE(IsInProgress());
  if (current_frame_ == nullptr) {
    return &failing_listener_;
  }
  return current_frame();
}

QuicHttpFrameDecoderListener* QuicHttpFramePartsCollector::EndFrame() {
  EXPECT_TRUE(IsInProgress());
  if (current_frame_ == nullptr) {
    return &failing_listener_;
  }
  QuicHttpFrameDecoderListener* result = current_frame();
  collected_frames_.push_back(std::move(current_frame_));
  return result;
}

QuicHttpFrameDecoderListener* QuicHttpFramePartsCollector::FrameError(
    const QuicHttpFrameHeader& header) {
  TestExpectedHeader(header);
  if (current_frame_ == nullptr) {
    // The decoder may detect an error before making any calls to the listener
    // regarding the frame, in which case current_frame_==nullptr and we need
    // to create a QuicHttpFrameParts instance.
    current_frame_ = QuicMakeUnique<QuicHttpFrameParts>(header);
  } else {
    // Similarly, the decoder may have made calls to the listener regarding the
    // frame before detecting the error; for example, the DATA payload decoder
    // calls OnDataStart before it can detect padding errors, hence before it
    // can call OnPaddingTooLong.
    EXPECT_EQ(header, current_frame_->frame_header);
  }
  QuicHttpFrameDecoderListener* result = current_frame();
  collected_frames_.push_back(std::move(current_frame_));
  return result;
}

}  // namespace test
}  // namespace gfe_quic
