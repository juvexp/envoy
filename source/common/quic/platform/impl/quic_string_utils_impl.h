#ifndef GFE_QUIC_PLATFORM_IMPL_QUIC_STRING_UTILS_IMPL_H_
#define GFE_QUIC_PLATFORM_IMPL_QUIC_STRING_UTILS_IMPL_H_

#include <utility>

#include "absl/strings/str_cat.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"
// #include "strings/hex_ascii_dump.h"
// #include "third_party/absl/strings/escaping.h"

namespace gfe_quic {

template <typename... Args>
inline void QuicStrAppendImpl(QuicString* output, const Args&... args) {
  absl::StrAppend(output, args...);
}

}  // namespace gfe_quic

#endif  // GFE_QUIC_PLATFORM_IMPL_QUIC_STRING_UTILS_IMPL_H_
