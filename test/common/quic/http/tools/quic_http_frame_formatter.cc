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

#include "test/common/quic/http/tools/quic_http_frame_formatter.h"

#include <stddef.h>

#include <algorithm>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "base/logging.h"
#include "common/quic/http/decoder/quic_http_decode_buffer.h"
#include "common/quic/http/decoder/quic_http_decode_structures.h"
#include "common/quic/http/quic_http_constants.h"
#include "common/quic/http/quic_http_structures.h"
#include "test/common/quic/http/quic_http_structures_test_util.h"
#include "common/quic/platform/api/quic_str_cat.h"
#include "common/quic/platform/api/quic_string_utils.h"
#include "strings/ascii_ctype.h"
#include "strings/escaping.h"
#include "strings/join.h"
#include "strings/numbers.h"
#include "strings/split.h"
#include "strings/stringpiece_utils.h"
#include "strings/strip.h"
#include "third_party/absl/strings/strip.h"
#include "util/regexp/re2/re2.h"
#include "util/task/canonical_errors.h"
#include "util/task/status.h"  // IWYU pragma: keep  // Falsely removes.
#include "util/task/status_macros.h"

using ::util::FailedPreconditionError;
using ::util::Status;
using ::util::StatusOr;

namespace gfe_quic {
namespace test {
namespace {
const char kLiteralClangFormatOff[] = "// clang-format off";
const char kLiteralClangFormatOn[] = "// clang-format on";

size_t CountLeadingDigits(QuicStringPiece text) {
  stringpiece_ssize_type count = 0;
  const char* ptr = text.data();
  while (count < text.size() && ascii_isdigit(*ptr)) {
    count++;
    ptr++;
  }
  return count;
}

size_t CountLeadingHexDigits(QuicStringPiece text) {
  stringpiece_ssize_type count = 0;
  const char* ptr = text.data();
  while (count < text.size() && ascii_isxdigit(*ptr)) {
    count++;
    ptr++;
  }
  return count;
}

size_t CountLeadingBlanks(QuicStringPiece text) {
  stringpiece_ssize_type count = 0;
  const char* ptr = text.data();
  while (count < text.size() && ascii_isblank(*ptr)) {
    count++;
    ptr++;
  }
  return count;
}

QuicStringPiece SkipLeadingBlanks(QuicStringPiece text) {
  text.remove_prefix(CountLeadingBlanks(text));
  return text;
}

QuicStringPiece GetLeadingBlanks(QuicStringPiece text) {
  return text.substr(0, CountLeadingBlanks(text));
}

QuicStringPiece StripTrailingWhitespace(QuicStringPiece text) {
  stringpiece_ssize_type count = 0;
  const char* ptr = text.data() + text.size() - 1;
  while (count < text.size() && ascii_isspace(*ptr)) {
    ++count;
    --ptr;
  }
  text.remove_suffix(count);
  return text;
}

bool SameExceptForTrailingWhiteSpace(QuicStringPiece a, QuicStringPiece b) {
  a = StripTrailingWhitespace(a);
  b = StripTrailingWhitespace(b);
  return a == b;
}

// If a big number that is 2^N-1, for some N, print in hex.
QuicString FormatInteger(QuicStringPiece label, uint32_t stream_id) {
  if (stream_id > 1024 && (stream_id & (stream_id + 1)) == 0) {
    return QuicStrCat(label, "0x", strings::Hex(stream_id));
  } else {
    return QuicStrCat(label, stream_id);
  }
}

}  // namespace

CppLineType IdentifyCppLineType(QuicStringPiece line) {
  line = SkipLeadingBlanks(line);
  if (line.empty() || strings::StartsWith(line, "\n")) {
    return kWhitespaceOrLineComment;
  }
  if (absl::ConsumePrefix(&line, "//")) {
    if (absl::ConsumePrefix(&line, " clang-format ")) {
      if (absl::ConsumePrefix(&line, "off")) {
        return kClangFormatOff;
      } else if (absl::ConsumePrefix(&line, "on")) {
        return kClangFormatOn;
      }
    } else if (absl::ConsumePrefix(&line, " frame-format ")) {
      if (absl::ConsumePrefix(&line, "off")) {
        return kFrameFormatOff;
      } else if (absl::ConsumePrefix(&line, "on")) {
        return kFrameFormatOn;
      }
    }
    return kWhitespaceOrLineComment;
  }
  const RE2 starts_with_character_literal("^['0-9].*");
  if (RE2::PartialMatch(line, starts_with_character_literal)) {
    VLOG(3) << "kCharLiteral: " << line;
    return kCharLiteral;
  }
  const RE2 starts_with_decl(
      R"#(^(const\s+)?(unsigned\s+)?char\s+\w+\s*\[\s*\])#"
      R"#(\s*=\s*\{\s*(//.*)?$)#");
  if (RE2::PartialMatch(line, starts_with_decl)) {
    return kStartOfCharArray;
  }
  const RE2 starts_with_decl_end(
      R"#(^}\s*;)#");
  if (RE2::PartialMatch(line, starts_with_decl_end)) {
    return kEndOfCharArray;
  }
  return kOther;
}

util::StatusOr<QuicString> ParseCharLiteralsInLine(QuicStringPiece line,
                                                   bool* comma_at_end) {
  CHECK_NOTNULL(comma_at_end);
  QuicString result;
  line = SkipLeadingBlanks(line);
  while (!line.empty()) {
    if (strings::StartsWith(line, "//")) {
      break;
    }
    QuicStringPiece literal;
    int32_t value = -1;
    if (strings::StartsWith(line, "'")) {
      if (line[1] != '\\' && line[2] == '\'') {
        value = line[1];
        literal = line.substr(0, 3);
      } else if (line[1] == '\\') {
        // Find the single quote that terminates the character literal;
        // because of the leading backslash in the character literal, we start
        // searching at the fourth character.
        auto pos = line.find('\'', 3);
        if (pos == QuicStringPiece::npos) {
          return FailedPreconditionError(
              QuicStrCat("Unable to locate end of escaped character literal: ",
                         line.substr(0, 10)));
        }
        QuicString dest;
        QuicString error;
        if (!absl::CUnescape(absl::ClippedSubstr(line, 1, pos - 1), &dest,
                             &error)) {
          return FailedPreconditionError(error);
        }
        if (dest.size() != 1) {
          return FailedPreconditionError(
              QuicStrCat("Expected dest.size()==1, not ", dest.size()));
        }
        value = dest[0];
        literal = line.substr(0, pos + 1);
      } else {
        return FailedPreconditionError(QuicStrCat(
            "Unable to parse char literal starting at: ", line.substr(0, 10)));
      }
    } else if (ascii_isdigit(line[0])) {
      size_t num_digits;
      if (strings::StartsWith(line, "0x")) {
        size_t num_xdigits =
            CountLeadingHexDigits(absl::ClippedSubstr(line, 2));
        if (num_xdigits > 0) {
          num_digits = 2 + num_xdigits;
        } else {
          num_digits = 0;
        }
      } else {
        num_digits = CountLeadingDigits(line);
      }
      if (num_digits == 0) {
        return FailedPreconditionError(QuicStrCat(
            "Unable to parse char literal starting at: ", line.substr(0, 10)));
      }
      literal = line.substr(0, num_digits);
      if (!strings::safe_strto32_base(literal, &value, 0)) {
        return util::InternalError(
            QuicStrCat("Unable to parse \"", literal,
                       "\"; error code or possible value=", value));
      }
    } else if (strings::StartsWith(line, "kRstStreamStatusTooLow")) {
      literal = "kRstStreamStatusTooLow";
      value = 0;
    } else if (strings::StartsWith(line, "kRstStreamStatusTooHigh")) {
      literal = "kRstStreamStatusTooHigh";
      value = 255;
    } else {
      size_t num_digits;
      if (strings::StartsWith(line, "0x")) {
        size_t num_xdigits =
            CountLeadingHexDigits(absl::ClippedSubstr(line, 2));
        if (num_xdigits > 0) {
          num_digits = 2 + num_xdigits;
        } else {
          num_digits = 0;
        }
      } else {
        num_digits = CountLeadingDigits(line);
      }
      if (num_digits == 0) {
        return FailedPreconditionError(QuicStrCat(
            "Unable to parse char literal starting at: ", line.substr(0, 10)));
      }
      literal = line.substr(0, num_digits);
      if (!strings::safe_strto32_base(literal, &value, 0)) {
        return util::InternalError(
            QuicStrCat("Unable to parse \"", literal,
                       "\"; error code or possible value=", value));
      }
    }

    if (0 <= value && value < 256) {
      result.push_back(static_cast<char>(value));
    } else {
      return util::OutOfRangeError(QuicStrCat("Converted literal \"", literal,
                                              "\" to ", value,
                                              ", which is out of range."));
    }
    line.remove_prefix(literal.size());

    // Skip blanks, comma and then more blanks.
    line = SkipLeadingBlanks(line);
    if (strings::StartsWith(line, ",")) {
      *comma_at_end = true;  // Not fully determined yet, but looking good.
      line.remove_prefix(1);
      line = SkipLeadingBlanks(line);
    } else if (line.empty() || strings::StartsWith(line, "//")) {
      *comma_at_end = false;
      break;
    } else {
      return FailedPreconditionError(
          QuicStrCat("Expected literals on line to end with comma, not: ",
                     line.substr(0, 10)));
    }
  }
  return result;
}

namespace {

typedef std::vector<std::pair<QuicString, QuicString>> LineParts;

QuicString FormatByte(uint8_t v) {
  return QuicStringPrintf("0x%02x", v);
}

QuicString FormatBytes(QuicStringPiece s) {
  QuicString result;
  for (char c : s) {
    if (!result.empty()) {
      result.push_back(' ');
    }
    result.append(FormatByte(static_cast<uint8_t>(c)));
    result.push_back(',');
  }
  return result;
}

QuicString AssembleParts(const LineParts& line_parts,
                         QuicStringPiece line_prefix) {
  size_t max_len = 0;
  for (auto& lp : line_parts) {
    max_len = std::max(max_len, lp.first.size());
  }
  QuicString result;
  result.reserve(40 * line_parts.size());
  for (auto& lp : line_parts) {
    const auto& bytes = lp.first;
    const auto& comment = lp.second;
    if (bytes.empty() && comment.empty()) {
      result.push_back('\n');
      continue;
    }
    QuicStrAppend(&result, line_prefix);
    result.append(bytes);
    // Always add comment, even if blank, so that we avoid clang-fromat.
    size_t padding_len = max_len + 2 - lp.first.size();
    result.append(padding_len, ' ');
    result.append("//");
    if (!comment.empty()) {
      // Has a comment.
      result.append(" ");
      result.append(comment);
    }
    result.push_back('\n');
  }
  return result;
}

void GenerateFrameHeaderLineParts(const QuicHttpFrameHeader& header,
                                  QuicStringPiece encoded_header,
                                  LineParts* line_parts) {
  CHECK_EQ(encoded_header.size(), QuicHttpFrameHeader::EncodedSize());

  {
    QuicString bytes = FormatBytes(encoded_header.substr(0, 3));
    QuicString comment = FormatInteger("Length: ", header.payload_length);
    line_parts->push_back({bytes, comment});
  }

  {
    QuicString bytes = FormatBytes(absl::ClippedSubstr(encoded_header, 3, 1));
    QuicString comment =
        QuicStrCat("  Type: ", QuicHttpFrameTypeToString(header.type));
    line_parts->push_back({bytes, comment});
  }

  {
    QuicString bytes = FormatBytes(absl::ClippedSubstr(encoded_header, 4, 1));
    QuicString comment = " Flags: ";
    if (header.flags == 0) {
      comment.append("none");
    } else {
      comment.append(QuicHttpFrameFlagsToString(header.type, header.flags));
    }
    line_parts->push_back({bytes, comment});
  }

  {
    QuicString bytes = FormatBytes(absl::ClippedSubstr(encoded_header, 5, 4));
    QuicString comment = FormatInteger("Stream: ", header.stream_id);
    if (0x80 == (encoded_header[5] & 0x80)) {
      comment.append(" (R-bit set)");
    }
    line_parts->push_back({bytes, comment});
  }
}

QuicHttpFrameHeader GenerateFrameHeaderLineParts(QuicStringPiece encoded_frame,
                                                 LineParts* line_parts) {
  CHECK_GE(encoded_frame.size(), QuicHttpFrameHeader::EncodedSize());
  QuicStringPiece encoded_header =
      encoded_frame.substr(0, QuicHttpFrameHeader::EncodedSize());
  QuicHttpDecodeBuffer b(encoded_header);
  QuicHttpFrameHeader header;
  DoDecode(&header, &b);
  GenerateFrameHeaderLineParts(header, encoded_header, line_parts);
  return header;
}

void GenerateMiscPayloadLineParts(QuicStringPiece label,
                                  bool repeat_label,
                                  QuicStringPiece encoded_payload,
                                  LineParts* line_parts) {
  while (!encoded_payload.empty()) {
    QuicStringPiece fragment = encoded_payload.substr(0, 4);
    QuicString bytes = FormatBytes(fragment);
    QuicString comment;
    comment.assign(label.data(), label.size());
    if (!repeat_label) {
      label = QuicStringPiece();
    }
    line_parts->push_back({bytes, comment});
    encoded_payload.remove_prefix(fragment.size());
  }
}

void GenerateTruncatedPayload(QuicStringPiece name,
                              QuicStringPiece truncated_payload,
                              LineParts* line_parts) {
  while (!truncated_payload.empty()) {
    QuicStringPiece fragment = truncated_payload.substr(0, 4);
    QuicString bytes = FormatBytes(fragment);
    QuicString comment;
    if (!name.empty()) {
      comment.assign(name.data(), name.size());
      comment += " (Truncated)";
      name = QuicStringPiece();
    }
    line_parts->push_back({bytes, comment});
    truncated_payload.remove_prefix(fragment.size());
  }
}

QuicStringPiece GeneratePriorityPayloadLineParts(
    const QuicHttpFrameHeader& header,
    QuicStringPiece encoded_payload,
    LineParts* line_parts) {
  CHECK(FrameHasPriority(header));
  if (encoded_payload.size() < QuicHttpPriorityFields::EncodedSize()) {
    GenerateTruncatedPayload("Priority", encoded_payload, line_parts);
    return QuicStringPiece();
  }
  QuicHttpDecodeBuffer db(encoded_payload);
  QuicHttpPriorityFields fields;
  DoDecode(&fields, &db);
  {
    QuicString bytes = FormatBytes(encoded_payload.substr(0, 4));
    QuicString comment = FormatInteger("Parent: ", fields.stream_dependency);
    if (fields.is_exclusive) {
      comment.append(" (Exclusive)");
    }
    line_parts->push_back({bytes, comment});
  }
  {
    QuicString bytes = FormatBytes(absl::ClippedSubstr(encoded_payload, 4, 1));
    QuicString comment = QuicStrCat("Weight: ", fields.weight);
    line_parts->push_back({bytes, comment});
  }
  encoded_payload.remove_prefix(QuicHttpPriorityFields::EncodedSize());
  return encoded_payload;
}

QuicStringPiece GenerateRstStreamPayloadLineParts(
    const QuicHttpFrameHeader& header,
    QuicStringPiece encoded_payload,
    LineParts* line_parts) {
  if (encoded_payload.size() < QuicHttpRstStreamFields::EncodedSize()) {
    GenerateTruncatedPayload("RstStream", encoded_payload, line_parts);
    return QuicStringPiece();
  }
  QuicHttpDecodeBuffer db(encoded_payload);
  QuicHttpRstStreamFields fields;
  DoDecode(&fields, &db);
  {
    QuicString bytes = FormatBytes(encoded_payload.substr(0, 4));
    QuicString comment;
    if (IsSupportedQuicHttpErrorCode(fields.error_code)) {
      comment =
          QuicStrCat(" Error: ", QuicHttpErrorCodeToString(fields.error_code));
    } else {
      comment = FormatInteger(" Error: ", static_cast<int>(fields.error_code));
    }
    line_parts->push_back({bytes, comment});
  }
  encoded_payload.remove_prefix(QuicHttpRstStreamFields::EncodedSize());
  return encoded_payload;
}

QuicStringPiece GeneratePushPromisePayloadLineParts(
    const QuicHttpFrameHeader& header,
    QuicStringPiece encoded_payload,
    LineParts* line_parts) {
  if (encoded_payload.size() < QuicHttpPushPromiseFields::EncodedSize()) {
    GenerateTruncatedPayload("PushPromise", encoded_payload, line_parts);
    return QuicStringPiece();
  }
  QuicHttpDecodeBuffer db(encoded_payload);
  QuicHttpPushPromiseFields fields;
  DoDecode(&fields, &db);
  {
    QuicString bytes = FormatBytes(encoded_payload.substr(0, 4));
    QuicString comment = FormatInteger("Promise: ", fields.promised_stream_id);
    if (0x80 == (encoded_payload[0] & 0x80)) {
      comment.append(" (R-bit set)");
    }
    line_parts->push_back({bytes, comment});
  }
  encoded_payload.remove_prefix(QuicHttpPushPromiseFields::EncodedSize());
  return encoded_payload;
}

QuicStringPiece GeneratePingPayloadLineParts(const QuicHttpFrameHeader& header,
                                             QuicStringPiece encoded_payload,
                                             LineParts* line_parts) {
  if (encoded_payload.size() < QuicHttpPingFields::EncodedSize()) {
    GenerateTruncatedPayload("Ping", encoded_payload, line_parts);
    return QuicStringPiece();
  }
  CHECK_EQ(QuicHttpPingFields::EncodedSize(), 8);
  {
    QuicString bytes = FormatBytes(encoded_payload.substr(0, 4));
    QuicString comment = "Opaque";
    line_parts->push_back({bytes, comment});
  }
  {
    QuicString bytes = FormatBytes(absl::ClippedSubstr(encoded_payload, 4, 4));
    QuicString comment = "    Data";
    line_parts->push_back({bytes, comment});
  }
  encoded_payload.remove_prefix(QuicHttpPingFields::EncodedSize());
  return encoded_payload;
}

QuicStringPiece GenerateGoAwayPayloadLineParts(
    const QuicHttpFrameHeader& header,
    QuicStringPiece encoded_payload,
    LineParts* line_parts) {
  if (encoded_payload.size() < QuicHttpGoAwayFields::EncodedSize()) {
    GenerateTruncatedPayload("GoAway", encoded_payload, line_parts);
    return QuicStringPiece();
  }
  QuicHttpDecodeBuffer db(encoded_payload);
  QuicHttpGoAwayFields fields;
  DoDecode(&fields, &db);
  {
    QuicString bytes = FormatBytes(encoded_payload.substr(0, 4));
    QuicString comment = FormatInteger("  Last: ", fields.last_stream_id);
    if (0x80 == (encoded_payload[0] & 0x80)) {
      comment.append(" (R-bit set)");
    }
    line_parts->push_back({bytes, comment});
  }
  {
    QuicString bytes = FormatBytes(absl::ClippedSubstr(encoded_payload, 4, 4));
    QuicString comment;
    if (IsSupportedQuicHttpErrorCode(fields.error_code)) {
      comment =
          QuicStrCat(" Error: ", QuicHttpErrorCodeToString(fields.error_code));
    } else {
      comment = FormatInteger(" Error: ", static_cast<int>(fields.error_code));
    }
    line_parts->push_back({bytes, comment});
  }
  encoded_payload.remove_prefix(QuicHttpGoAwayFields::EncodedSize());
  GenerateMiscPayloadLineParts("Description", false, encoded_payload,
                               line_parts);
  return QuicStringPiece();
}

QuicStringPiece GenerateSettingsPayloadLineParts(
    const QuicHttpFrameHeader& header,
    QuicStringPiece encoded_payload,
    LineParts* line_parts) {
  while (encoded_payload.size() >= QuicHttpSettingFields::EncodedSize()) {
    QuicHttpDecodeBuffer db(encoded_payload);
    QuicHttpSettingFields fields;
    DoDecode(&fields, &db);
    {
      QuicString bytes = FormatBytes(encoded_payload.substr(0, 2));
      QuicString comment;
      if (IsSupportedQuicHttpSettingsParameter(fields.parameter)) {
        comment = QuicStrCat(
            " Param: ", QuicHttpSettingsParameterToString(fields.parameter));
      } else {
        comment = FormatInteger(" Param: ", static_cast<int>(fields.parameter));
      }
      line_parts->push_back({bytes, comment});
    }
    {
      QuicString bytes =
          FormatBytes(absl::ClippedSubstr(encoded_payload, 2, 4));
      QuicString comment = QuicStrCat(" Value: ", fields.value);
      line_parts->push_back({bytes, comment});
    }
    encoded_payload.remove_prefix(QuicHttpSettingFields::EncodedSize());
  }
  if (!encoded_payload.empty()) {
    GenerateTruncatedPayload("Settings", encoded_payload, line_parts);
  }
  return QuicStringPiece();
}

QuicStringPiece GenerateWindowUpdatePayloadLineParts(
    const QuicHttpFrameHeader& header,
    QuicStringPiece encoded_payload,
    LineParts* line_parts) {
  if (encoded_payload.size() < QuicHttpWindowUpdateFields::EncodedSize()) {
    GenerateTruncatedPayload("WindowUpdate", encoded_payload, line_parts);
    return QuicStringPiece();
  }
  QuicHttpDecodeBuffer db(encoded_payload);
  QuicHttpWindowUpdateFields fields;
  DoDecode(&fields, &db);
  {
    QuicString bytes = FormatBytes(encoded_payload.substr(0, 4));
    QuicString comment =
        FormatInteger("Increment: ", fields.window_size_increment);
    line_parts->push_back({bytes, comment});
  }
  encoded_payload.remove_prefix(QuicHttpWindowUpdateFields::EncodedSize());
  return encoded_payload;
}

void GenerateHpackPayloadLineParts(const QuicHttpFrameHeader& header,
                                   QuicStringPiece encoded_payload,
                                   LineParts* line_parts) {
  // TODO: Someday might implement this, but its a bit tricky
  // because we sometimes need support for CONTINUATION frames.
  GenerateMiscPayloadLineParts("HPQUIC_HTTP_ACK", false, encoded_payload,
                               line_parts);
}

void GenerateUnpaddedPayloadLineParts(const QuicHttpFrameHeader& header,
                                      QuicStringPiece encoded_payload,
                                      LineParts* line_parts) {
  if (FrameHasPriority(header)) {
    encoded_payload =
        GeneratePriorityPayloadLineParts(header, encoded_payload, line_parts);
  } else if (header.type == QuicHttpFrameType::RST_STREAM) {
    encoded_payload =
        GenerateRstStreamPayloadLineParts(header, encoded_payload, line_parts);
  } else if (header.type == QuicHttpFrameType::SETTINGS && !header.IsAck()) {
    encoded_payload =
        GenerateSettingsPayloadLineParts(header, encoded_payload, line_parts);
  } else if (header.type == QuicHttpFrameType::PUSH_PROMISE) {
    encoded_payload = GeneratePushPromisePayloadLineParts(
        header, encoded_payload, line_parts);
  } else if (header.type == QuicHttpFrameType::PING) {
    encoded_payload =
        GeneratePingPayloadLineParts(header, encoded_payload, line_parts);
  } else if (header.type == QuicHttpFrameType::GOAWAY) {
    encoded_payload =
        GenerateGoAwayPayloadLineParts(header, encoded_payload, line_parts);
  } else if (header.type == QuicHttpFrameType::WINDOW_UPDATE) {
    encoded_payload = GenerateWindowUpdatePayloadLineParts(
        header, encoded_payload, line_parts);
  }

  if (FrameCanHaveHpackPayload(header)) {
    GenerateHpackPayloadLineParts(header, encoded_payload, line_parts);
    return;
  }

  GenerateMiscPayloadLineParts("Payload", false, encoded_payload, line_parts);
}

void GenerateTrailingPaddingLineParts(QuicStringPiece trailing_padding,
                                      LineParts* line_parts) {
  while (!trailing_padding.empty()) {
    QuicStringPiece fragment = trailing_padding.substr(0, 4);
    QuicString bytes = FormatBytes(fragment);
    QuicString comment = "Padding";
    line_parts->push_back({bytes, comment});
    trailing_padding.remove_prefix(fragment.size());
  }
}

void GeneratePaddablePayloadLineParts(const QuicHttpFrameHeader& header,
                                      QuicStringPiece encoded_payload,
                                      LineParts* line_parts) {
  if (encoded_payload.empty()) {
    return;
  }
  if (!FrameIsPadded(header)) {
    GenerateUnpaddedPayloadLineParts(header, encoded_payload, line_parts);
    return;
  }
  const size_t pad_length = static_cast<unsigned char>(encoded_payload[0]);
  QuicStringPiece unpadded_payload;
  QuicStringPiece trailing_padding;

  QuicString bytes = FormatBytes(encoded_payload.substr(0, 1));
  QuicString comment = QuicStrCat("PadLen: ", pad_length, " trailing byte");
  if (pad_length != 1) {
    comment.append("s");
  }

  encoded_payload.remove_prefix(1);
  if (encoded_payload.size() < pad_length) {
    comment.append(" (Too Long)");
    trailing_padding = encoded_payload;
  } else {
    unpadded_payload =
        encoded_payload.substr(0, encoded_payload.size() - pad_length);
    trailing_padding =
        absl::ClippedSubstr(encoded_payload, unpadded_payload.size());
  }
  line_parts->push_back({bytes, comment});

  GenerateUnpaddedPayloadLineParts(header, unpadded_payload, line_parts);
  GenerateTrailingPaddingLineParts(trailing_padding, line_parts);
}

void GeneratePayloadLineParts(const QuicHttpFrameHeader& header,
                              QuicStringPiece encoded_payload,
                              LineParts* line_parts) {}

}  // namespace

QuicString FormatFrameHeader(QuicStringPiece encoded_frame,
                             QuicStringPiece line_prefix) {
  LineParts line_parts;
  GenerateFrameHeaderLineParts(encoded_frame, &line_parts);
  return AssembleParts(line_parts, line_prefix);
}

QuicString FormatFrames(QuicStringPiece encoded_frames,
                        QuicStringPiece line_prefix) {
  LineParts line_parts;
  while (encoded_frames.size() >= QuicHttpFrameHeader::EncodedSize()) {
    if (!line_parts.empty()) {
      line_parts.push_back({"", ""});
    }
    QuicHttpFrameHeader header =
        GenerateFrameHeaderLineParts(encoded_frames, &line_parts);
    encoded_frames.remove_prefix(QuicHttpFrameHeader::EncodedSize());
    QuicStringPiece encoded_payload =
        encoded_frames.substr(0, header.payload_length);
    encoded_frames.remove_prefix(encoded_payload.size());
    if (!encoded_payload.empty()) {
      GeneratePaddablePayloadLineParts(header, encoded_payload, &line_parts);
    }
  }
  if (!encoded_frames.empty()) {
    if (!line_parts.empty()) {
      line_parts.push_back({"", ""});
    }
    GenerateMiscPayloadLineParts("Truncated Frame Header", false,
                                 encoded_frames, &line_parts);
  }
  return AssembleParts(line_parts, line_prefix);
}

namespace {

typedef std::vector<QuicStringPiece> FileLines;

FileLines SplitFileContents(QuicStringPiece file_contents) {
  return ::absl::StrSplit(file_contents, '\n');
}

class StringPieceStorage : public QuicStringPiece {
 public:
  StringPieceStorage() = default;
  explicit StringPieceStorage(QuicStringPiece s) : QuicStringPiece(s) {}
  StringPieceStorage(StringPieceStorage&& s) = default;
  explicit StringPieceStorage(std::unique_ptr<QuicString> s)
      : QuicStringPiece(*s), storage_(std::move(s)) {}

