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

#ifndef GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_MESSAGE_INTERFACE_H_
#define GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_MESSAGE_INTERFACE_H_

// MessageInterface and sub-classes represent conceptual HTTP/2 messages:
// A sequence of headers, a body and an optional sequence of trailers.

#include <memory>
#include <utility>
#include <vector>

#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {
namespace test {

class MessageVisitorInterface;
class PushedMessageInterface;

// Represents a message (request or response) to be encoded as HTTP/2 frames.
class MessageInterface {
 public:
  typedef std::pair<QuicString, QuicString> NameAndValue;
  typedef std::vector<NameAndValue> NamesAndValues;

  MessageInterface();
  virtual ~MessageInterface();

  // Calls the appropriate method (VisitMessage or VisitPushedMessage)
  // based on the concrete type of |this|.
  virtual void Visit(MessageVisitorInterface* visitor) const = 0;

  // Contains headers (name & value pairs); the names are lower case, with the
  // pseudo-headers (e.g. ":method") first.
  virtual const NamesAndValues& headers() const = 0;

  // Returns the body of the message.
  virtual const QuicString& body() const = 0;

  // Contains headers (name & value pairs); the names are lower case, with the
  // pseudo-headers (e.g. ":method") first.
  virtual const NamesAndValues& trailers() const = 0;
};

// Represents a request to be encoded as HTTP/2 frames.
class RequestMessageInterface : public virtual MessageInterface {
 public:
  RequestMessageInterface();
  ~RequestMessageInterface() override;
};

// Represents a response to be encoded as HTTP/2 frames. A response may have
// pushed messages, i.e. a request to be sent by a server starting with a
// PUSH_PROMISE frame, and followed by a server initiated stream.
class ResponseMessageInterface : public virtual MessageInterface {
 public:
  typedef std::vector<std::unique_ptr<PushedMessageInterface>> PushedMessages;

  ResponseMessageInterface();
  ~ResponseMessageInterface() override;

  // Only a response on a client initiated stream can have pushed messages
  // and thus there can be only two levels of messages (parent and pushed),
  // though for raw encode or decode benchmarking it doesn't really matter.
  virtual const PushedMessages& pushed_messages() const = 0;
};

// A pushed message, i.e. a server pushed requst and server initiated response.
class PushedMessageInterface : public virtual MessageInterface {
 public:
  PushedMessageInterface();
  ~PushedMessageInterface() override;

  // The headers for a PUSH_PROMISE message.
  virtual const NamesAndValues& pushed_request_headers() const = 0;
};

class ConcreteMessageBase : public virtual MessageInterface {
 public:
  ConcreteMessageBase(const NamesAndValues& headers,
                      QuicStringPiece body,
                      const NamesAndValues& trailers);
  ~ConcreteMessageBase() override;

  const NamesAndValues& headers() const override { return headers_; }
  const QuicString& body() const override { return body_; }
  const NamesAndValues& trailers() const override { return trailers_; }

 private:
  const NamesAndValues headers_;
  const NamesAndValues trailers_;
  const QuicString body_;
};

class RequestMessage : public virtual ConcreteMessageBase,
                       public virtual RequestMessageInterface {
 public:
  RequestMessage(const NamesAndValues& headers, QuicStringPiece body);
  RequestMessage(const NamesAndValues& headers,
                 QuicStringPiece body,
                 const NamesAndValues& trailers);
  ~RequestMessage() override;
  void Visit(MessageVisitorInterface* visitor) const override;
};

class ResponseMessage : public virtual ConcreteMessageBase,
                        public virtual ResponseMessageInterface {
 public:
  ResponseMessage(const NamesAndValues& headers,
                  QuicStringPiece body,
                  const NamesAndValues& trailers);
  ResponseMessage(const NamesAndValues& headers,
                  QuicStringPiece body,
                  const NamesAndValues& trailers,
                  PushedMessages&& pushed_messages);
  ~ResponseMessage() override;
  void Visit(MessageVisitorInterface* visitor) const override;
  const PushedMessages& pushed_messages() const override {
    return pushed_messages_;
  }

 private:
  std::vector<std::unique_ptr<PushedMessageInterface>> pushed_messages_;
};

class MessageVisitorInterface {
 public:
  MessageVisitorInterface();
  virtual ~MessageVisitorInterface();
  virtual void VisitRequestMessage(const RequestMessageInterface& message) = 0;
  virtual void VisitResponseMessage(
      const ResponseMessageInterface& message) = 0;
  virtual void VisitPushedMessage(const PushedMessageInterface& message) = 0;
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_TOOLS_QUIC_HTTP_MESSAGE_INTERFACE_H_
