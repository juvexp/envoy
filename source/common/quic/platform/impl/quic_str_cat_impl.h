#ifndef GFE_QUIC_PLATFORM_IMPL_QUIC_STR_CAT_IMPL_H_
#define GFE_QUIC_PLATFORM_IMPL_QUIC_STR_CAT_IMPL_H_

#include <string>
#include <utility>

#include "absl/strings/str_cat.h"
#include "fmt/printf.h"

namespace gfe_quic {

template <typename... Args>
inline std::string QuicStrCatImpl(const Args&... args) {
  return absl::StrCat(args...);
}

template <typename... Args>
inline std::string QuicStringPrintfImpl(const Args&... args) {
  return fmt::sprintf(std::forward<const Args&>(args)...);
}

}  // namespace gfe_quic

#endif  // GFE_QUIC_PLATFORM_IMPL_QUIC_STR_CAT_IMPL_H_
