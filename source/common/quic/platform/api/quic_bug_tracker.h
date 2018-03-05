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

#ifndef GFE_QUIC_PLATFORM_API_QUIC_BUG_TRACKER_H_
#define GFE_QUIC_PLATFORM_API_QUIC_BUG_TRACKER_H_

#include "common/quic/platform/impl/quic_bug_tracker_impl.h"

#define QUIC_BUG QUIC_BUG_IMPL
#define QUIC_BUG_IF QUIC_BUG_IF_IMPL
#define QUIC_PEER_BUG QUIC_PEER_BUG_IMPL
#define QUIC_PEER_BUG_IF QUIC_PEER_BUG_IF_IMPL

#endif  // GFE_QUIC_PLATFORM_API_QUIC_BUG_TRACKER_H_
