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

#ifndef GFE_SPDY_CORE_SPDY_BUG_TRACKER_H_
#define GFE_SPDY_CORE_SPDY_BUG_TRACKER_H_

// Defined in Blaze when targetting non-production platforms (iOS, Android, etc)
// The fallback implimentation is the same as in Chromium which simply delegates
// to LOG(DFATAL) which is part of PG3.
#if SPDY_GENERIC_BUG

#define SPDY_BUG LOG(DFATAL)
#define SPDY_BUG_IF(condition) LOG_IF(DFATAL, condition)
#define FLAGS_spdy_always_log_bugs_for_tests true

#else

#include "gfe/gfe2/base/bug_utils.h"

// For external SPDY, SPDY_BUG should be #defined to LOG(DFATAL) and
// SPDY_BUG_IF(condition) to LOG_IF(DFATAL, condition) as client-side log rate
// limiting is less important and chrome doesn't LOG_FIRST_N anyway.
//
// This file should change infrequently if ever, so update cost should be
// minimal. Meanwhile we do want different macros so we can rate limit server
// side, so the google3 shared code increments GFE varz, and chrome can have its
// own custom hooks.
#define SPDY_BUG GFE_BUG
#define SPDY_BUG_IF GFE_BUG_IF
#define FLAGS_spdy_always_log_bugs_for_tests FLAGS_gfe_always_log_bug_for_tests

#endif  // __ANDROID__

#endif  // GFE_SPDY_CORE_SPDY_BUG_TRACKER_H_
