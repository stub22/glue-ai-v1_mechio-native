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


#ifndef MOTIONDETECTCONFIG_H_2558437636__H_
#define MOTIONDETECTCONFIG_H_2558437636__H_


#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

struct MotionDetectConfig {
    std::string brokerAddress;
    std::string connectionOptions;
    std::string imageEventDestination;
    std::string procEventDestination;
    std::string commandDestination;
    std::string errorDestination;
    std::string imageProcessorId;
    std::string imageSourceId;
    int32_t imageWidth;
    int32_t imageHeight;
};

namespace avro {
template<> struct codec_traits<MotionDetectConfig> {
    static void encode(Encoder& e, const MotionDetectConfig& v) {
        avro::encode(e, v.brokerAddress);
        avro::encode(e, v.connectionOptions);
        avro::encode(e, v.imageEventDestination);
        avro::encode(e, v.procEventDestination);
        avro::encode(e, v.commandDestination);
        avro::encode(e, v.errorDestination);
        avro::encode(e, v.imageProcessorId);
        avro::encode(e, v.imageSourceId);
        avro::encode(e, v.imageWidth);
        avro::encode(e, v.imageHeight);
    }
    static void decode(Decoder& d, MotionDetectConfig& v) {
        avro::decode(d, v.brokerAddress);
        avro::decode(d, v.connectionOptions);
        avro::decode(d, v.imageEventDestination);
        avro::decode(d, v.procEventDestination);
        avro::decode(d, v.commandDestination);
        avro::decode(d, v.errorDestination);
        avro::decode(d, v.imageProcessorId);
        avro::decode(d, v.imageSourceId);
        avro::decode(d, v.imageWidth);
        avro::decode(d, v.imageHeight);
    }
};

}
#endif