 private:
  std::unique_ptr<QuicString> storage_;
};

class FileFormatter {
 public:
  static util::StatusOr<QuicString> Format(QuicStringPiece file_contents) {
    FileFormatter fp(true, file_contents);
    return fp.Format();
  }

 private:
  FileFormatter(bool _, QuicStringPiece file_contents)
      : input_lines_(SplitFileContents(file_contents)),
        file_contents_(file_contents) {}

  util::StatusOr<QuicString> Format() {
    while (!Done()) {
      auto status_or_found_array = AdvanceToStartOfArrayOrEOF();
      if (!status_or_found_array.ok()) {
        return status_or_found_array.status();
      }
      if (!status_or_found_array.ConsumeValueOrDie()) {
        CHECK(Done());
        break;
      }
      bool do_format = false;
      auto status_or_do_format = GatherArrayContentsToFormat();
      if (!status_or_do_format.ok()) {
        LOG(ERROR) << "GatherArrayContentsToFormat returned status: "
                   << status_or_do_format.status().error_message();
      } else {
        do_format = status_or_do_format.ConsumeValueOrDie();
      }
      if (do_format) {
        EmitFormattedArray();
        did_format_ = true;
      } else {
        EmitSkippedArray();
      }
    }
    if (!did_format_) {
      return QuicString();
    }
    QuicString result = absl::StrJoin(output_lines_, "\n");
    if (SameExceptForTrailingWhiteSpace(file_contents_, result)) {
      return QuicString();
    }
    return result;
  }

