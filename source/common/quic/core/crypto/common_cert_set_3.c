/* Copyright 2018 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* This file contains common certificates. It's designed to be #included in
 * another file, in a namespace. */

#include "common/quic/core/crypto/common_cert_set_3a.inc"
#include "common/quic/core/crypto/common_cert_set_3b.inc"

static const size_t kNumCerts = 52;
static const unsigned char* const kCerts[] = {
  kDERCert0,
  kDERCert1,
  kDERCert2,
  kDERCert3,
  kDERCert4,
  kDERCert5,
  kDERCert6,
  kDERCert7,
  kDERCert8,
  kDERCert9,
  kDERCert10,
  kDERCert11,
  kDERCert12,
  kDERCert13,
  kDERCert14,
  kDERCert15,
  kDERCert16,
  kDERCert17,
  kDERCert18,
  kDERCert19,
  kDERCert20,
  kDERCert21,
  kDERCert22,
  kDERCert23,
  kDERCert24,
  kDERCert25,
  kDERCert26,
  kDERCert27,
  kDERCert28,
  kDERCert29,
  kDERCert30,
  kDERCert31,
  kDERCert32,
  kDERCert33,
  kDERCert34,
  kDERCert35,
  kDERCert36,
  kDERCert37,
  kDERCert38,
  kDERCert39,
  kDERCert40,
  kDERCert41,
  kDERCert42,
  kDERCert43,
  kDERCert44,
  kDERCert45,
  kDERCert46,
  kDERCert47,
  kDERCert48,
  kDERCert49,
  kDERCert50,
  kDERCert51,
};

static const size_t kLens[] = {
  897,
  911,
  1012,
  1049,
  1065,
  1096,
  1097,
  1101,
  1105,
  1105,
  1107,
  1117,
  1127,
  1133,
  1136,
  1138,
  1139,
  1145,
  1149,
  1153,
  1167,
  1172,
  1174,
  1174,
  1176,
  1188,
  1194,
  1196,
  1203,
  1205,
  1206,
  1208,
  1209,
  1210,
  1222,
  1227,
  1236,
  1236,
  1238,
  1283,
  1284,
  1287,
  1298,
  1315,
  1327,
  1340,
  1357,
  1418,
  1447,
  1509,
  1513,
  1632,
};

static const uint64_t kHash = UINT64_C(0x918215a28680ed7e);
