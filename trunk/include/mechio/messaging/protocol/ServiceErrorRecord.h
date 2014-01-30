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


#ifndef SERVICEERRORRECORD_H_1881126234__H_
#define SERVICEERRORRECORD_H_1881126234__H_


#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

struct ServiceErrorRecord {
    std::string sourceId;
    std::string destinationId;
    int64_t timestampMillisecUTC;
    std::string errorType;
    std::string message;
    std::string details;
};

namespace avro {
template<> struct codec_traits<ServiceErrorRecord> {
    static void encode(Encoder& e, const ServiceErrorRecord& v) {
        avro::encode(e, v.sourceId);
        avro::encode(e, v.destinationId);
        avro::encode(e, v.timestampMillisecUTC);
        avro::encode(e, v.errorType);
        avro::encode(e, v.message);
        avro::encode(e, v.details);
    }
    static void decode(Decoder& d, ServiceErrorRecord& v) {
        avro::decode(d, v.sourceId);
        avro::decode(d, v.destinationId);
        avro::decode(d, v.timestampMillisecUTC);
        avro::decode(d, v.errorType);
        avro::decode(d, v.message);
        avro::decode(d, v.details);
    }
};

}
#endif