  // Advances from input_line_num_ to EOF looking for the start of an
  // char array initialized with an aggregate literal, stopping if it
  // finds one.
  // Returns true if finds the start of an array at input_line_num_,
  // else returns false because
  util::StatusOr<bool> AdvanceToStartOfArrayOrEOF() {
    VLOG(1) << "AdvanceToStartOfArrayOrEOF starting @ " << input_line_num_;
    bool in_suppressed_array = false;
    while (!Done()) {
      QuicStringPiece input_line = CurrentInputLine();
      switch (IdentifyCppLineType(input_line)) {
        case kStartOfCharArray:
          VLOG(1) << "kStartOfCharArray @ " << input_line_num_ << ": "
                  << input_line;
          if (in_suppressed_array) {
            return ReportNestedArray();
          }
          array_starts_at_ = input_line_num_;
          if (!frame_format_allowed_) {
            in_suppressed_array = true;
          } else if (input_line.find("V3") != QuicStringPiece::npos) {
            // Skip formatting Spdy V3 lines in spdy_framer_test.cc.
            VLOG(1) << "Found array with V3: " << input_line;
            in_suppressed_array = true;
          } else {
            // For now have decided to suppress ADDING clang-format off/on
            // because this formatter is emitting a comment at the end of each
            // line, preventing clang-format from combining adjacent lines.
            do_emit_clang_format_off_ = false;
            line_prefix_ = GetLeadingBlanks(input_line).ToString() + "    ";
            return true;
          }
          break;

        case kCharLiteral:
          break;

        case kEndOfCharArray:
          if (in_suppressed_array) {
            in_suppressed_array = false;
          }
          break;

        case kClangFormatOff:
          clang_format_allowed_ = false;
          break;

        case kClangFormatOn:
          clang_format_allowed_ = true;
          break;

        case kFrameFormatOff:
          frame_format_allowed_ = false;
          break;

        case kFrameFormatOn:
          frame_format_allowed_ = true;
          break;

        case kWhitespaceOrLineComment:
          break;

        case kOther:
          break;
      }
      AppendInputLineToOutput();
    }
    return false;
  }

