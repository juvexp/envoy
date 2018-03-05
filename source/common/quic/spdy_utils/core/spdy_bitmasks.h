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

// Copyright 2009 Google Inc. All Rights Reserved.
// Author: fenix@google.com (Roberto J Peon)
// Author: mbelshe@google.com (Mike Belshe)

#ifndef GFE_SPDY_CORE_SPDY_BITMASKS_H_
#define GFE_SPDY_CORE_SPDY_BITMASKS_H_

namespace gfe_spdy {

// StreamId mask from the SpdyHeader
const unsigned int kStreamIdMask = 0x7fffffff;

// Mask the lower 24 bits.
const unsigned int kLengthMask = 0xffffff;

}  // namespace gfe_spdy

#endif  // GFE_SPDY_CORE_SPDY_BITMASKS_H_
