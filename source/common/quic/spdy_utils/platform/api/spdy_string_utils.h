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

#ifndef GFE_SPDY_PLATFORM_API_SPDY_STRING_UTILS_H_
#define GFE_SPDY_PLATFORM_API_SPDY_STRING_UTILS_H_

#include <utility>

// The following header file has to be included from at least
// non-test file in order to avoid strange linking errors.
// TODO: Remove this include as soon as it is included elsewhere in
// non-test code.
#include "common/quic/spdy_utils/platform/api/spdy_mem_slice.h"

#include "common/quic/spdy_utils/platform/api/spdy_string.h"
#include "common/quic/spdy_utils/platform/api/spdy_string_piece.h"
#include "common/quic/spdy_utils/platform/impl/spdy_string_utils_impl.h"

namespace gfe_spdy {

template <typename... Args>
inline SpdyString SpdyStrCat(const Args&... args) {
  return SpdyStrCatImpl(std::forward<const Args&>(args)...);
}

template <typename... Args>
inline void SpdyStrAppend(SpdyString* output, const Args&... args) {
  SpdyStrAppendImpl(output, std::forward<const Args&>(args)...);
}

template <typename... Args>
inline SpdyString SpdyStringPrintf(const Args&... args) {
  return SpdyStringPrintfImpl(std::forward<const Args&>(args)...);
}

template <typename... Args>
inline void SpdyStringAppendF(const Args&... args) {
  SpdyStringAppendFImpl(std::forward<const Args&>(args)...);
}

inline char SpdyHexDigitToInt(char c) {
  return SpdyHexDigitToIntImpl(c);
}

inline SpdyString SpdyHexDecode(SpdyStringPiece data) {
  return SpdyHexDecodeImpl(data);
}

inline bool SpdyHexDecodeToUInt32(SpdyStringPiece data, uint32_t* out) {
  return SpdyHexDecodeToUInt32Impl(data, out);
}

inline SpdyString SpdyHexEncode(const char* bytes, size_t size) {
  return SpdyHexEncodeImpl(bytes, size);
}

inline SpdyString SpdyHexEncodeUInt32AndTrim(uint32_t data) {
  return SpdyHexEncodeUInt32AndTrimImpl(data);
}

inline SpdyString SpdyHexDump(SpdyStringPiece data) {
  return SpdyHexDumpImpl(data);
}

}  // namespace gfe_spdy

#endif  // GFE_SPDY_PLATFORM_API_SPDY_STRING_UTILS_H_
