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


#ifndef IMAGEREGIONS_H_745728776__H_
#define IMAGEREGIONS_H_745728776__H_


#include <boost/any.hpp>
#include <avro/Specific.hh>
#include <avro/Encoder.hh>
#include <avro/Decoder.hh>
struct ImageRegionRecord {
    int32_t regionId;
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
};

struct ImageRegionListRecord {
    std::string imageSourceId;
    int64_t imageId;
    int64_t imageTimestampMillisecUTC;
    std::string imageProcessorId;
    int64_t imageRegionsId;
    int64_t processorStartTimestampMillisecUTC;
    int64_t processorCompleteTimestampMillisecUTC;
    std::vector<ImageRegionRecord> regions;
};

namespace avro {
template<> struct codec_traits<ImageRegionRecord> {
    static void encode(Encoder& e, const ImageRegionRecord& v) {
        avro::encode(e, v.regionId);
        avro::encode(e, v.x);
        avro::encode(e, v.y);
        avro::encode(e, v.width);
        avro::encode(e, v.height);
    }
    static void decode(Decoder& d, ImageRegionRecord& v) {
        avro::decode(d, v.regionId);
        avro::decode(d, v.x);
        avro::decode(d, v.y);
        avro::decode(d, v.width);
        avro::decode(d, v.height);
    }
};

template<> struct codec_traits<ImageRegionListRecord> {
    static void encode(Encoder& e, const ImageRegionListRecord& v) {
        avro::encode(e, v.imageSourceId);
        avro::encode(e, v.imageId);
        avro::encode(e, v.imageTimestampMillisecUTC);
        avro::encode(e, v.imageProcessorId);
        avro::encode(e, v.imageRegionsId);
        avro::encode(e, v.processorStartTimestampMillisecUTC);
        avro::encode(e, v.processorCompleteTimestampMillisecUTC);
        avro::encode(e, v.regions);
    }
    static void decode(Decoder& d, ImageRegionListRecord& v) {
        avro::decode(d, v.imageSourceId);
        avro::decode(d, v.imageId);
        avro::decode(d, v.imageTimestampMillisecUTC);
        avro::decode(d, v.imageProcessorId);
        avro::decode(d, v.imageRegionsId);
        avro::decode(d, v.processorStartTimestampMillisecUTC);
        avro::decode(d, v.processorCompleteTimestampMillisecUTC);
        avro::decode(d, v.regions);
    }
};

}
#endif
