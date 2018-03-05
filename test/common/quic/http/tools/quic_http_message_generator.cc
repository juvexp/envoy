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

#include "test/common/quic/http/tools/quic_http_message_generator.h"

#include <vector>

#include "test/common/quic/http/tools/quic_http_random_util.h"
#include "common/quic/platform/api/quic_ptr_util.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_string_piece.h"
#include "strings/stringpiece_utils.h"

// TODO: Compare headers and trailers to real responses,
// see if the generated values are reasonable.

namespace gfe_quic {
namespace test {
namespace {

typedef MessageInterface::NamesAndValues NamesAndValues;

class SampleAuthorizationTokens {
 public:
  explicit SampleAuthorizationTokens(QuicTestRandomBase* rng) {
    tokens_.push_back("Bearer 1/" + GenerateWebSafeString(50, 100, rng));
    tokens_.push_back("Bearer 1/" + GenerateWebSafeString(75, 150, rng));
    tokens_.push_back("Bearer 1/" + GenerateWebSafeString(100, 200, rng));
    tokens_.push_back("EndUserCreds 1 " +
                      GenerateWebSafeString(400, 1000, rng));
  }

  const QuicString& token(QuicTestRandomBase* rng) const {
    size_t index = rng->Uniform(tokens_.size());
    return tokens_[index];
  }

 private:
  std::vector<QuicString> tokens_;
};

const QuicString& SelectAuthorizationToken(QuicTestRandomBase* rng) {
  // This is single-threaded code. There isn't a need for anything fancier.
  static SampleAuthorizationTokens* sample = nullptr;
  if (sample == nullptr) {
    sample = new SampleAuthorizationTokens(rng);
  }
  return sample->token(rng);
}

void AddAuthorization(NamesAndValues* headers, QuicTestRandomBase* rng) {
  auto token = SelectAuthorizationToken(rng);
  headers->push_back({"authorization", token});
  if (strings::StartsWith(QuicStringPiece(token), "EndUserCreds")) {
    headers->push_back({"x-google-endusercreds", "1"});
  }
}

NamesAndValues GenerateGrpcRequestHeaders(QuicTestRandomBase* rng) {
  NamesAndValues headers;
  headers.push_back({":method", "POST"});
  headers.push_back({":scheme", "https"});
  headers.push_back(
      {":path", "/google.pubsub.v2.PublisherService/CreateTopic"});
  headers.push_back({":authority", "pubsub.googleapis.com"});
  headers.push_back({"grpc-timeout", "15"});
  headers.push_back({"content-type", "application/grpc+proto"});
  headers.push_back({"grpc-encoding", "gzip"});
  headers.push_back({"grpc-accept-encoding", "identity,deflate,gzip"});
  headers.push_back({"grpc-message-type", "some.package.ProtoName"});
  headers.push_back({"user-agent", "http2-message-generator/0.1"});
  AddAuthorization(&headers, rng);
  return headers;
}

NamesAndValues GenerateGrpcResponseHeaders(QuicTestRandomBase* rng) {
  NamesAndValues headers;
  headers.push_back({":status", "200"});
  headers.push_back({":grpc-encoding", "gzip"});
  headers.push_back({"te", "trailers"});
  return headers;
}

NamesAndValues GenerateGrpcResponseTrailers(QuicTestRandomBase* rng) {
  NamesAndValues headers;
  headers.push_back({"grpc-status", "0"});
  headers.push_back({"trace-proto-bin", "jher831yy13JHy3hc"});
  return headers;
}

// Note that this function does NOT generate a valid Length-Prefixed-Message
// used by gRPC. Probably doesn't matter at all for our purpose here.
QuicString GenerateBodyInSizeRange(size_t body_size_lo,
                                   size_t body_size_hi,
                                   QuicTestRandomBase* rng) {
  size_t body_size = GenerateUniformInRange(body_size_lo, body_size_hi, rng);
  return rng->RandString(body_size);
}

}  // namespace

std::unique_ptr<RequestMessageInterface> GenerateGrpcRequest(
    size_t body_size_lo,
    size_t body_size_hi,
    QuicTestRandomBase* rng) {
  NamesAndValues headers = GenerateGrpcRequestHeaders(rng);
  QuicString body = GenerateBodyInSizeRange(body_size_lo, body_size_hi, rng);
  return QuicMakeUnique<RequestMessage>(headers, body);
}

std::unique_ptr<ResponseMessageInterface> GenerateGrpcResponse(
    size_t body_size_lo,
    size_t body_size_hi,
    QuicTestRandomBase* rng) {
  NamesAndValues headers = GenerateGrpcResponseHeaders(rng);
  QuicString body = GenerateBodyInSizeRange(body_size_lo, body_size_hi, rng);
  NamesAndValues trailers = GenerateGrpcResponseTrailers(rng);
  return QuicMakeUnique<ResponseMessage>(headers, body, trailers);
}

}  // namespace test
}  // namespace gfe_quic
