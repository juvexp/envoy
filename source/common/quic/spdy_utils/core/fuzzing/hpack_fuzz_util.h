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

#ifndef GFE_SPDY_CORE_FUZZING_HPACK_FUZZ_UTIL_H_
#define GFE_SPDY_CORE_FUZZING_HPACK_FUZZ_UTIL_H_

#include <memory>
#include <vector>

#include "common/quic/spdy_utils/core/hpack/hpack_decoder_adapter.h"
#include "common/quic/spdy_utils/core/hpack/hpack_encoder.h"
#include "common/quic/spdy_utils/platform/api/spdy_export.h"
#include "common/quic/spdy_utils/platform/api/spdy_string.h"
#include "common/quic/spdy_utils/platform/api/spdy_string_piece.h"
#include "util/random/random_base.h"

namespace gfe_spdy {

class SPDY_EXPORT_PRIVATE HpackFuzzUtil {
 public:
  // A GeneratorContext holds ordered header names & values which are
  // initially seeded and then expanded with dynamically generated data.
  struct SPDY_EXPORT_PRIVATE GeneratorContext {
    GeneratorContext();
    ~GeneratorContext();
    std::vector<SpdyString> names;
    std::vector<SpdyString> values;
  };

  // Initializes a GeneratorContext with a random seed and name/value fixtures.
  static void InitializeGeneratorContext(GeneratorContext* context);

  // Generates a header set from the generator context.
  static SpdyHeaderBlock NextGeneratedHeaderSet(RandomBase* random,
                                                GeneratorContext* context);

  // Samples a size from the exponential distribution with mean |mean|,
  // upper-bounded by |sanity_bound|.
  static size_t SampleExponential(RandomBase* random,
                                  size_t mean,
                                  size_t sanity_bound);

  // Holds an input SpdyString, and manages an offset into that SpdyString.
  struct SPDY_EXPORT_PRIVATE Input {
    Input();  // Initializes |offset| to zero.
    ~Input();

    size_t remaining() { return input.size() - offset; }
    const char* ptr() { return input.data() + offset; }

    SpdyString input;
    size_t offset;
  };

  // Returns true if the next header block was set at |out|. Returns
  // false if no input header blocks remain.
  static bool NextHeaderBlock(Input* input, SpdyStringPiece* out);

  // Returns the serialized header block length prefix for a block of
  // |block_size| bytes.
  static SpdyString HeaderBlockPrefix(size_t block_size);

  // A FuzzerContext holds fuzzer input, as well as each of the decoder and
  // encoder stages which fuzzed header blocks are processed through.
  struct SPDY_EXPORT_PRIVATE FuzzerContext {
    FuzzerContext();
    ~FuzzerContext();
    std::unique_ptr<HpackDecoderAdapter> first_stage;
    std::unique_ptr<HpackEncoder> second_stage;
    std::unique_ptr<HpackDecoderAdapter> third_stage;
  };

  static void InitializeFuzzerContext(FuzzerContext* context);

  // Runs |input_block| through |first_stage| and, iff that succeeds,
  // |second_stage| and |third_stage| as well. Returns whether all stages
  // processed the input without error.
  static bool RunHeaderBlockThroughFuzzerStages(FuzzerContext* context,
                                                SpdyStringPiece input_block);
};

}  // namespace gfe_spdy

#endif  // GFE_SPDY_CORE_FUZZING_HPACK_FUZZ_UTIL_H_
