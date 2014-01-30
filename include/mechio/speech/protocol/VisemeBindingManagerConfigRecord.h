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


#ifndef VISEMEBINDINGMANAGERCONFIGRECORD_H_3192739986__H_
#define VISEMEBINDINGMANAGERCONFIGRECORD_H_3192739986__H_


#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

struct VisemePositionRecord {
    int32_t visemeId;
    double position;
};

struct VisemeBindingConfigRecord {
    int32_t bindingId;
    std::vector<VisemePositionRecord > visemeBindings;
};

struct VisemeBindingManagerConfigRecord {
    std::vector<VisemeBindingConfigRecord > visemeBindings;
};

namespace avro {
template<> struct codec_traits<VisemePositionRecord> {
    static void encode(Encoder& e, const VisemePositionRecord& v) {
        avro::encode(e, v.visemeId);
        avro::encode(e, v.position);
    }
    static void decode(Decoder& d, VisemePositionRecord& v) {
        avro::decode(d, v.visemeId);
        avro::decode(d, v.position);
    }
};

template<> struct codec_traits<VisemeBindingConfigRecord> {
    static void encode(Encoder& e, const VisemeBindingConfigRecord& v) {
        avro::encode(e, v.bindingId);
        avro::encode(e, v.visemeBindings);
    }
    static void decode(Decoder& d, VisemeBindingConfigRecord& v) {
        avro::decode(d, v.bindingId);
        avro::decode(d, v.visemeBindings);
    }
};

template<> struct codec_traits<VisemeBindingManagerConfigRecord> {
    static void encode(Encoder& e, const VisemeBindingManagerConfigRecord& v) {
        avro::encode(e, v.visemeBindings);
    }
    static void decode(Decoder& d, VisemeBindingManagerConfigRecord& v) {
        avro::decode(d, v.visemeBindings);
    }
};

}
#endif
