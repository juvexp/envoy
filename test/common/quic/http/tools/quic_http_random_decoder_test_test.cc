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
#include <functional>
#include <set>
#include <type_traits>

#include "base/commandlineflags.h"
#include "base/logging.h"
#include "base/vlog_is_on.h"
#include "common/quic/http/decoder/quic_http_decode_buffer.h"
#include "common/quic/http/decoder/quic_http_decode_status.h"
#include "common/quic/platform/api/quic_bug_tracker.h"
#include "testing/base/public/gunit.h"
#include "util/random/mt_random.h"
#include "util/random/test_random-inl.h"

DECLARE_int32(select_one_decode_limit);
DECLARE_int32(select_zero_or_one_decode_limit);

namespace gfe_quic {
namespace test {
namespace {
const char kData[]{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
const bool kReturnNonZeroOnFirst = true;
const bool kMayReturnZeroOnFirst = false;

// Confirm the behavior of various parts of QuicHttpRandomDecoderTest.
class QuicHttpRandomDecoderTestTest : public QuicHttpRandomDecoderTest {
 public:
  QuicHttpRandomDecoderTestTest() : data_db_(kData) {
    CHECK_EQ(sizeof kData, 8);
    CHECK_LT(sizeof kData, FLAGS_select_one_decode_limit);
    CHECK_LT(sizeof kData, FLAGS_select_zero_or_one_decode_limit);
  }

 protected:
  typedef std::function<QuicHttpDecodeStatus(QuicHttpDecodeBuffer* db)>
      DecodingFn;

  QuicHttpDecodeStatus StartDecoding(QuicHttpDecodeBuffer* db) override {
    ++start_decoding_calls_;
    if (start_decoding_fn_) {
      return start_decoding_fn_(db);
    }
    return QuicHttpDecodeStatus::kDecodeError;
  }

  QuicHttpDecodeStatus ResumeDecoding(QuicHttpDecodeBuffer* db) override {
    ++resume_decoding_calls_;
    if (resume_decoding_fn_) {
      return resume_decoding_fn_(db);
    }
    return QuicHttpDecodeStatus::kDecodeError;
  }

  bool StopDecodeOnDone() override {
    ++stop_decode_on_done_calls_;
    if (override_stop_decode_on_done_) {
      return sub_stop_decode_on_done_;
    }
    return QuicHttpRandomDecoderTest::StopDecodeOnDone();
  }

  size_t start_decoding_calls_ = 0;
  size_t resume_decoding_calls_ = 0;
  size_t stop_decode_on_done_calls_ = 0;

  DecodingFn start_decoding_fn_;
  DecodingFn resume_decoding_fn_;

  QuicHttpDecodeBuffer data_db_;

  bool sub_stop_decode_on_done_ = true;
  bool override_stop_decode_on_done_ = true;
};

// Decode a single byte on the StartDecoding call, then stop.
TEST_F(QuicHttpRandomDecoderTestTest, StopOnStartPartiallyDone) {
  start_decoding_fn_ = [this](QuicHttpDecodeBuffer* db) {
    EXPECT_EQ(1, start_decoding_calls_);
    // Make sure the correct buffer is being used.
    EXPECT_EQ(kData, db->cursor());
    EXPECT_EQ(sizeof kData, db->Remaining());
    db->DecodeUInt8();
    return QuicHttpDecodeStatus::kDecodeDone;
  };

  EXPECT_EQ(QuicHttpDecodeStatus::kDecodeDone,
            DecodeSegments(&data_db_, SelectRemaining()));
  EXPECT_EQ(1, data_db_.Offset());
  // StartDecoding should only be called once from each call to DecodeSegments.
  EXPECT_EQ(1, start_decoding_calls_);
  EXPECT_EQ(0, resume_decoding_calls_);
  EXPECT_EQ(1, stop_decode_on_done_calls_);
}

// Stop decoding upon return from the first ResumeDecoding call.
TEST_F(QuicHttpRandomDecoderTestTest, StopOnResumePartiallyDone) {
  start_decoding_fn_ = [this](QuicHttpDecodeBuffer* db) {
    EXPECT_EQ(1, start_decoding_calls_);
    db->DecodeUInt8();
    return QuicHttpDecodeStatus::kDecodeInProgress;
  };
  resume_decoding_fn_ = [this](QuicHttpDecodeBuffer* db) {
    EXPECT_EQ(1, resume_decoding_calls_);
    // Make sure the correct buffer is being used.
    EXPECT_EQ(data_db_.cursor(), db->cursor());
    db->DecodeUInt16();
    return QuicHttpDecodeStatus::kDecodeDone;
  };

  // Check that the base class honors it's member variable stop_decode_on_done_.
  override_stop_decode_on_done_ = false;
  stop_decode_on_done_ = true;

  EXPECT_EQ(QuicHttpDecodeStatus::kDecodeDone,
            DecodeSegments(&data_db_, SelectRemaining()));
  EXPECT_EQ(3, data_db_.Offset());
  EXPECT_EQ(1, start_decoding_calls_);
  EXPECT_EQ(1, resume_decoding_calls_);
  EXPECT_EQ(1, stop_decode_on_done_calls_);
}

// Decode a random sized chunks, always reporting back kDecodeInProgress.
TEST_F(QuicHttpRandomDecoderTestTest, InProgressWhenEmpty) {
  start_decoding_fn_ = [this](QuicHttpDecodeBuffer* db) {
    EXPECT_EQ(1, start_decoding_calls_);
    // Consume up to 2 bytes.
    if (db->HasData()) {
      db->DecodeUInt8();
      if (db->HasData()) {
        db->DecodeUInt8();
      }
    }
    return QuicHttpDecodeStatus::kDecodeInProgress;
  };
  resume_decoding_fn_ = [this](QuicHttpDecodeBuffer* db) {
    // Consume all available bytes.
    if (db->HasData()) {
      db->AdvanceCursor(db->Remaining());
    }
    return QuicHttpDecodeStatus::kDecodeInProgress;
  };

  EXPECT_EQ(QuicHttpDecodeStatus::kDecodeInProgress,
            DecodeSegments(&data_db_, SelectRandom(kMayReturnZeroOnFirst)));
  EXPECT_TRUE(data_db_.Empty());
  EXPECT_EQ(1, start_decoding_calls_);
  EXPECT_LE(1, resume_decoding_calls_);
  EXPECT_EQ(0, stop_decode_on_done_calls_);
}

TEST_F(QuicHttpRandomDecoderTestTest, DoneExactlyAtEnd) {
  start_decoding_fn_ = [this](QuicHttpDecodeBuffer* db) {
    EXPECT_EQ(1, start_decoding_calls_);
    EXPECT_EQ(1, db->Remaining());
    EXPECT_EQ(1, db->FullSize());
    db->DecodeUInt8();
    return QuicHttpDecodeStatus::kDecodeInProgress;
  };
  resume_decoding_fn_ = [this](QuicHttpDecodeBuffer* db) {
    EXPECT_EQ(1, db->Remaining());
    EXPECT_EQ(1, db->FullSize());
    db->DecodeUInt8();
    if (data_db_.Remaining() == 1) {
      return QuicHttpDecodeStatus::kDecodeDone;
    }
    return QuicHttpDecodeStatus::kDecodeInProgress;
  };
  override_stop_decode_on_done_ = true;
  sub_stop_decode_on_done_ = true;

  EXPECT_EQ(QuicHttpDecodeStatus::kDecodeDone,
            DecodeSegments(&data_db_, SelectOne()));
  EXPECT_EQ(0, data_db_.Remaining());
  EXPECT_EQ(1, start_decoding_calls_);
  EXPECT_EQ((sizeof kData) - 1, resume_decoding_calls_);
  // Didn't need to call StopDecodeOnDone because we didn't finish early.
  EXPECT_EQ(0, stop_decode_on_done_calls_);
}

TEST_F(QuicHttpRandomDecoderTestTest, DecodeSeveralWaysToEnd) {
  // Each call to StartDecoding or ResumeDecoding will consume all that is
  // available. When all the data has been consumed, returns kDecodeDone.
  int decoded_since_start = -1000;
  auto shared_fn = [&decoded_since_start, this](QuicHttpDecodeBuffer* db) {
    decoded_since_start += db->Remaining();
    db->AdvanceCursor(db->Remaining());
    EXPECT_EQ(0, db->Remaining());
    if (decoded_since_start == data_db_.FullSize()) {
      return QuicHttpDecodeStatus::kDecodeDone;
    }
    return QuicHttpDecodeStatus::kDecodeInProgress;
  };

  start_decoding_fn_ = [&decoded_since_start,
                        shared_fn](QuicHttpDecodeBuffer* db) {
    decoded_since_start = 0;
    return shared_fn(db);
  };
  resume_decoding_fn_ = shared_fn;

  Validator validator = ValidateDoneAndEmpty();

  EXPECT_TRUE(DecodeAndValidateSeveralWays(&data_db_, kMayReturnZeroOnFirst,
                                           validator));

  // We should have reached the end.
  EXPECT_EQ(0, data_db_.Remaining());

  // We currently have 4 ways of decoding; update this if that changes.
  EXPECT_EQ(4, start_decoding_calls_);

  // Didn't need to call StopDecodeOnDone because we didn't finish early.
  EXPECT_EQ(0, stop_decode_on_done_calls_);
}

TEST_F(QuicHttpRandomDecoderTestTest, DecodeTwoWaysAndStopEarly) {
  // On the second decode, return kDecodeDone before finishing.
  int decoded_since_start = -1000;
  auto shared_fn = [&decoded_since_start, this](QuicHttpDecodeBuffer* db) {
    uint32_t amount = db->Remaining();
    if (start_decoding_calls_ == 2 && amount > 1) {
      amount = 1;
    }
    decoded_since_start += amount;
    db->AdvanceCursor(amount);
    if (decoded_since_start == data_db_.FullSize()) {
      return QuicHttpDecodeStatus::kDecodeDone;
    }
    if (decoded_since_start > 1 && start_decoding_calls_ == 2) {
      return QuicHttpDecodeStatus::kDecodeDone;
    }
    return QuicHttpDecodeStatus::kDecodeInProgress;
  };

  start_decoding_fn_ = [&decoded_since_start,
                        shared_fn](QuicHttpDecodeBuffer* db) {
    decoded_since_start = 0;
    return shared_fn(db);
  };
  resume_decoding_fn_ = shared_fn;

  // We expect the first and second to succeed, but the second to end at a
  // different offset, which DecodeAndValidateSeveralWays should complain about.
  Validator validator = [this](const QuicHttpDecodeBuffer& input,
                               QuicHttpDecodeStatus status) -> AssertionResult {
    if (start_decoding_calls_ <= 2 &&
        status != QuicHttpDecodeStatus::kDecodeDone) {
      return ::testing::AssertionFailure()
             << "Expected QuicHttpDecodeStatus::kDecodeDone, not " << status;
    }
    if (start_decoding_calls_ > 2) {
      return ::testing::AssertionFailure()
             << "How did we get to pass " << start_decoding_calls_;
    }
    return ::testing::AssertionSuccess();
  };

  EXPECT_FALSE(DecodeAndValidateSeveralWays(&data_db_, kMayReturnZeroOnFirst,
                                            validator));
  EXPECT_EQ(2, start_decoding_calls_);
  EXPECT_EQ(1, stop_decode_on_done_calls_);
}

TEST_F(QuicHttpRandomDecoderTestTest, DecodeThreeWaysAndError) {
  // Return kDecodeError from ResumeDecoding on the third decoding pass.
  int decoded_since_start = -1000;
  auto shared_fn = [&decoded_since_start, this](QuicHttpDecodeBuffer* db) {
    if (start_decoding_calls_ == 3 && decoded_since_start > 0) {
      return QuicHttpDecodeStatus::kDecodeError;
    }
    uint32_t amount = db->Remaining();
    if (start_decoding_calls_ == 3 && amount > 1) {
      amount = 1;
    }
    decoded_since_start += amount;
    db->AdvanceCursor(amount);
    if (decoded_since_start == data_db_.FullSize()) {
      return QuicHttpDecodeStatus::kDecodeDone;
    }
    return QuicHttpDecodeStatus::kDecodeInProgress;
  };

  start_decoding_fn_ = [&decoded_since_start,
                        shared_fn](QuicHttpDecodeBuffer* db) {
    decoded_since_start = 0;
    return shared_fn(db);
  };
  resume_decoding_fn_ = shared_fn;

  Validator validator = ValidateDoneAndEmpty();
  EXPECT_FALSE(DecodeAndValidateSeveralWays(&data_db_, kReturnNonZeroOnFirst,
                                            validator));
  EXPECT_EQ(3, start_decoding_calls_);
  EXPECT_EQ(0, stop_decode_on_done_calls_);
}

// CorruptEnum should produce lots of different values. On the assumption that
// the enum gets at least a byte of storage, we should be able to produce
// 256 distinct values.
TEST(CorruptEnumTest, ManyValues) {
  std::set<uint64_t> values;
  QuicHttpDecodeStatus status;
  LOG(INFO) << "sizeof status = " << sizeof status;
  ::random_test::IncrementingSequence rng(0);
  for (int ndx = 0; ndx < 256; ++ndx) {
    CorruptEnum(&status, &rng);
    values.insert(static_cast<uint64_t>(status));
  }
}

// In practice the underlying type is an int, and currently that is 4 bytes.
typedef typename std::underlying_type<QuicHttpDecodeStatus>::type
    QuicHttpDecodeStatusUT;

struct CorruptEnumTestStruct {
  QuicHttpDecodeStatusUT filler1;
  QuicHttpDecodeStatus status;
  QuicHttpDecodeStatusUT filler2;
};

// CorruptEnum should only overwrite the enum, not any adjacent storage.
TEST(CorruptEnumTest, CorruptsOnlyEnum) {
  MTRandom rng(0);
  for (const QuicHttpDecodeStatusUT filler :
       {QuicHttpDecodeStatusUT(), ~QuicHttpDecodeStatusUT()}) {
    LOG(INFO) << "filler=0x" << std::hex << filler;
    CorruptEnumTestStruct s;
    s.filler1 = filler;
    s.filler2 = filler;
    for (int ndx = 0; ndx < 256; ++ndx) {
      CorruptEnum(&s.status, &rng);
      EXPECT_EQ(s.filler1, filler);
      EXPECT_EQ(s.filler2, filler);
    }
  }
}

class BoostAndRestoreVerbosityTest : public QuicHttpRandomDecoderTest {
 protected:
  QuicHttpDecodeStatus StartDecoding(QuicHttpDecodeBuffer* db) override {
    QUIC_BUG << "BoostAndRestoreVerbosityTest::StartDecoding() called!";
    return QuicHttpDecodeStatus::kDecodeError;
  }
  QuicHttpDecodeStatus ResumeDecoding(QuicHttpDecodeBuffer* db) override {
    QUIC_BUG << "BoostAndRestoreVerbosityTest::ResumeDecoding() called!";
    return QuicHttpDecodeStatus::kDecodeError;
  }
};

TEST_F(BoostAndRestoreVerbosityTest, BoostAndRestoreVerbosity) {
  // Boosts only if --v=0 and --vmodule=""
  base::SetFlag(&FLAGS_v, 0);
  base::SetFlag(&FLAGS_vmodule, "");
  {
    RestoreVerbosity rv(BoostLoggingIfDefault(2));
    EXPECT_EQ(base::GetFlag(FLAGS_v), 2);
    EXPECT_EQ(base::GetFlag(FLAGS_vmodule), "");
  }
  EXPECT_EQ(base::GetFlag(FLAGS_v), 0);
  EXPECT_EQ(base::GetFlag(FLAGS_vmodule), "");

  // Doesn't boost if FLAGS_v is not 0.
  base::SetFlag(&FLAGS_v, 1);
  {
    EXPECT_EQ(base::GetFlag(FLAGS_v), 1);
    RestoreVerbosity rv(BoostLoggingIfDefault(2));
    EXPECT_EQ(base::GetFlag(FLAGS_v), 1);
    EXPECT_EQ(base::GetFlag(FLAGS_vmodule), "");
  }
  EXPECT_EQ(base::GetFlag(FLAGS_v), 1);

  // Doesn't boost if FLAGS_vmodule is not "".
  base::SetFlag(&FLAGS_v, 0);
  base::SetFlag(&FLAGS_vmodule, "foo=1");
  {
    EXPECT_EQ(base::GetFlag(FLAGS_v), 0);
    RestoreVerbosity rv(BoostLoggingIfDefault(2));
    EXPECT_EQ(base::GetFlag(FLAGS_v), 0);
    EXPECT_EQ(base::GetFlag(FLAGS_vmodule), "foo=1");
  }
  EXPECT_EQ(base::GetFlag(FLAGS_v), 0);
  EXPECT_EQ(base::GetFlag(FLAGS_vmodule), "foo=1");
}

TEST_F(BoostAndRestoreVerbosityTest, RestoreVerbosity) {
  base::SetFlag(&FLAGS_v, 0);
  {
    // The constructor doesn't modify FLAGS_v...
    RestoreVerbosity rv(123);
    EXPECT_EQ(base::GetFlag(FLAGS_v), 0);
  }
  // But the destructor forces FLAGS_v to the value passed to the constructor.
  EXPECT_EQ(base::GetFlag(FLAGS_v), 123);
  {
    RestoreVerbosity rv(0);
    EXPECT_EQ(base::GetFlag(FLAGS_v), 123);
  }

  EXPECT_EQ(base::GetFlag(FLAGS_v), 0);
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
