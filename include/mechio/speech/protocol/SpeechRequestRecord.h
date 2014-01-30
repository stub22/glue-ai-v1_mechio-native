/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef SPEECHREQUESTRECORD_H_1363344155__H_
#define SPEECHREQUESTRECORD_H_1363344155__H_


#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

struct SpeechRequestRecord {
    std::string speechServiceId;
    std::string requestSourceId;
    int64_t timestampMillisecUTC;
    std::string phrase;
};

namespace avro {
template<> struct codec_traits<SpeechRequestRecord> {
    static void encode(Encoder& e, const SpeechRequestRecord& v) {
        avro::encode(e, v.speechServiceId);
        avro::encode(e, v.requestSourceId);
        avro::encode(e, v.timestampMillisecUTC);
        avro::encode(e, v.phrase);
    }
    static void decode(Decoder& d, SpeechRequestRecord& v) {
        avro::decode(d, v.speechServiceId);
        avro::decode(d, v.requestSourceId);
        avro::decode(d, v.timestampMillisecUTC);
        avro::decode(d, v.phrase);
    }
};

}
#endif