  // Returns true if reaches the end of the aggregate literal as expected,
  // and has parsed all the lines. Returns false if we should skip formatting
  // the array after all.
  util::StatusOr<bool> GatherArrayContentsToFormat() {
    CHECK_EQ(array_starts_at_, input_line_num_);
    CHECK_EQ(kStartOfCharArray, IdentifyCppLineType(CurrentInputLine()));
    AppendInputLineToOutput();
    array_storage_.clear();
    bool local_clang_format_allowed_ = clang_format_allowed_;
    bool local_frame_format_allowed_ = frame_format_allowed_;
    bool comma_at_end = true;

    while (!Done()) {
      QuicStringPiece input_line = CurrentInputLine();
      switch (IdentifyCppLineType(input_line)) {
        case kStartOfCharArray:
          return ReportNestedArray();

        case kCharLiteral:
          if (!comma_at_end) {
            return FailedPreconditionError(
                QuicStrCat("Already reached end of array that "
                           "started at line ",
                           array_starts_at_, ", but at line ", input_line_num_,
                           " it appears to continue."));
          }
          RETURN_IF_ERROR(
              AddCharLiteralsInLineToStorage(input_line, &comma_at_end));
          break;

        case kEndOfCharArray:
          return array_storage_.size() >= QuicHttpFrameHeader::EncodedSize();

        case kClangFormatOff:
          local_clang_format_allowed_ = false;
          break;

        case kClangFormatOn:
          local_clang_format_allowed_ = true;
          break;

        case kFrameFormatOff:
          local_frame_format_allowed_ = false;
          break;

        case kFrameFormatOn:
          local_frame_format_allowed_ = true;
          break;

        case kWhitespaceOrLineComment:
          // Ignoring (i.e. dropping) these lines for now.
          break;

        case kOther:
          return FailedPreconditionError(
              QuicStrCat("Found unexpected content on line ", input_line_num_,
                         " in array starting at line ", array_starts_at_,
                         "\nLine contents: ", input_line));
      }
      ++input_line_num_;
    }
    return FailedPreconditionError(
        QuicStrCat("Found array starting at line ", array_starts_at_,
                   ", but didn't find the end of the array"));
  }

