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

#include "common/quic/core/quic_tag.h"

#include <algorithm>

#include "base/macros.h"
#include "common/quic/platform/api/quic_arraysize.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_text_utils.h"

namespace gfe_quic {

bool FindMutualQuicTag(const QuicTagVector& our_tags,
                       const QuicTagVector& their_tags,
                       QuicTag* out_result,
                       size_t* out_index) {
  const size_t num_our_tags = our_tags.size();
  const size_t num_their_tags = their_tags.size();
  for (size_t i = 0; i < num_our_tags; i++) {
    for (size_t j = 0; j < num_their_tags; j++) {
      if (our_tags[i] == their_tags[j]) {
        *out_result = our_tags[i];
        if (out_index != nullptr) {
          *out_index = j;
        }
        return true;
      }
    }
  }

  return false;
}

QuicString QuicTagToString(QuicTag tag) {
  char chars[sizeof tag];
  bool ascii = true;
  const QuicTag orig_tag = tag;

  for (size_t i = 0; i < QUIC_ARRAYSIZE(chars); i++) {
    chars[i] = static_cast<char>(tag);
    if ((chars[i] == 0 || chars[i] == '\xff') &&
        i == QUIC_ARRAYSIZE(chars) - 1) {
      chars[i] = ' ';
    }
    if (!isprint(static_cast<unsigned char>(chars[i]))) {
      ascii = false;
      break;
    }
    tag >>= 8;
  }

  if (ascii) {
    return QuicString(chars, sizeof(chars));
  }

  return QuicTextUtils::Uint64ToString(orig_tag);
}

uint32_t MakeQuicTag(char a, char b, char c, char d) {
  return static_cast<uint32_t>(a) | static_cast<uint32_t>(b) << 8 |
         static_cast<uint32_t>(c) << 16 | static_cast<uint32_t>(d) << 24;
}

bool ContainsQuicTag(const QuicTagVector& tag_vector, QuicTag tag) {
  return std::find(tag_vector.begin(), tag_vector.end(), tag) !=
         tag_vector.end();
}

}  // namespace gfe_quic
