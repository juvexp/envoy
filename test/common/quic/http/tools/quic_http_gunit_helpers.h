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

#ifndef GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_GUNIT_HELPERS_H_
#define GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_GUNIT_HELPERS_H_

// Provides VERIFY_* macros, similar to EXPECT_* and ASSERT_*, but they return
// an AssertionResult if the condition is not satisfied.

// Exports the macros from GFE's failure.h to make the port to Chrome easier
// (i.e. one file to be considered, not many).

#include "gfe/gfe2/test_tools/failure.h"  // IWYU pragma: export
#include "testing/base/public/gunit.h"    // IWYU pragma: keep
                                          // For AssertionSuccess

// TODO: Cleanup and move to gfe/gfe2/test_tools/failure.h.
#define VERIFY_AND_RETURN_SUCCESS(expression) \
  {                                           \
    VERIFY_SUCCESS(expression);               \
    return ::testing::AssertionSuccess();     \
  }

#endif  // GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_GUNIT_HELPERS_H_