  Status AddCharLiteralsInLineToStorage(QuicStringPiece line,
                                        bool* comma_at_end) {
    auto status_or_parsed = ParseCharLiteralsInLine(line, comma_at_end);
    if (!status_or_parsed.ok()) {
      return util::Annotate(status_or_parsed.status(),
                            QuicStrCat("Parsing line ", input_line_num_, ": ",
                                       CurrentInputLine()));
    }
    array_storage_ += status_or_parsed.ConsumeValueOrDie();
    return ::util::OkStatus();
  }

  void EmitFormattedArray() {
    if (do_emit_clang_format_off_) {
      AppendStringToOutput(line_prefix_ + kLiteralClangFormatOff);
    }
    AppendStringToOutput(FormatFrames(array_storage_, line_prefix_));
    if (do_emit_clang_format_off_) {
      AppendStringToOutput(line_prefix_ + kLiteralClangFormatOn);
    }
    TerminateArray();
  }

  void EmitSkippedArray() {
    size_t array_end_line_num_ = input_line_num_;
    input_line_num_ = array_starts_at_ + 1;
    while (input_line_num_ <= array_end_line_num_) {
      AppendInputLineToOutput();
    }
  }

  void TerminateArray() {
    CHECK_LT(array_starts_at_, input_line_num_);
    CHECK_EQ(kEndOfCharArray, IdentifyCppLineType(CurrentInputLine()));
    AppendInputLineToOutput();
  }

