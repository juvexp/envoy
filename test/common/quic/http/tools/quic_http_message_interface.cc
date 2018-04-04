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

#include "test/common/quic/http/tools/quic_http_message_interface.h"

#include <type_traits>

namespace gfe_quic {
namespace test {
namespace {

#define ASSERT_THIS_IS_CONCRETE                                              \
  {                                                                          \
    typedef decltype(*this) this_type;                                       \
    static_assert(!std::is_abstract<this_type>::value, "Must be concrete."); \
  }

}  // namespace

MessageInterface::MessageInterface() {}
MessageInterface::~MessageInterface() {}

RequestMessageInterface::RequestMessageInterface() {}
RequestMessageInterface::~RequestMessageInterface() {}

ResponseMessageInterface::ResponseMessageInterface() {}
ResponseMessageInterface::~ResponseMessageInterface() {}

PushedMessageInterface::PushedMessageInterface() {}
PushedMessageInterface::~PushedMessageInterface() {}

ConcreteMessageBase::ConcreteMessageBase(const NamesAndValues& headers,
                                         QuicStringPiece body,
                                         const NamesAndValues& trailers)
    : headers_(headers), trailers_(trailers), body_(body) {}
ConcreteMessageBase::~ConcreteMessageBase() {}

RequestMessage::RequestMessage(const NamesAndValues& headers,
                               QuicStringPiece body)
    : ConcreteMessageBase(headers, body, {}) {
  ASSERT_THIS_IS_CONCRETE;
}
RequestMessage::RequestMessage(const NamesAndValues& headers,
                               QuicStringPiece body,
                               const NamesAndValues& trailers)
    : ConcreteMessageBase(headers, body, trailers) {}
RequestMessage::~RequestMessage() {}
void RequestMessage::Visit(MessageVisitorInterface* visitor) const {
  visitor->VisitRequestMessage(*this);
}

ResponseMessage::ResponseMessage(const NamesAndValues& headers,
                                 QuicStringPiece body,
                                 const NamesAndValues& trailers)
    : ConcreteMessageBase(headers, body, trailers) {
  ASSERT_THIS_IS_CONCRETE;
}
ResponseMessage::ResponseMessage(const NamesAndValues& headers,
                                 QuicStringPiece body,
                                 const NamesAndValues& trailers,
                                 PushedMessages&& pushed_messages)
    : ConcreteMessageBase(headers, body, trailers),
      pushed_messages_(std::move(pushed_messages)) {}
ResponseMessage::~ResponseMessage() {}
void ResponseMessage::Visit(MessageVisitorInterface* visitor) const {
  visitor->VisitResponseMessage(*this);
}

MessageVisitorInterface::MessageVisitorInterface() {}
MessageVisitorInterface::~MessageVisitorInterface() {}

}  // namespace test
}  // namespace gfe_quic
