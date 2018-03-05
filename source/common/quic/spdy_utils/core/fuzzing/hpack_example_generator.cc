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

#include <cstdint>

#include "base/commandlineflags.h"
#include "base/init_google.h"
#include "file/base/file.h"
#include "file/base/helpers.h"
#include "common/quic/spdy_utils/core/fuzzing/hpack_fuzz_util.h"
#include "common/quic/spdy_utils/core/hpack/hpack_constants.h"
#include "common/quic/spdy_utils/core/hpack/hpack_encoder.h"
#include "common/quic/spdy_utils/core/spdy_protocol.h"
#include "common/quic/spdy_utils/platform/api/spdy_string.h"
#include "util/random/mt_random.h"
#include "util/task/status.h"

DEFINE_FLAG(string,
            file_to_write,
            "",
            "Target file for generated HPACK header sets.");
DEFINE_FLAG(uint64, /* allow-non-std-int */
            example_count,
            100,
            "Number of header sets to generate");

using gfe_spdy::HpackEncoder;
using gfe_spdy::HpackFuzzUtil;
using gfe_spdy::ObtainHpackHuffmanTable;
using gfe_spdy::SpdyHeaderBlock;
using gfe_spdy::SpdyString;

int main(int argc, char** argv) {
  InitGoogle(argv[0], &argc, &argv, true);

  if (base::GetFlag(FLAGS_file_to_write).empty()) {
    fprintf(stderr, "No output file given\n");
    return -1;
  }
  LOG(INFO) << "Writing output to " << base::GetFlag(FLAGS_file_to_write);
  File* file = file::OpenOrDie(base::GetFlag(FLAGS_file_to_write), "w",
                               file::Defaults());

  HpackFuzzUtil::GeneratorContext context;
  HpackFuzzUtil::InitializeGeneratorContext(&context);

  MTRandom random;
  uint32_t seed = random.Rand32();
  LOG(INFO) << "Seeding with " << seed << "";
  random.Reset(seed);

  HpackEncoder encoder(ObtainHpackHuffmanTable());

  for (uint64_t i = base::GetFlag(FLAGS_example_count); i > 0; --i) {
    SpdyHeaderBlock headers =
        HpackFuzzUtil::NextGeneratedHeaderSet(&random, &context);

    SpdyString buffer;
    CHECK(encoder.EncodeHeaderSet(headers, &buffer));

    SpdyString prefix = HpackFuzzUtil::HeaderBlockPrefix(buffer.size());

    file::WriteString(file, prefix, file::Defaults()).IgnoreError();
    file::WriteString(file, buffer, file::Defaults()).IgnoreError();
  }
  file->Close(file::Defaults()).IgnoreError();
  return 0;
}
