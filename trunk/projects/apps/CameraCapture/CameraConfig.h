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


#ifndef CAMERACONFIG_H_2804996440__H_
#define CAMERACONFIG_H_2804996440__H_


#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

struct CameraConfig {
    std::string brokerAddress;
    std::string connectionOptions;
    std::string eventDestination;
    std::string commandDestination;
    std::string errorDestination;
    int32_t cameraNumber;
    int32_t frameLength;
    int32_t imageWidth;
    int32_t imageHeight;
    int32_t imageChannels;
    int32_t cameraWidth;
    int32_t cameraHeight;
    int32_t cameraChannels;
    bool flipImage;
    bool flipImageHorizontal;
};

namespace avro {
template<> struct codec_traits<CameraConfig> {
    static void encode(Encoder& e, const CameraConfig& v) {
        avro::encode(e, v.brokerAddress);
        avro::encode(e, v.connectionOptions);
        avro::encode(e, v.eventDestination);
        avro::encode(e, v.commandDestination);
        avro::encode(e, v.errorDestination);
        avro::encode(e, v.cameraNumber);
        avro::encode(e, v.frameLength);
        avro::encode(e, v.imageWidth);
        avro::encode(e, v.imageHeight);
        avro::encode(e, v.imageChannels);
        avro::encode(e, v.cameraWidth);
        avro::encode(e, v.cameraHeight);
        avro::encode(e, v.cameraChannels);
        avro::encode(e, v.flipImage);
        avro::encode(e, v.flipImageHorizontal);
    }
    static void decode(Decoder& d, CameraConfig& v) {
        avro::decode(d, v.brokerAddress);
        avro::decode(d, v.connectionOptions);
        avro::decode(d, v.eventDestination);
        avro::decode(d, v.commandDestination);
        avro::decode(d, v.errorDestination);
        avro::decode(d, v.cameraNumber);
        avro::decode(d, v.frameLength);
        avro::decode(d, v.imageWidth);
        avro::decode(d, v.imageHeight);
        avro::decode(d, v.imageChannels);
        avro::decode(d, v.cameraWidth);
        avro::decode(d, v.cameraHeight);
        avro::decode(d, v.cameraChannels);
        avro::decode(d, v.flipImage);
        avro::decode(d, v.flipImageHorizontal);
    }
};

}
#endif
