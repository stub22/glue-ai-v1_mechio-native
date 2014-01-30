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


#ifndef SPEECHCONFIGRECORD_H_3674775984__H_
#define SPEECHCONFIGRECORD_H_3674775984__H_


#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

struct SpeechConfigRecord {
    std::string speechServiceId;
    std::string configSourceId;
    std::string voiceName;
    double sampleRate;
};

namespace avro {
template<> struct codec_traits<SpeechConfigRecord> {
    static void encode(Encoder& e, const SpeechConfigRecord& v) {
        avro::encode(e, v.speechServiceId);
        avro::encode(e, v.configSourceId);
        avro::encode(e, v.voiceName);
        avro::encode(e, v.sampleRate);
    }
    static void decode(Decoder& d, SpeechConfigRecord& v) {
        avro::decode(d, v.speechServiceId);
        avro::decode(d, v.configSourceId);
        avro::decode(d, v.voiceName);
        avro::decode(d, v.sampleRate);
    }
};

}
#endif
