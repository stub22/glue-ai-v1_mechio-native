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


#ifndef SENSORPROTOCOL2_H_4101677906__H_
#define SENSORPROTOCOL2_H_4101677906__H_


#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

struct HeaderRecord {
    int64_t timestamp;
    int32_t sequenceId;
    int32_t frameId;
};

struct Vector3Record {
    double x;
    double y;
    double z;
};

struct StampedVector3Record {
    HeaderRecord header;
    Vector3Record vector;
};

struct FilteredVector3Record {
    HeaderRecord header;
    Vector3Record rawVector;
    Vector3Record filteredVector;
};

struct DeviceAnalogRecord {
    HeaderRecord header;
    int32_t channelId;
    double analogValue;
};

struct DeviceBoolRecord {
    HeaderRecord header;
    int32_t channelId;
    bool boolValue;
};

struct GyroConfigRecord {
    GyroConfigRecord() : ctl1(-1), ctl2(-1), ctl3(-1), ctl4(-1), ctl5(-1) {};
    HeaderRecord header;
    int32_t ctl1;
    int32_t ctl2;
    int32_t ctl3;
    int32_t ctl4;
    int32_t ctl5;
};

struct AccelerometerConfigRecord {
    HeaderRecord header;
    int32_t reg;
    int32_t value;
};

struct CompassConfigRecord {
    HeaderRecord header;
	int32_t average;		//# of samples to average together: 0=1, 1=2, 2=4, 3=8
	int32_t rate;			//sample rate: 0=.75, 1=1.5, 2=3, 3=7.5, 4=15, 5=30, 6=75
	int32_t bias;			// 0=normal; 1=positive; 2=negative
	int32_t gain;			//(LSB/Gauss): 0=1370 to 7=230
};

struct AdcConfigRecord {
    HeaderRecord header;
    int32_t channelMask;     // channels
};

struct GpioConfigRecord {
    HeaderRecord header;
    int32_t inputMask;     // Bits to configure for input
};

struct DeviceReadPeriodRecord {
    HeaderRecord header;
    double period;
};

/*
struct DeviceScheduleRecord {
    HeaderRecord header;
    int32_t deviceAddress;
    bool enabled;
    int64_t readIntervalMillisec;
};

struct I2CcommandRecord {
    HeaderRecord header;
    int32_t deviceAddress;
    std::vector<uint8_t> commandData;
    int32_t readLength;
};

struct I2CDataRecord {
    HeaderRecord header;
    HeaderRecord commandheader;
    int32_t deviceAddress;
    std::vector<uint8_t> data;
};
*/
        
struct RGBOutputRecord {
    HeaderRecord header;
    int32_t channelId;
    int32_t red;
    int32_t green;
    int32_t blue;
};

struct I2CQpidConfigRecord {
    std::string brokerIPAddress;
    std::string brokerOptions;
    std::string accelerometerReadDestination;
    std::string accelerometerEventDestination;
    std::string accelerometerConfigDestination;
    std::string compassReadDestination;
    std::string compassEventDestination;
    std::string compassConfigDestination;
    std::string gyroReadDestination;
    std::string gyroEventDestination;
    std::string gyroConfigDestination;
    std::string gpioReadDestination;
    std::string gpioWriteDestination;
    std::string gpioEventDestination;
    std::string gpioConfigDestination;
    std::string adcReadDestination;
    std::string adcEventDestination;
    std::string adcConfigDestination;
    std::string ledConfigDestination;
};

