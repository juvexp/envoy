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

#ifndef GFE_QUIC_CORE_PRIVACY_MODE_H_
#define GFE_QUIC_CORE_PRIVACY_MODE_H_

namespace gfe_quic {

// Privacy Mode is enabled if cookies to particular site are blocked, so
// Channel ID is disabled on that connection (https, spdy or quic).
enum PrivacyMode {
  PRIVACY_MODE_DISABLED = 0,
  PRIVACY_MODE_ENABLED = 1,
};

};  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_PRIVACY_MODE_H_
