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

#ifndef GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_FRAME_FORMATTER_H_
#define GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_FRAME_FORMATTER_H_

#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"
#include "util/task/statusor.h"

namespace gfe_quic {
namespace test {

// Primary entry point, for formatting of whole files.
util::StatusOr<QuicString> FormatFramesInFileContents(
    QuicStringPiece file_contents);

// The following are exposed just for testing of the formatter.

// Support for finding character array literals.
enum CppLineType {
  kStartOfCharArray,  // Looks like: const char WORD[] = {
  kCharLiteral,       // Looks like: 0x\d\d    or   'c'   or   '\n'
  kEndOfCharArray,    // Looks like: };
  kClangFormatOff,    //
  kClangFormatOn,     //
  kFrameFormatOff,    // Suppresses this frame formatter from modifying.
  kFrameFormatOn,     //

  kWhitespaceOrLineComment,  //
  kOther,                    // Anything else.
};
CppLineType IdentifyCppLineType(QuicStringPiece line);

// Given a line starting with a character literal, possibly followed by
// others, comma separated, parses the character literals and returns the
// resulting characters/bytes.
util::StatusOr<QuicString> ParseCharLiteralsInLine(QuicStringPiece line,
                                                   bool* comma_at_end);

// Produces output like:
//   <line_prefix>0x00, 0x01, 0x00,        // Length: 256
//   <line_prefix>0x00,                    //   Type: DATA
//   <line_prefix>0x00,                    //  Flags: none
//   <line_prefix>0x00, 0x00, 0x00, 0x01,  // Stream: 1

QuicString FormatFrameHeader(QuicStringPiece encoded_frame,
                             QuicStringPiece line_prefix);

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_FRAME_FORMATTER_H_