namespace avro {
template<> struct codec_traits<HeaderRecord> {
    static void encode(Encoder& e, const HeaderRecord& v) {
        avro::encode(e, v.timestamp);
        avro::encode(e, v.sequenceId);
        avro::encode(e, v.frameId);
    }
    static void decode(Decoder& d, HeaderRecord& v) {
        avro::decode(d, v.timestamp);
        avro::decode(d, v.sequenceId);
        avro::decode(d, v.frameId);
    }
};

template<> struct codec_traits<Vector3Record> {
    static void encode(Encoder& e, const Vector3Record& v) {
        avro::encode(e, v.x);
        avro::encode(e, v.y);
        avro::encode(e, v.z);
    }
    static void decode(Decoder& d, Vector3Record& v) {
        avro::decode(d, v.x);
        avro::decode(d, v.y);
        avro::decode(d, v.z);
    }
};

template<> struct codec_traits<StampedVector3Record> {
    static void encode(Encoder& e, const StampedVector3Record& v) {
        avro::encode(e, v.header);
        avro::encode(e, v.vector);
    }
    static void decode(Decoder& d, StampedVector3Record& v) {
        avro::decode(d, v.header);
        avro::decode(d, v.vector);
    }
};

template<> struct codec_traits<FilteredVector3Record> {
    static void encode(Encoder& e, const FilteredVector3Record& v) {
        avro::encode(e, v.header);
        avro::encode(e, v.rawVector);
        avro::encode(e, v.filteredVector);
    }
    static void decode(Decoder& d, FilteredVector3Record& v) {
        avro::decode(d, v.header);
        avro::decode(d, v.rawVector);
        avro::decode(d, v.filteredVector);
    }
};

template<> struct codec_traits<DeviceAnalogRecord> {
    static void encode(Encoder& e, const DeviceAnalogRecord& v) {
        avro::encode(e, v.header);
        avro::encode(e, v.channelId);
        avro::encode(e, v.analogValue);
    }
    static void decode(Decoder& d, DeviceAnalogRecord& v) {
        avro::decode(d, v.header);
        avro::decode(d, v.channelId);
        avro::decode(d, v.analogValue);
    }
};

template<> struct codec_traits<DeviceBoolRecord> {
    static void encode(Encoder& e, const DeviceBoolRecord& v) {
        avro::encode(e, v.header);
        avro::encode(e, v.channelId);
        avro::encode(e, v.boolValue);
    }
    static void decode(Decoder& d, DeviceBoolRecord& v) {
        avro::decode(d, v.header);
        avro::decode(d, v.channelId);
        avro::decode(d, v.boolValue);
    }
};

template<> struct codec_traits<AccelerometerConfigRecord> {
    static void encode(Encoder& e, const AccelerometerConfigRecord& v) {
        avro::encode(e, v.reg);
        avro::encode(e, v.value);
    }
    static void decode(Decoder& d, AccelerometerConfigRecord& v) {
        avro::decode(d, v.reg);
        avro::decode(d, v.value);
    }
};

template<> struct codec_traits<GyroConfigRecord> {
    static void encode(Encoder& e, const GyroConfigRecord& v) {
        avro::encode(e, v.ctl1);
        avro::encode(e, v.ctl2);
        avro::encode(e, v.ctl3);
        avro::encode(e, v.ctl4);
        avro::encode(e, v.ctl5);
    }
    static void decode(Decoder& d, GyroConfigRecord& v) {
        avro::decode(d, v.ctl1);
        avro::decode(d, v.ctl2);
        avro::decode(d, v.ctl3);
        avro::decode(d, v.ctl4);
        avro::decode(d, v.ctl5);
    }
};

template<> struct codec_traits<CompassConfigRecord> {
    static void encode(Encoder& e, const CompassConfigRecord& v) {
        avro::encode(e, v.header);
        avro::encode(e, v.average);
        avro::encode(e, v.rate);
        avro::encode(e, v.bias);
        avro::encode(e, v.gain);
    }
    static void decode(Decoder& d, CompassConfigRecord& v) {
        avro::decode(d, v.header);
        avro::decode(d, v.average);
        avro::decode(d, v.rate);
        avro::decode(d, v.bias);
        avro::decode(d, v.gain);
    }
};

template<> struct codec_traits<AdcConfigRecord> {
    static void encode(Encoder& e, const AdcConfigRecord& v) {
        avro::encode(e, v.header);
        avro::encode(e, v.channelMask);
    }
    static void decode(Decoder& d, AdcConfigRecord& v) {
        avro::decode(d, v.header);
        avro::decode(d, v.channelMask);
    }
};

template<> struct codec_traits<GpioConfigRecord> {
    static void encode(Encoder& e, const GpioConfigRecord& v) {
        avro::encode(e, v.header);
        avro::encode(e, v.inputMask);
    }
    static void decode(Decoder& d, GpioConfigRecord& v) {
        avro::decode(d, v.header);
        avro::decode(d, v.inputMask);
    }
};

template<> struct codec_traits<DeviceReadPeriodRecord> {
    static void encode(Encoder& e, const DeviceReadPeriodRecord& v) {
        avro::encode(e, v.header);
        avro::encode(e, v.period);
    }
    static void decode(Decoder& d, DeviceReadPeriodRecord& v) {
        avro::decode(d, v.header);
        avro::decode(d, v.period);
    }
};
/*
template<> struct codec_traits<DeviceConfigRecord> {
    static void encode(Encoder& e, const DeviceConfigRecord& v) {
        avro::encode(e, v.header);
        avro::encode(e, v.deviceAddress);
        avro::encode(e, v.enabled);
        avro::encode(e, v.readIntervalMillisec);
    }
    static void decode(Decoder& d, DeviceConfigRecord& v) {
        avro::decode(d, v.header);
        avro::decode(d, v.deviceAddress);
        avro::decode(d, v.enabled);
        avro::decode(d, v.readIntervalMillisec);
    }
};

template<> struct codec_traits<I2CcommandRecord> {
    static void encode(Encoder& e, const I2CcommandRecord& v) {
        avro::encode(e, v.header);
        avro::encode(e, v.deviceAddress);
        avro::encode(e, v.commandData);
        avro::encode(e, v.readLength);
    }
    static void decode(Decoder& d, I2CcommandRecord& v) {
        avro::decode(d, v.header);
        avro::decode(d, v.deviceAddress);
        avro::decode(d, v.commandData);
        avro::decode(d, v.readLength);
    }
};

template<> struct codec_traits<I2CDataRecord> {
    static void encode(Encoder& e, const I2CDataRecord& v) {
        avro::encode(e, v.header);
        avro::encode(e, v.commandheader);
        avro::encode(e, v.deviceAddress);
        avro::encode(e, v.data);
    }
    static void decode(Decoder& d, I2CDataRecord& v) {
        avro::decode(d, v.header);
        avro::decode(d, v.commandheader);
        avro::decode(d, v.deviceAddress);
        avro::decode(d, v.data);
    }
};
*/
template<> struct codec_traits<RGBOutputRecord> {
    static void encode(Encoder& e, const RGBOutputRecord& v) {
        avro::encode(e, v.header);
        avro::encode(e, v.channelId);
        avro::encode(e, v.red);
        avro::encode(e, v.green);
        avro::encode(e, v.blue);
    }
    static void decode(Decoder& d, RGBOutputRecord& v) {
        avro::decode(d, v.header);
        avro::decode(d, v.channelId);
        avro::decode(d, v.red);
        avro::decode(d, v.green);
        avro::decode(d, v.blue);
    }
};

template<> struct codec_traits<I2CQpidConfigRecord> {
    static void encode(Encoder& e, const I2CQpidConfigRecord& v) {
        avro::encode(e, v.brokerIPAddress);
        avro::encode(e, v.brokerOptions);
        avro::encode(e, v.accelerometerReadDestination);
        avro::encode(e, v.accelerometerEventDestination);
        avro::encode(e, v.accelerometerConfigDestination);
        avro::encode(e, v.compassReadDestination);
        avro::encode(e, v.compassEventDestination);
        avro::encode(e, v.compassConfigDestination);
        avro::encode(e, v.gyroReadDestination);
        avro::encode(e, v.gyroEventDestination);
        avro::encode(e, v.gyroConfigDestination);
        avro::encode(e, v.gpioReadDestination);
        avro::encode(e, v.gpioWriteDestination);
        avro::encode(e, v.gpioEventDestination);
        avro::encode(e, v.gpioConfigDestination);
        avro::encode(e, v.adcReadDestination);
        avro::encode(e, v.adcEventDestination);
        avro::encode(e, v.adcConfigDestination);
        avro::encode(e, v.ledConfigDestination);
    }
    static void decode(Decoder& d, I2CQpidConfigRecord& v) {
        avro::decode(d, v.brokerIPAddress);
        avro::decode(d, v.brokerOptions);
        avro::decode(d, v.accelerometerReadDestination);
        avro::decode(d, v.accelerometerEventDestination);
        avro::decode(d, v.accelerometerConfigDestination);
        avro::decode(d, v.compassReadDestination);
        avro::decode(d, v.compassEventDestination);
        avro::decode(d, v.compassConfigDestination);
        avro::decode(d, v.gyroReadDestination);
        avro::decode(d, v.gyroEventDestination);
        avro::decode(d, v.gyroConfigDestination);
        avro::decode(d, v.gpioReadDestination);
        avro::decode(d, v.gpioWriteDestination);
        avro::decode(d, v.gpioEventDestination);
        avro::decode(d, v.gpioConfigDestination);
        avro::decode(d, v.adcReadDestination);
        avro::decode(d, v.adcEventDestination);
        avro::decode(d, v.adcConfigDestination);
        avro::decode(d, v.ledConfigDestination);
    }
};

}
#endif