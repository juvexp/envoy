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

#ifndef GFE_SPDY_CORE_SPDY_TEST_UTILS_H_
#define GFE_SPDY_CORE_SPDY_TEST_UTILS_H_

#include <cstdint>

#include "gfe/gfe2/test_tools/failure.h"
#include "common/quic/spdy_utils/core/spdy_bug_tracker.h"
#include "common/quic/spdy_utils/core/spdy_header_block.h"
#include "common/quic/spdy_utils/core/spdy_headers_handler_interface.h"
#include "common/quic/spdy_utils/core/spdy_protocol.h"
#include "common/quic/spdy_utils/platform/api/spdy_string.h"
#include "common/quic/spdy_utils/platform/api/spdy_string_piece.h"

// EXPECT_SPDY_BUG is like EXPECT_DFATAL, except it ensures that no DFATAL
// logging is skipped due to exponential backoff.
//
// For external SPDY, EXPECT_SPDY_BUG should be #defined to EXPECT_DFATAL.
#define EXPECT_SPDY_BUG EXPECT_GFE_BUG

namespace gfe_spdy {

inline bool operator==(SpdyStringPiece x,
                       const SpdyHeaderBlock::ValueProxy& y) {
  return x == y.as_string();
}

namespace test {

SpdyString HexDumpWithMarks(const unsigned char* data,
                            int length,
                            const bool* marks,
                            int mark_length);

void CompareCharArraysWithHexError(const SpdyString& description,
                                   const unsigned char* actual,
                                   const int actual_len,
                                   const unsigned char* expected,
                                   const int expected_len);

void SetFrameFlags(SpdySerializedFrame* frame, uint8_t flags);

void SetFrameLength(SpdySerializedFrame* frame, size_t length);

// A test implementation of SpdyHeadersHandlerInterface that correctly
// reconstructs multiple header values for the same name.
class TestHeadersHandler : public SpdyHeadersHandlerInterface {
 public:
  TestHeadersHandler() {}

  void OnHeaderBlockStart() override;

  void OnHeader(SpdyStringPiece name, SpdyStringPiece value) override;

  void OnHeaderBlockEnd(size_t header_bytes_parsed,
                        size_t compressed_header_bytes_parsed) override;

  const SpdyHeaderBlock& decoded_block() const { return block_; }
  size_t header_bytes_parsed() const { return header_bytes_parsed_; }
  size_t compressed_header_bytes_parsed() const {
    return compressed_header_bytes_parsed_;
  }

 private:
  SpdyHeaderBlock block_;
  size_t header_bytes_parsed_ = 0;
  size_t compressed_header_bytes_parsed_ = 0;

  DISALLOW_COPY_AND_ASSIGN(TestHeadersHandler);
};

}  // namespace test
}  // namespace gfe_spdy

#endif  // GFE_SPDY_CORE_SPDY_TEST_UTILS_H_
