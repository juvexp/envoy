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

// Copyright 2012 Google Inc. All Rights Reserved.
// Author: hkhalil@google.com (Hasan Khalil)

#include "common/quic/spdy_utils/core/spdy_frame_reader.h"

// For ntohs() and ntohl()
#ifdef WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

#include "common/quic/spdy_utils/core/spdy_protocol.h"

namespace gfe_spdy {

SpdyFrameReader::SpdyFrameReader(const char* data, const size_t len)
    : data_(data), len_(len), ofs_(0) {}

bool SpdyFrameReader::ReadUInt8(uint8_t* result) {
  // Make sure that we have the whole uint8_t.
  if (!CanRead(1)) {
    OnFailure();
    return false;
  }

  // Read into result.
  *result = *reinterpret_cast<const uint8_t*>(data_ + ofs_);

  // Iterate.
  ofs_ += 1;

  return true;
}

bool SpdyFrameReader::ReadUInt16(uint16_t* result) {
  // Make sure that we have the whole uint16_t.
  if (!CanRead(2)) {
    OnFailure();
    return false;
  }

  // Read into result.
  *result = ntohs(*(reinterpret_cast<const uint16_t*>(data_ + ofs_)));

  // Iterate.
  ofs_ += 2;

  return true;
}

bool SpdyFrameReader::ReadUInt32(uint32_t* result) {
  // Make sure that we have the whole uint32_t.
  if (!CanRead(4)) {
    OnFailure();
    return false;
  }

  // Read into result.
  *result = ntohl(*(reinterpret_cast<const uint32_t*>(data_ + ofs_)));

  // Iterate.
  ofs_ += 4;

  return true;
}

bool SpdyFrameReader::ReadUInt64(uint64_t* result) {
  // Make sure that we have the whole uint64_t.
  if (!CanRead(8)) {
    OnFailure();
    return false;
  }

  // Read into result. Network byte order is big-endian.
  uint64_t upper = ntohl(*(reinterpret_cast<const uint32_t*>(data_ + ofs_)));
  uint64_t lower =
      ntohl(*(reinterpret_cast<const uint32_t*>(data_ + ofs_ + 4)));
  *result = (upper << 32) + lower;

  // Iterate.
  ofs_ += 8;

  return true;
}

bool SpdyFrameReader::ReadUInt31(uint32_t* result) {
  bool success = ReadUInt32(result);

  // Zero out highest-order bit.
  if (success) {
    *result &= 0x7fffffff;
  }

  return success;
}

bool SpdyFrameReader::ReadUInt24(uint32_t* result) {
  // Make sure that we have the whole uint24_t.
  if (!CanRead(3)) {
    OnFailure();
    return false;
  }

  // Read into result.
  *result = 0;
  memcpy(reinterpret_cast<char*>(result) + 1, data_ + ofs_, 3);
  *result = ntohl(*result);

  // Iterate.
  ofs_ += 3;

  return true;
}

bool SpdyFrameReader::ReadStringPiece16(SpdyStringPiece* result) {
  // Read resultant length.
  uint16_t result_len;
  if (!ReadUInt16(&result_len)) {
    // OnFailure() already called.
    return false;
  }

  // Make sure that we have the whole string.
  if (!CanRead(result_len)) {
    OnFailure();
    return false;
  }

  // Set result.
  *result = SpdyStringPiece(data_ + ofs_, result_len);

  // Iterate.
  ofs_ += result_len;

  return true;
}

bool SpdyFrameReader::ReadStringPiece32(SpdyStringPiece* result) {
  // Read resultant length.
  uint32_t result_len;
  if (!ReadUInt32(&result_len)) {
    // OnFailure() already called.
    return false;
  }

  // Make sure that we have the whole string.
  if (!CanRead(result_len)) {
    OnFailure();
    return false;
  }

  // Set result.
  *result = SpdyStringPiece(data_ + ofs_, result_len);

  // Iterate.
  ofs_ += result_len;

  return true;
}

bool SpdyFrameReader::ReadBytes(void* result, size_t size) {
  // Make sure that we have enough data to read.
  if (!CanRead(size)) {
    OnFailure();
    return false;
  }

  // Read into result.
  memcpy(result, data_ + ofs_, size);

  // Iterate.
  ofs_ += size;

  return true;
}

bool SpdyFrameReader::Seek(size_t size) {
  if (!CanRead(size)) {
    OnFailure();
    return false;
  }

  // Iterate.
  ofs_ += size;

  return true;
}

bool SpdyFrameReader::IsDoneReading() const {
  return len_ == ofs_;
}

bool SpdyFrameReader::CanRead(size_t bytes) const {
  return bytes <= (len_ - ofs_);
}

void SpdyFrameReader::OnFailure() {
  // Set our iterator to the end of the buffer so that further reads fail
  // immediately.
  ofs_ = len_;
}

}  // namespace gfe_spdy