  bool Done() { return input_line_num_ > input_lines_.size(); }

  const QuicStringPiece& CurrentInputLine() {
    CHECK_LT(0, input_line_num_);
    CHECK_LE(input_line_num_, input_lines_.size());
    return input_lines_[input_line_num_ - 1];
  }

  void AppendInputLineToOutput() {
    const QuicStringPiece& current_line = CurrentInputLine();
    // Make sure we didn't already do this.
    CHECK(output_lines_.empty() ||
          output_lines_.back().data() != current_line.data());
    output_lines_.push_back(StringPieceStorage(current_line));
    ++input_line_num_;
  }

  void AppendStringToOutput(QuicString s) {
    strings::StripTrailingNewline(&s);
    std::unique_ptr<QuicString> sptr(new QuicString(std::move(s)));
    StringPieceStorage sps(std::move(sptr));
    output_lines_.push_back(std::move(sps));
  }

  Status ReportNestedArray() {
    return FailedPreconditionError(QuicStrCat(
        "Found the start of an array at line ", input_line_num_,
        ", but already in an array that started at line ", array_starts_at_));
  }

  const FileLines input_lines_;
  std::vector<StringPieceStorage> output_lines_;
  QuicString array_storage_;
  QuicString line_prefix_;
  QuicStringPiece file_contents_;
  size_t input_line_num_ = 1;  // One-based for easier error reporting.
  size_t array_starts_at_ = -1;
  bool clang_format_allowed_ = true;
  bool frame_format_allowed_ = true;
  bool do_emit_clang_format_off_;
  bool did_format_ = false;
};

}  // namespace

util::StatusOr<QuicString> FormatFramesInFileContents(
    QuicStringPiece file_contents) {
  return FileFormatter::Format(file_contents);
}

}  // namespace test
}  // namespace gfe_quic
