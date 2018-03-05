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

#include "common/quic/spdy_utils/core/fuzzing/hpack_fuzz_util.h"

#include "file/base/file.h"
#include "file/base/helpers.h"
#include "test/common/quic/spdy_utils/core/spdy_test_utils.h"
#include "common/quic/spdy_utils/platform/api/spdy_string_utils.h"
#include "testing/base/public/gunit.h"
#include "util/random/mt_random.h"

namespace gfe_spdy {
namespace test {

TEST(HpackFuzzUtilTest, GeneratorContextInitialization) {
  HpackFuzzUtil::GeneratorContext context;
  HpackFuzzUtil::InitializeGeneratorContext(&context);

  // Context was seeded with initial name & value fixtures.
  EXPECT_LT(0u, context.names.size());
  EXPECT_LT(0u, context.values.size());
}

TEST(HpackFuzzUtil, GeneratorContextExpansion) {
  HpackFuzzUtil::GeneratorContext context;

  MTRandom random;
  SpdyHeaderBlock headers =
      HpackFuzzUtil::NextGeneratedHeaderSet(&random, &context);

  // Headers were generated, and the generator context was expanded.
  EXPECT_LT(0u, headers.size());
  EXPECT_LT(0u, context.names.size());
  EXPECT_LT(0u, context.values.size());
}

// TODO: A better test would mock |random| and evaluate
// SampleExponential along fixed points of the random->RandDouble() domain.
TEST(HpackFuzzUtilTest, SampleExponentialRegression) {
  MTRandom random;
  random.Reset(42);

  EXPECT_EQ(6u, HpackFuzzUtil::SampleExponential(&random, 10, 30));
  EXPECT_EQ(4u, HpackFuzzUtil::SampleExponential(&random, 10, 30));
  EXPECT_EQ(0u, HpackFuzzUtil::SampleExponential(&random, 10, 30));
  EXPECT_EQ(1u, HpackFuzzUtil::SampleExponential(&random, 10, 30));
  EXPECT_EQ(1u, HpackFuzzUtil::SampleExponential(&random, 10, 30));

  for (size_t i = 0; i != 100; ++i) {
    EXPECT_GE(30u, HpackFuzzUtil::SampleExponential(&random, 10, 30));
  }
}

TEST(HpackFuzzUtilTest, ParsesSequenceOfHeaderBlocks) {
  char fixture[] =
      "\x00\x00\x00\x05""aaaaa"
      "\x00\x00\x00\x04""bbbb"
      "\x00\x00\x00\x03""ccc"
      "\x00\x00\x00\x02""dd"
      "\x00\x00\x00\x01""e"
      "\x00\x00\x00\x00"""
      "\x00\x00\x00\x03""fin";

  HpackFuzzUtil::Input input;
  input.input.assign(fixture, ABSL_ARRAYSIZE(fixture) - 1);

  SpdyStringPiece block;

  EXPECT_TRUE(HpackFuzzUtil::NextHeaderBlock(&input, &block));
  EXPECT_EQ("aaaaa", block);
  EXPECT_TRUE(HpackFuzzUtil::NextHeaderBlock(&input, &block));
  EXPECT_EQ("bbbb", block);
  EXPECT_TRUE(HpackFuzzUtil::NextHeaderBlock(&input, &block));
  EXPECT_EQ("ccc", block);
  EXPECT_TRUE(HpackFuzzUtil::NextHeaderBlock(&input, &block));
  EXPECT_EQ("dd", block);
  EXPECT_TRUE(HpackFuzzUtil::NextHeaderBlock(&input, &block));
  EXPECT_EQ("e", block);
  EXPECT_TRUE(HpackFuzzUtil::NextHeaderBlock(&input, &block));
  EXPECT_EQ("", block);
  EXPECT_TRUE(HpackFuzzUtil::NextHeaderBlock(&input, &block));
  EXPECT_EQ("fin", block);
  EXPECT_FALSE(HpackFuzzUtil::NextHeaderBlock(&input, &block));
}

TEST(HpackFuzzUtilTest, SerializedHeaderBlockPrefixes) {
  EXPECT_EQ(SpdyString("\x00\x00\x00\x00", 4),
            HpackFuzzUtil::HeaderBlockPrefix(0));
  EXPECT_EQ(SpdyString("\x00\x00\x00\x05", 4),
            HpackFuzzUtil::HeaderBlockPrefix(5));
  EXPECT_EQ("\x4f\xb3\x0a\x91", HpackFuzzUtil::HeaderBlockPrefix(1337133713));
}

TEST(HpackFuzzUtilTest, PassValidInputThroughAllStages) {
  // Example lifted from HpackDecoderTest.SectionD4RequestHuffmanExamples.
  SpdyString input = SpdyHexDecode("828684418cf1e3c2e5f23a6ba0ab90f4ff");

  HpackFuzzUtil::FuzzerContext context;
  HpackFuzzUtil::InitializeFuzzerContext(&context);

  EXPECT_TRUE(
      HpackFuzzUtil::RunHeaderBlockThroughFuzzerStages(&context, input));

  SpdyHeaderBlock expect;
  expect[":method"] = "GET";
  expect[":scheme"] = "http";
  expect[":path"] = "/";
  expect[":authority"] = "www.example.com";
  EXPECT_EQ(expect, context.third_stage->decoded_block());
}

TEST(HpackFuzzUtilTest, ValidFuzzExamplesRegressionTest) {
  SpdyString file_path =
      FLAGS_test_srcdir +
      "/google3/gfe/spdy/core/fuzzing/testdata/examples.hpack";

  HpackFuzzUtil::Input input;
  CHECK_OK(file::GetContents(file_path, &input.input, file::Defaults()));

  HpackFuzzUtil::FuzzerContext context;
  HpackFuzzUtil::InitializeFuzzerContext(&context);

  SpdyStringPiece block;
  while (HpackFuzzUtil::NextHeaderBlock(&input, &block)) {
    // As these are valid examples, all fuzz stages should succeed.
    EXPECT_TRUE(
        HpackFuzzUtil::RunHeaderBlockThroughFuzzerStages(&context, block));
  }
}

}  // namespace test
}  // namespace gfe_spdy
