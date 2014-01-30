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


#ifndef SPEECHEVENTLISTRECORD_H_3221741757__H_
#define SPEECHEVENTLISTRECORD_H_3221741757__H_


#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

struct SpeechEventRecord {
    std::string eventType;
    int64_t streamNumber;
    int32_t textPosition;
    int32_t textLength;
    int32_t currentData;
    int32_t nextData;
    std::string stringData;
    int32_t duration;
};

struct SpeechEventListRecord {
    std::string speechServiceId;
    int64_t timestampMillisecUTC;
    std::vector<SpeechEventRecord > speechEvents;
};

namespace avro {
template<> struct codec_traits<SpeechEventRecord> {
    static void encode(Encoder& e, const SpeechEventRecord& v) {
        avro::encode(e, v.eventType);
        avro::encode(e, v.streamNumber);
        avro::encode(e, v.textPosition);
        avro::encode(e, v.textLength);
        avro::encode(e, v.currentData);
        avro::encode(e, v.nextData);
        avro::encode(e, v.stringData);
        avro::encode(e, v.duration);
    }
    static void decode(Decoder& d, SpeechEventRecord& v) {
        avro::decode(d, v.eventType);
        avro::decode(d, v.streamNumber);
        avro::decode(d, v.textPosition);
        avro::decode(d, v.textLength);
        avro::decode(d, v.currentData);
        avro::decode(d, v.nextData);
        avro::decode(d, v.stringData);
        avro::decode(d, v.duration);
    }
};

template<> struct codec_traits<SpeechEventListRecord> {
    static void encode(Encoder& e, const SpeechEventListRecord& v) {
        avro::encode(e, v.speechServiceId);
        avro::encode(e, v.timestampMillisecUTC);
        avro::encode(e, v.speechEvents);
    }
    static void decode(Decoder& d, SpeechEventListRecord& v) {
        avro::decode(d, v.speechServiceId);
        avro::decode(d, v.timestampMillisecUTC);
        avro::decode(d, v.speechEvents);
    }
};

}
#endif
