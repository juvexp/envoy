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

#include "test/common/quic/http/tools/quic_http_random_decoder_test.h"

#include <stddef.h>
#include <algorithm>
#include <memory>

#include "base/commandlineflags.h"
#include "base/logging.h"
#include "base/vlog_is_on.h"
#include "common/quic/http/decoder/quic_http_decode_buffer.h"
#include "common/quic/http/decoder/quic_http_decode_status.h"
#include "common/quic/http/quic_http_constants.h"
#include "testing/base/public/googletest.h"
#include "gtest/gtest.h"

// It's rather time consuming to decode large buffers one at a time,
// especially with the log level cranked up. So, by default we don't do
// that unless explicitly requested.
DEFINE_int32(
    select_one_decode_limit,
    30,
    "Maximum length of a QuicHttpDecodeBuffer that will be decoded one "
    "byte at a time.");
// And that is doubling true for decoding alternating buffers of zero and
// one byte in length.
DEFINE_int32(
    select_zero_or_one_decode_limit,
    20,
    "Maximum length of a QuicHttpDecodeBuffer that will be decoded zero "
    "or one byte at a time.");

using ::testing::AssertionFailure;
using ::testing::AssertionResult;
using ::testing::AssertionSuccess;

namespace gfe_quic {
namespace test {

QuicHttpRandomDecoderTest::QuicHttpRandomDecoderTest()
    : random_(FLAGS_test_random_seed) {
  // Assuming here that we don't run multiple tests at the same time in the
  // same process.
  static uint32_t last_seed = 0;
  LOG_IF(INFO, last_seed != FLAGS_test_random_seed)
      << "Random seed (--test_random_seed): " << FLAGS_test_random_seed;
  last_seed = FLAGS_test_random_seed;
}

// In support of better coverage of VLOG and DVLOG lines, increase the log
// level if not overridden already. Because there are so many D?VLOG(2)
// statements triggered by the randomized decoding, it is recommended to only
// call this method when decoding a small payload.
int QuicHttpRandomDecoderTest::BoostLoggingIfDefault(int target_level) {
  // Note that setting --v=2 generates a huge amount of logging for
  // the large string sizes, so we avoid that in general.
  int old_level = base::GetFlag(FLAGS_v);
  if (target_level > 0 && base::GetFlag(FLAGS_v) == 0 &&
      base::GetFlag(FLAGS_vmodule).empty()) {
    base::SetFlag(&FLAGS_v, target_level);
    LOG(INFO) << "Overriding --v=0 for this test case, setting to "
              << base::GetFlag(FLAGS_v);
  }
  return old_level;
}

QuicHttpDecodeStatus QuicHttpRandomDecoderTest::DecodeSegments(
    QuicHttpDecodeBuffer* original,
    const SelectSize& select_size) {
  QuicHttpDecodeStatus status = QuicHttpDecodeStatus::kDecodeInProgress;
  bool first = true;
  VLOG(2) << "DecodeSegments: input size=" << original->Remaining();
  while (first || original->HasData()) {
    size_t remaining = original->Remaining();
    size_t size =
        std::min(remaining, select_size(first, original->Offset(), remaining));
    QuicHttpDecodeBuffer db(original->cursor(), size);
    VLOG(2) << "Decoding " << size << " bytes of " << remaining << " remaining";
    if (first) {
      first = false;
      status = StartDecoding(&db);
    } else {
      status = ResumeDecoding(&db);
    }
    // A decoder MUST consume some input (if any is available), else we could
    // get stuck in infinite loops.
    if (db.Offset() == 0 && db.HasData() &&
        status != QuicHttpDecodeStatus::kDecodeError) {
      ADD_FAILURE() << "Decoder didn't make any progress; db.FullSize="
                    << db.FullSize()
                    << "   original.Offset=" << original->Offset();
      return QuicHttpDecodeStatus::kDecodeError;
    }
    original->AdvanceCursor(db.Offset());
    switch (status) {
      case QuicHttpDecodeStatus::kDecodeDone:
        if (original->Empty() || StopDecodeOnDone()) {
          return QuicHttpDecodeStatus::kDecodeDone;
        }
        continue;
      case QuicHttpDecodeStatus::kDecodeInProgress:
        continue;
      case QuicHttpDecodeStatus::kDecodeError:
        return QuicHttpDecodeStatus::kDecodeError;
    }
  }
  return status;
}

// Decode |original| multiple times, with different segmentations, validating
// after each decode, returning on the first failure.
AssertionResult QuicHttpRandomDecoderTest::DecodeAndValidateSeveralWays(
    QuicHttpDecodeBuffer* original,
    bool return_non_zero_on_first,
    const Validator& validator) {
  const uint32_t original_remaining = original->Remaining();
  VLOG(1) << "DecodeAndValidateSeveralWays - Start, remaining = "
          << original_remaining;
  uint32_t first_consumed;
  {
    // Fast decode (no stopping unless decoder does so).
    QuicHttpDecodeBuffer input(original->cursor(), original_remaining);
    VLOG(2) << "DecodeSegmentsAndValidate with SelectRemaining";
    VERIFY_SUCCESS(
        DecodeSegmentsAndValidate(&input, SelectRemaining(), validator))
        << "\nFailed with SelectRemaining; input.Offset=" << input.Offset()
        << "; input.Remaining=" << input.Remaining();
    first_consumed = input.Offset();
  }
  if (original_remaining <= FLAGS_select_one_decode_limit) {
    // Decode again, one byte at a time.
    QuicHttpDecodeBuffer input(original->cursor(), original_remaining);
    VLOG(2) << "DecodeSegmentsAndValidate with SelectOne";
    VERIFY_SUCCESS(DecodeSegmentsAndValidate(&input, SelectOne(), validator))
        << "\nFailed with SelectOne; input.Offset=" << input.Offset()
        << "; input.Remaining=" << input.Remaining();
    VERIFY_EQ(first_consumed, input.Offset()) << "\nFailed with SelectOne";
  }
  if (original_remaining <= FLAGS_select_zero_or_one_decode_limit) {
    // Decode again, one or zero bytes at a time.
    QuicHttpDecodeBuffer input(original->cursor(), original_remaining);
    VLOG(2) << "DecodeSegmentsAndValidate with SelectZeroAndOne";
    VERIFY_SUCCESS(DecodeSegmentsAndValidate(
        &input, SelectZeroAndOne(return_non_zero_on_first), validator))
        << "\nFailed with SelectZeroAndOne";
    VERIFY_EQ(first_consumed, input.Offset())
        << "\nFailed with SelectZeroAndOne; input.Offset=" << input.Offset()
        << "; input.Remaining=" << input.Remaining();
  }
  {
    // Decode again, with randomly selected segment sizes.
    QuicHttpDecodeBuffer input(original->cursor(), original_remaining);
    VLOG(2) << "DecodeSegmentsAndValidate with SelectRandom";
    VERIFY_SUCCESS(DecodeSegmentsAndValidate(
        &input, SelectRandom(return_non_zero_on_first), validator))
        << "\nFailed with SelectRandom; input.Offset=" << input.Offset()
        << "; input.Remaining=" << input.Remaining();
    VERIFY_EQ(first_consumed, input.Offset()) << "\nFailed with SelectRandom";
  }
  VERIFY_EQ(original_remaining, original->Remaining());
  original->AdvanceCursor(first_consumed);
  VLOG(1) << "DecodeAndValidateSeveralWays - SUCCESS";
  return ::testing::AssertionSuccess();
}

// static
QuicHttpRandomDecoderTest::SelectSize
QuicHttpRandomDecoderTest::SelectZeroAndOne(bool return_non_zero_on_first) {
  std::shared_ptr<bool> zero_next(new bool);
  *zero_next = !return_non_zero_on_first;
  return [zero_next](bool first, size_t offset, size_t remaining) -> size_t {
    if (*zero_next) {
      *zero_next = false;
      return 0;
    } else {
      *zero_next = true;
      return 1;
    }
  };
}

QuicHttpRandomDecoderTest::SelectSize QuicHttpRandomDecoderTest::SelectRandom(
    bool return_non_zero_on_first) {
  return [this, return_non_zero_on_first](bool first, size_t offset,
                                          size_t remaining) -> size_t {
    uint32_t r = random_.Rand32();
    if (first && return_non_zero_on_first) {
      CHECK_LT(0u, remaining);
      if (remaining == 1) {
        return 1;
      }
      return 1 + (r % remaining);  // size in range [1, remaining).
    }
    return r % (remaining + 1);  // size in range [0, remaining].
  };
}

uint32_t QuicHttpRandomDecoderTest::RandStreamId() {
  return random_.Rand32() & QuicHttpStreamIdMask();
}

QuicHttpRandomDecoderTest::RestoreVerbosity::RestoreVerbosity(int old_verbosity)
    : old_verbosity_(old_verbosity) {}

QuicHttpRandomDecoderTest::RestoreVerbosity::~RestoreVerbosity() {
  base::SetFlag(&FLAGS_v, old_verbosity_);
}

}  // namespace test
}  // namespace gfe_quic
