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

#include "common/quic/spdy_utils/core/hpack/hpack_entry.h"

#include "base/logging.h"
#include "common/quic/spdy_utils/platform/api/spdy_estimate_memory_usage.h"
#include "strings/numbers.h"

namespace gfe_spdy {

const size_t HpackEntry::kSizeOverhead = 32;

HpackEntry::HpackEntry(SpdyStringPiece name, SpdyStringPiece value,
                       bool is_static, size_t insertion_index)
    : name_(name.data(), name.size()),
      value_(value.data(), value.size()),
      name_ref_(name_),
      value_ref_(value_),
      insertion_index_(insertion_index),
      type_(is_static ? STATIC : DYNAMIC),
      time_added_(0) {}

HpackEntry::HpackEntry(SpdyStringPiece name, SpdyStringPiece value)
    : name_ref_(name),
      value_ref_(value),
      insertion_index_(0),
      type_(LOOKUP),
      time_added_(0) {}

HpackEntry::HpackEntry() : insertion_index_(0), type_(LOOKUP), time_added_(0) {}

HpackEntry::HpackEntry(const HpackEntry& other)
    : insertion_index_(other.insertion_index_),
      type_(other.type_),
      time_added_(0) {
  if (type_ == LOOKUP) {
    name_ref_ = other.name_ref_;
    value_ref_ = other.value_ref_;
  } else {
    name_ = other.name_;
    value_ = other.value_;
    name_ref_ = SpdyStringPiece(name_.data(), name_.size());
    value_ref_ = SpdyStringPiece(value_.data(), value_.size());
  }
}

HpackEntry& HpackEntry::operator=(const HpackEntry& other) {
  insertion_index_ = other.insertion_index_;
  type_ = other.type_;
  if (type_ == LOOKUP) {
    name_ref_ = other.name_ref_;
    value_ref_ = other.value_ref_;
    return *this;
  }
  name_ = other.name_;
  value_ = other.value_;
  name_ref_ = SpdyStringPiece(name_.data(), name_.size());
  value_ref_ = SpdyStringPiece(value_.data(), value_.size());
  return *this;
}

HpackEntry::~HpackEntry() = default;

// static
size_t HpackEntry::Size(SpdyStringPiece name, SpdyStringPiece value) {
  return name.size() + value.size() + kSizeOverhead;
}
size_t HpackEntry::Size() const {
  return Size(name(), value());
}

SpdyString HpackEntry::GetDebugString() const {
  return "{ name: \"" + SpdyString(name_ref_) + "\", value: \"" +
         SpdyString(value_ref_) +
         "\", index: " + strings::SimpleItoa(insertion_index_) +
         (IsStatic() ? " static" : (IsLookup() ? " lookup" : " dynamic")) +
         " }";
}

size_t HpackEntry::EstimateMemoryUsage() const {
  return SpdyEstimateMemoryUsage(name_) + SpdyEstimateMemoryUsage(value_);
}

}  // namespace gfe_spdy
