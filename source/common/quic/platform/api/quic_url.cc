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

#include "common/quic/platform/api/quic_url.h"
#include "common/quic/platform/api/quic_string.h"

namespace gfe_quic {

QuicUrl::QuicUrl(QuicStringPiece url) : impl_(url) {}

QuicUrl::QuicUrl(QuicStringPiece url, QuicStringPiece default_scheme)
    : impl_(url, default_scheme) {}

QuicUrl::QuicUrl(const QuicUrl& url) : impl_(url.impl()) {}

bool QuicUrl::IsValid() const {
  return impl_.IsValid();
}

QuicString QuicUrl::ToString() const {
  return impl_.ToStringIfValid();
}

QuicString QuicUrl::HostPort() const {
  return impl_.HostPort();
}

QuicString QuicUrl::PathParamsQuery() const {
  return impl_.PathParamsQuery();
}

QuicString QuicUrl::host() const {
  return impl_.host();
}

QuicString QuicUrl::path() const {
  return impl_.path();
}

QuicString QuicUrl::scheme() const {
  return impl_.scheme();
}

uint16_t QuicUrl::port() const {
  return impl_.port();
}

}  // namespace gfe_quic
