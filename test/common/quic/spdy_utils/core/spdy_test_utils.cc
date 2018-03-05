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

#include "test/common/quic/spdy_utils/core/spdy_test_utils.h"

#include <arpa/inet.h>

#include <algorithm>
#include <cstring>
#include <memory>
#include <utility>

#include "base/logging.h"
#include "strings/strcat.h"
#include "testing/base/public/gunit.h"

namespace gfe_spdy {
namespace test {

SpdyString HexDumpWithMarks(const unsigned char* data,
                            int length,
                            const bool* marks,
                            int mark_length) {
  static const char kHexChars[] = "0123456789abcdef";
  static const int kColumns = 4;

  const int kSizeLimit = 1024;
  if (length > kSizeLimit || mark_length > kSizeLimit) {
    LOG(ERROR) << "Only dumping first " << kSizeLimit << " bytes.";
    length = std::min(length, kSizeLimit);
    mark_length = std::min(mark_length, kSizeLimit);
  }

  SpdyString hex;
  for (const unsigned char *row = data; length > 0;
       row += kColumns, length -= kColumns) {
    for (const unsigned char* p = row; p < row + 4; ++p) {
      if (p < row + length) {
        const bool mark =
            (marks && (p - data) < mark_length && marks[p - data]);
        hex += mark ? '*' : ' ';
        hex += kHexChars[(*p & 0xf0) >> 4];
        hex += kHexChars[*p & 0x0f];
        hex += mark ? '*' : ' ';
      } else {
        hex += "    ";
      }
    }
    hex = hex + "  ";

    for (const unsigned char* p = row; p < row + 4 && p < row + length; ++p) {
      hex += (*p >= 0x20 && *p <= 0x7f) ? (*p) : '.';
    }

    hex = hex + '\n';
  }
  return hex;
}

void CompareCharArraysWithHexError(const SpdyString& description,
                                   const unsigned char* actual,
                                   const int actual_len,
                                   const unsigned char* expected,
                                   const int expected_len) {
  const int min_len = std::min(actual_len, expected_len);
  const int max_len = std::max(actual_len, expected_len);
  std::unique_ptr<bool[]> marks(new bool[max_len]);
  bool identical = (actual_len == expected_len);
  for (int i = 0; i < min_len; ++i) {
    if (actual[i] != expected[i]) {
      marks[i] = true;
      identical = false;
    } else {
      marks[i] = false;
    }
  }
  for (int i = min_len; i < max_len; ++i) {
    marks[i] = true;
  }
  if (identical)
    return;
  ADD_FAILURE() << "Description:\n"
                << description << "\n\nExpected:\n"
                << HexDumpWithMarks(expected, expected_len, marks.get(),
                                    max_len)
                << "\nActual:\n"
                << HexDumpWithMarks(actual, actual_len, marks.get(), max_len);
}

void SetFrameFlags(SpdySerializedFrame* frame, uint8_t flags) {
  frame->data()[4] = flags;
}

void SetFrameLength(SpdySerializedFrame* frame, size_t length) {
  CHECK_GT(1 << 14, length);
  {
    int32_t wire_length = htonl(length);
    memcpy(frame->data(), reinterpret_cast<char*>(&wire_length) + 1, 3);
  }
}

void TestHeadersHandler::OnHeaderBlockStart() {
  block_.clear();
}

void TestHeadersHandler::OnHeader(SpdyStringPiece name, SpdyStringPiece value) {
  block_.AppendValueOrAddHeader(name, value);
}

void TestHeadersHandler::OnHeaderBlockEnd(
    size_t header_bytes_parsed,
    size_t compressed_header_bytes_parsed) {
  header_bytes_parsed_ = header_bytes_parsed;
  compressed_header_bytes_parsed_ = compressed_header_bytes_parsed;
}

}  // namespace test
}  // namespace gfe_spdy
