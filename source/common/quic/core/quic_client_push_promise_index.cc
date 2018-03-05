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

#include "common/quic/core/quic_client_push_promise_index.h"

#include "common/quic/core/quic_client_promised_info.h"
#include "common/quic/core/spdy_utils.h"
#include "common/quic/platform/api/quic_string.h"

using gfe_spdy::SpdyHeaderBlock;

namespace gfe_quic {

QuicClientPushPromiseIndex::QuicClientPushPromiseIndex() {}

QuicClientPushPromiseIndex::~QuicClientPushPromiseIndex() {}

QuicClientPushPromiseIndex::TryHandle::~TryHandle() {}

QuicClientPromisedInfo* QuicClientPushPromiseIndex::GetPromised(
    const QuicString& url) {
  QuicPromisedByUrlMap::iterator it = promised_by_url_.find(url);
  if (it == promised_by_url_.end()) {
    return nullptr;
  }
  return it->second;
}

QuicAsyncStatus QuicClientPushPromiseIndex::Try(
    const gfe_spdy::SpdyHeaderBlock& request,
    QuicClientPushPromiseIndex::Delegate* delegate,
    TryHandle** handle) {
  QuicString url(SpdyUtils::GetPromisedUrlFromHeaders(request));
  QuicPromisedByUrlMap::iterator it = promised_by_url_.find(url);
  if (it != promised_by_url_.end()) {
    QuicClientPromisedInfo* promised = it->second;
    QuicAsyncStatus rv = promised->HandleClientRequest(request, delegate);
    if (rv == QUIC_PENDING) {
      *handle = promised;
    }
    return rv;
  }
  return QUIC_FAILURE;
}

}  // namespace gfe_quic
