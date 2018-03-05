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

#ifndef GFE_QUIC_PLATFORM_API_QUIC_LOGGING_H_
#define GFE_QUIC_PLATFORM_API_QUIC_LOGGING_H_

#include "common/quic/platform/impl/quic_logging_impl.h"

// Please note following QUIC_LOG are platform dependent:
// INFO severity can be degraded (to VLOG(1) or DVLOG(1)).
// Some platforms may not support QUIC_LOG_FIRST_N or QUIC_LOG_EVERY_N_SEC, and
// they would simply be translated to LOG.

#define QUIC_DVLOG(verbose_level) QUIC_DVLOG_IMPL(verbose_level)
#define QUIC_DLOG(severity) QUIC_DLOG_IMPL(severity)
#define QUIC_DLOG_IF(severity, condition) QUIC_DLOG_IF_IMPL(severity, condition)
#define QUIC_VLOG(verbose_level) QUIC_VLOG_IMPL(verbose_level)
#define QUIC_LOG(severity) QUIC_LOG_IMPL(severity)
#define QUIC_LOG_FIRST_N(severity, n) QUIC_LOG_FIRST_N_IMPL(severity, n)
#define QUIC_LOG_EVERY_N_SEC(severity, seconds) \
  QUIC_LOG_EVERY_N_SEC_IMPL(severity, seconds)
#define QUIC_LOG_IF(severity, condition) QUIC_LOG_IF_IMPL(severity, condition)

#define QUIC_PREDICT_FALSE(x) QUIC_PREDICT_FALSE_IMPL(x)

// This is a noop in release build.
#define QUIC_NOTREACHED() QUIC_NOTREACHED_IMPL()

#define QUIC_PLOG(severity) QUIC_PLOG_IMPL(severity)

#endif  // GFE_QUIC_PLATFORM_API_QUIC_LOGGING_H_
