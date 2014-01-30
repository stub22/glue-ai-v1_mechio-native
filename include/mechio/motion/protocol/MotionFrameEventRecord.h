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


#ifndef MOTIONFRAMEEVENTRECORD_H_1923972156__H_
#define MOTIONFRAMEEVENTRECORD_H_1923972156__H_


#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

struct JointIdRecord {
    std::string robotId;
    int32_t jointId;
};

struct JointPositionRecord {
    JointIdRecord jointId;
    double normalizedPosition;
};

struct RobotPositionMapRecord {
    std::vector<JointPositionRecord > jointPositions;
};

struct MotionFrameEventRecord_json_Union__0__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    RobotPositionMapRecord get_RobotPositionMapRecord() const;
    void set_RobotPositionMapRecord(const RobotPositionMapRecord& v);
    bool is_null() const {
        return (idx_ == 1);
    }
    void set_null() {
        idx_ = 1;
        value_ = boost::any();
    }
    MotionFrameEventRecord_json_Union__0__();
};

struct MotionFrameRecord {
    typedef MotionFrameEventRecord_json_Union__0__ startPositions_t;
    int64_t timestampMillisecUTC;
    int64_t moveDurationMillisec;
    RobotPositionMapRecord goalPositions;
    startPositions_t startPositions;
};

struct MotionFrameEventRecord {
    std::string sourceId;
    std::string destinationId;
    int64_t currentTimeMillisecUTC;
    MotionFrameRecord motionFrame;
};

inline
RobotPositionMapRecord MotionFrameEventRecord_json_Union__0__::get_RobotPositionMapRecord() const {
    if (idx_ != 0) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<RobotPositionMapRecord >(value_);
}
inline
void MotionFrameEventRecord_json_Union__0__::set_RobotPositionMapRecord(const RobotPositionMapRecord& v) {
    idx_ = 0;
    value_ = v;
}

inline MotionFrameEventRecord_json_Union__0__::MotionFrameEventRecord_json_Union__0__() : idx_(0), value_(RobotPositionMapRecord()) { }
namespace avro {
template<> struct codec_traits<JointIdRecord> {
    static void encode(Encoder& e, const JointIdRecord& v) {
        avro::encode(e, v.robotId);
        avro::encode(e, v.jointId);
    }
    static void decode(Decoder& d, JointIdRecord& v) {
        avro::decode(d, v.robotId);
        avro::decode(d, v.jointId);
    }
};

template<> struct codec_traits<JointPositionRecord> {
    static void encode(Encoder& e, const JointPositionRecord& v) {
        avro::encode(e, v.jointId);
        avro::encode(e, v.normalizedPosition);
    }
    static void decode(Decoder& d, JointPositionRecord& v) {
        avro::decode(d, v.jointId);
        avro::decode(d, v.normalizedPosition);
    }
};

template<> struct codec_traits<RobotPositionMapRecord> {
    static void encode(Encoder& e, const RobotPositionMapRecord& v) {
        avro::encode(e, v.jointPositions);
    }
    static void decode(Decoder& d, RobotPositionMapRecord& v) {
        avro::decode(d, v.jointPositions);
    }
};

template<> struct codec_traits<MotionFrameEventRecord_json_Union__0__> {
    static void encode(Encoder& e, MotionFrameEventRecord_json_Union__0__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            avro::encode(e, v.get_RobotPositionMapRecord());
            break;
        case 1:
            e.encodeNull();
            break;
        }
    }
    static void decode(Decoder& d, MotionFrameEventRecord_json_Union__0__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 2) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            {
                RobotPositionMapRecord vv;
                avro::decode(d, vv);
                v.set_RobotPositionMapRecord(vv);
            }
            break;
        case 1:
            d.decodeNull();
            v.set_null();
            break;
        }
    }
};

template<> struct codec_traits<MotionFrameRecord> {
    static void encode(Encoder& e, const MotionFrameRecord& v) {
        avro::encode(e, v.timestampMillisecUTC);
        avro::encode(e, v.moveDurationMillisec);
        avro::encode(e, v.goalPositions);
        avro::encode(e, v.startPositions);
    }
    static void decode(Decoder& d, MotionFrameRecord& v) {
        avro::decode(d, v.timestampMillisecUTC);
        avro::decode(d, v.moveDurationMillisec);
        avro::decode(d, v.goalPositions);
        avro::decode(d, v.startPositions);
    }
};

template<> struct codec_traits<MotionFrameEventRecord> {
    static void encode(Encoder& e, const MotionFrameEventRecord& v) {
        avro::encode(e, v.sourceId);
        avro::encode(e, v.destinationId);
        avro::encode(e, v.currentTimeMillisecUTC);
        avro::encode(e, v.motionFrame);
    }
    static void decode(Decoder& d, MotionFrameEventRecord& v) {
        avro::decode(d, v.sourceId);
        avro::decode(d, v.destinationId);
        avro::decode(d, v.currentTimeMillisecUTC);
        avro::decode(d, v.motionFrame);
    }
};

}
#endif
