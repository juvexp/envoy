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


// Designed for formatting frame literals in C++ tests of HTTP/2 frame decoding.
// Currently optimized for spdy_framer_test.cc.
//
// Currently reads the C++ to be formatted from stdin and writes the formatted
// code back to stdout. Someday may complete support for reading from files
// given by name on the command line.
// Note that you don't want to redirect the output in the example command
// below back to the input file as that will probably have the effect of
// clearing the file altogether.
/*

blaze build //gfe/quic/http/tools:frame_formatter
blaze-bin/gfe/quic/http/tools/quic_http_frame_formatter \
  < gfe/spdy/spdy_framer_test.cc
  > gfe/spdy/spdy_framer_test.cc-formatted
diff gfe/spdy/spdy_framer_test.cc gfe/spdy/spdy_framer_test.cc-formatted

*/

#include <stddef.h>
#include <stdio.h>

#include <iostream>
#include <string>

#include "base/init_google.h"
#include "base/logging.h"
#include "test/common/quic/http/tools/quic_http_frame_formatter.h"
#include "common/quic/platform/api/quic_bug_tracker.h"
#include "util/task/statusor.h"

using ::util::StatusOr;

namespace {

string StringFromStdin() {
  string str;
  while (!feof(stdin) && !ferror(stdin)) {
    size_t const kBufferSize = 4096;
    char buffer[kBufferSize];
    size_t bytes_read = fread(buffer, 1, kBufferSize, stdin);
    str.append(buffer, bytes_read);
  }
  return str;
}

}  // namespace

namespace gfe_quic {
namespace test {

// Sketch of methods needed for reading and formatting files...
//
//     StatusOr<string> FormatFileContents(StringPiece file_path) {
//       string file_contents;
//       Status status =
//           file::GetContents(file_path, &file_contents, file::Defaults());
//       if (!status.ok()) {
//         return status;
//       }
//       return
//
//
//
//       return Status::OK;
//     }
//
//     StatusOr<string> OpenAndFormatFile(StringPiece file_path) {
//       string file_contents;
//       Status status =
//           file::GetContents(file_path, &file_contents, file::Defaults());
//       if (!status.ok()) {
//         return status;
//       }
//
//       return Status::OK;
//     }

}  // namespace test
}  // namespace gfe_quic

int main(int argc, char** argv) {
  InitGoogle(argv[0], &argc, &argv, true);

  if (argc > 1) {
    // File paths on command line.
    for (int ndx = 1; ndx < argc; ++ndx) {
      QUIC_BUG << "NOT YET IMPLEMENTED";
    }
  } else {
    std::cerr << "Reading from stdin\n";
    string file_contents = StringFromStdin();
    if (file_contents.size() > 0) {
      util::StatusOr<string> status_or_formatted =
          gfe_quic::test::FormatFramesInFileContents(file_contents);
      if (!status_or_formatted.ok()) {
        std::cerr << "Error formatting stdin: ";
        std::cerr << status_or_formatted.status().error_message();
        std::cerr << "\n";
        return 1;
      }
      string formatted_contents = status_or_formatted.ConsumeValueOrDie();
      if (formatted_contents == "") {
        std::cout << file_contents;
        return 1;
      } else {
        std::cout << formatted_contents;
        return 0;
      }
    }
  }

  return 0;
}
