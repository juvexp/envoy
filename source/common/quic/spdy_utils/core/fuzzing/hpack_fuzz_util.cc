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

#include <arpa/inet.h>

#include <algorithm>
#include <cstdint>

#include "common/quic/spdy_utils/core/hpack/hpack_constants.h"
#include "common/quic/spdy_utils/platform/api/spdy_ptr_util.h"

namespace gfe_spdy {

namespace {

// Sampled exponential distribution parameters:
// Number of headers in each header set.
const size_t kHeaderCountMean = 7;
const size_t kHeaderCountMax = 50;
// Selected index within list of headers.
const size_t kHeaderIndexMean = 20;
const size_t kHeaderIndexMax = 200;
// Approximate distribution of header name lengths.
const size_t kNameLengthMean = 5;
const size_t kNameLengthMax = 30;
// Approximate distribution of header value lengths.
const size_t kValueLengthMean = 15;
const size_t kValueLengthMax = 75;

}  //  namespace

HpackFuzzUtil::GeneratorContext::GeneratorContext() = default;
HpackFuzzUtil::GeneratorContext::~GeneratorContext() = default;

HpackFuzzUtil::Input::Input() : offset(0) {}
HpackFuzzUtil::Input::~Input() = default;

HpackFuzzUtil::FuzzerContext::FuzzerContext() = default;
HpackFuzzUtil::FuzzerContext::~FuzzerContext() = default;

// static
void HpackFuzzUtil::InitializeGeneratorContext(GeneratorContext* context) {
  // Seed the generator with common header fixtures.
  context->names.push_back(":authority");
  context->names.push_back(":path");
  context->names.push_back(":status");
  context->names.push_back("cookie");
  context->names.push_back("content-type");
  context->names.push_back("cache-control");
  context->names.push_back("date");
  context->names.push_back("user-agent");
  context->names.push_back("via");

  context->values.push_back("/");
  context->values.push_back("/index.html");
  context->values.push_back("200");
  context->values.push_back("404");
  context->values.push_back("");
  context->values.push_back("baz=bing; foo=bar; garbage");
  context->values.push_back("baz=bing; fizzle=fazzle; garbage");
  context->values.push_back("rudolph=the-red-nosed-reindeer");
  context->values.push_back("had=a;very_shiny=nose");
  context->values.push_back("and\0if\0you\0ever\1saw\0it;");
  context->values.push_back("u; would=even;say-it\xffglows");
}

// static
SpdyHeaderBlock HpackFuzzUtil::NextGeneratedHeaderSet(
    RandomBase* random,
    GeneratorContext* context) {
  SpdyHeaderBlock headers;

  size_t header_count =
      1 + SampleExponential(random, kHeaderCountMean, kHeaderCountMax);
  for (size_t j = 0; j != header_count; ++j) {
    size_t name_index =
        SampleExponential(random, kHeaderIndexMean, kHeaderIndexMax);
    size_t value_index =
        SampleExponential(random, kHeaderIndexMean, kHeaderIndexMax);
    SpdyString name, value;
    if (name_index >= context->names.size()) {
      context->names.push_back(random->RandString(
          1 + SampleExponential(random, kNameLengthMean, kNameLengthMax)));
      name = context->names.back();
    } else {
      name = context->names[name_index];
    }
    if (value_index >= context->values.size()) {
      context->values.push_back(random->RandString(
          1 + SampleExponential(random, kValueLengthMean, kValueLengthMax)));
      value = context->values.back();
    } else {
      value = context->values[value_index];
    }
    headers[name] = value;
  }
  return headers;
}

// static
size_t HpackFuzzUtil::SampleExponential(RandomBase* random,
                                        size_t mean,
                                        size_t sanity_bound) {
  return std::min(static_cast<size_t>(-std::log(random->RandDouble()) * mean),
                  sanity_bound);
}

// static
bool HpackFuzzUtil::NextHeaderBlock(Input* input, SpdyStringPiece* out) {
  CHECK_LE(input->offset, input->input.size());
  if (input->remaining() == 0) {
    return false;
  }
  CHECK_LE(sizeof(uint32_t), input->remaining());

  size_t length = ntohl(*reinterpret_cast<const uint32_t*>(input->ptr()));
  input->offset += sizeof(uint32_t);

  CHECK_LE(length, input->remaining());
  *out = SpdyStringPiece(input->ptr(), length);
  input->offset += length;
  return true;
}

// static
SpdyString HpackFuzzUtil::HeaderBlockPrefix(size_t block_size) {
  uint32_t length = htonl(block_size);
  return SpdyString(reinterpret_cast<char*>(&length), sizeof(uint32_t));
}

// static
void HpackFuzzUtil::InitializeFuzzerContext(FuzzerContext* context) {
  context->first_stage = SpdyMakeUnique<HpackDecoderAdapter>();
  context->second_stage =
      SpdyMakeUnique<HpackEncoder>(ObtainHpackHuffmanTable());
  context->third_stage = SpdyMakeUnique<HpackDecoderAdapter>();
}

// static
bool HpackFuzzUtil::RunHeaderBlockThroughFuzzerStages(
    FuzzerContext* context,
    SpdyStringPiece input_block) {
  // First stage: Decode the input header block. This may fail on invalid input.
  if (!context->first_stage->HandleControlFrameHeadersData(
          input_block.data(), input_block.size())) {
    return false;
  }
  if (!context->first_stage->HandleControlFrameHeadersComplete(nullptr)) {
    return false;
  }
  // Second stage: Re-encode the decoded header block. This must succeed.
  SpdyString second_stage_out;
  CHECK(context->second_stage->EncodeHeaderSet(
      context->first_stage->decoded_block(), &second_stage_out));

  // Third stage: Expect a decoding of the re-encoded block to succeed.
  CHECK(context->third_stage->HandleControlFrameHeadersData(
      second_stage_out.data(), second_stage_out.length()));
  CHECK(context->third_stage->HandleControlFrameHeadersComplete(nullptr));
  return true;
}

}  // namespace gfe_spdy
