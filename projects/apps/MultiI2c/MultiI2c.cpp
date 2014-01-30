/* 
 * File:   main.cpp
 * Author: matt, dbm
 *
 * Created on November 12, 2011, 3:59 PM
 */

#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include "mechio/messaging/MessagingProvider.h"
#include "mechio/messaging/MessageSender.h"
#include "mechio/messaging/RemoteService.h"
#include "mechio/messaging/RemoteServiceHost.h"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"
#include "avro/Compiler.hh"
#include "mechio/i2c/protocol/SensorProtocol.h"
#include "mechio/i2c/i2c.h"
#include "MultiI2c.h"
#include <boost/thread.hpp>

#define FAKE false

using namespace std;
using namespace qpid::messaging;
using namespace mechio;

int msgcnt=0;

boost::posix_time::ptime t1970(boost::gregorian::date(1970,1,1));
double MsStartSession;      // make global for debugging
#define MsSince1970 ((boost::posix_time::microsec_clock().local_time()-t1970).total_milliseconds())

bool fetchIfReady(MessageReceiver* recv, Message& msg) {
    try {
        msg = recv->fetchRaw(Duration(0));
        recv->acknowledge();
        return true;
    } catch (const NoMessageAvailable &ex) {
        return false;
    }
}

SensorBase::SensorBase() : period(-1), config(NULL), read(NULL), write(NULL), event(NULL), initRequest(false) {}

void SensorBase::setPeriod(double p) {
    period = p;
}

double SensorBase::getPeriod() {
    return period;
}

void SensorBase::initMsg(MessagingProvider *strm, string _config, string _read, string _event, string _write) {
    cout << "initMsg config=" << _config << " read=" << _read << " event=" << _event << " write=" << _write << endl; 
    config = strm->createReceiver(_config.c_str());
    read = strm->createReceiver(_read.c_str());
    write = strm->createReceiver(_write.c_str());
    event = strm->createSender(_event.c_str());
    time = 0;                           // need to do it somewhere
    timer = 0;
}

void SensorBase::initMsg(MessagingProvider *strm, string _config, string _read, string _event) {
    cout << "initMsg config=" << _config << " read=" << _read << " event=" << _event << endl; 
    config = strm->createReceiver(_config.c_str());
    read = strm->createReceiver(_read.c_str());
    event = strm->createSender(_event.c_str());
    time = 0;                           // need to do it somewhere
    timer = 0;
}

void SensorBase::initMsg(MessagingProvider *strm, string _config) {
    cout << "initMsg config=" << _config << endl; 
    config = strm->createReceiver(_config.c_str());
    time = 0;                           // need to do it somewhere
    timer = 0;
}

bool SensorBase::updateTimer(double dt) {
    time += dt;
    if (time >= timer + period) {
        timer += period;
        if (time > timer + period) {
            timer = time;              // don't stack 'em up
        }
        return true;
    }
    return false;
}

void Gyro::poll() {
    if (config==NULL) return;
    Message msg; 
    if(fetchIfReady(config, msg)) {
        GyroConfigRecord* cmd = unpackMessage<GyroConfigRecord>(msg);
        ctl1 = cmd->ctl1;
        ctl2 = cmd->ctl2;
        ctl3 = cmd->ctl3;
        ctl4 = cmd->ctl4;
        ctl5 = cmd->ctl5;
        cout << "Gyro Config ctl regs:" << ctl1 << ", " << ctl2 << ", " 
                 << ctl3 << ", "  << ctl4 << ", " << ctl5 << endl;
        initRequest = true;
    }
    if(fetchIfReady(read, msg)) {
        DeviceReadPeriodRecord* per = unpackMessage<DeviceReadPeriodRecord>(msg);
        setPeriod(per->period);
        cout << "Gyro Read set period: " << getPeriod() << endl;
    }
}

void Gyro::update(double dt) {
    if(initRequest) {
        init();
        initRequest = false;
    }
    bool sendReading = false;
    if(getPeriod() == 0) {
        cout << "Gyro single-shot read" << endl;
        sendReading = true;
        setPeriod(-1);
    }
    if(getPeriod() > 0) {
        if(updateTimer(dt)) {
//            cout << MsSince1970-MsStartSession << " Gyro periodic" << endl;
            sendReading = true;
        }
    }
    if(sendReading) {
        HeaderRecord header;
        FilteredVector3Record msg;
        Vector3Record rvec;
        Vector3Record fvec;

        i2cSetDev(wr, 3);
        char* resp = i2cRead(wr, 0xa8, 6);                                // read 6 bytes for XYZ data
        
        // convert to deg per sec (assumes default setting of 250 DPS = full scale
        double mul = 250.0 / 65536.0;
        rvec.x = ((double)chars2intLE(resp))*mul;
        rvec.y = ((double)chars2intLE(resp+2))*mul;
        rvec.z = ((double)chars2intLE(resp+4))*mul;

        // DO Kalman magic here
        fvec.x = rvec.x;
        fvec.y = rvec.y;
        fvec.z = rvec.z;
        
        msg.rawVector = rvec;
        msg.filteredVector = fvec;
        header.sequenceId = wr >> 1;                            // chip ID -- use i2c addr for now
        header.frameId = msgcnt++;
        header.timestamp = MsSince1970;
        msg.header = header;
        cout << "sending Gyro read event " << rvec.x << ", " << rvec.y << ", " << rvec.z << endl;
        event->send(&msg);
        free(resp);
    }
}

bool Gyro::test() {
    i2cSetDev(wr, 3);
    char* resp = i2cRead(wr, 0x0f, 1);
    if (resp == NULL)
    {
        mechio::common::error("ERROR i2cRead\n");
        return false;
    }
    if ((unsigned char) resp[0] == 0xd3)
    {
        free(resp);
        return true;
    } else {
        mechio::common::error("This is NOT an ST Micro gyro");
        free(resp);
        return false;
    }
}

bool Gyro::init() {
    test();       // this seems to grease the wheels
    i2cSetDev(wr, 3);
    if (ctl1 != -1) {
        if (i2cWrite(wr, 2, 0x20, ctl1)) {
            mechio::common::error("Gyro: failure to write control reg 1\n");
            return false;
        }
    }
    if (ctl2 != -1) {
        if (i2cWrite(wr, 2, 0x21, ctl2)) {
            mechio::common::error("Gyro: failure to write control reg 2\n");
            return false;
        }
    }
    if (ctl3 != -1) {
        if (i2cWrite(wr, 2, 0x22, ctl3)) {
            mechio::common::error("Gyro: failure to write control reg 3\n");
            return false;
        }
    }
    if (ctl4 != -1) {
        if (i2cWrite(wr, 2, 0x23, ctl4)) {
            mechio::common::error("Gyro: failure to write control reg 4\n");
            return false;
        }
    }
    if (ctl5 != -1) {
        if (i2cWrite(wr, 2, 0x24, ctl5)) {
            mechio::common::error("Gyro: failure to write control reg 5\n");
            return false;
        }
    }
    return true;
}

bool Gyro::readInt(int& x) {
    mechio::common::error("ERROR: Gyro::read not implemented\n");
}

bool Gyro::readXYZ(int& x, int& y, int& z) {
    i2cSetDev(wr, 3);
    char* resp = i2cRead(wr, 0xa8, 6);                                // read 6 bytes for XYZ data
    if (resp == NULL) {
        return false;
    }
    x = chars2intLE(resp);
    y = chars2intLE(resp+2);
    z = chars2intLE(resp+4);
    free(resp);
    return true;
}

int Gyro::getAddr() {
    return wr >> 1;
}

string Gyro::getName() {
    return "Gyro";
}

Accelerometer::Accelerometer() : initialized(false) {}

void Accelerometer::poll() {
    if (config==NULL) return;
    Message msg; 
    if(fetchIfReady(config, msg)) {
        AccelerometerConfigRecord* cmd = unpackMessage<AccelerometerConfigRecord>(msg);
        reg = cmd->reg;
        value = cmd->value;
        cout << "Accelerometer Config register:" << reg << " value: " << value << endl;
        initRequest = true;
    }
    if(fetchIfReady(read, msg)) {
        DeviceReadPeriodRecord* per = unpackMessage<DeviceReadPeriodRecord>(msg);
        setPeriod(per->period);
        cout << "Accelerometer Read set period: " << getPeriod() << endl;
    }
}

void Accelerometer::update(double dt) {
    if(initRequest) {
        init();
        initRequest = false;
    }
    bool sendReading = false;
    if(getPeriod() == 0) {
        cout << "Accelerometer single-shot read" << endl;
        sendReading = true;
        setPeriod(-1);
    }
    if(getPeriod() > 0) {
        if(updateTimer(dt)) {
//            cout << MsSince1970-MsStartSession << " Accel periodic" << endl;
            sendReading = true;
        }
    }
    
    if(sendReading) {
        HeaderRecord header;
        FilteredVector3Record msg;
        Vector3Record rvec;
        Vector3Record fvec;

        i2cSetDev(wr, 3);
        i2cStop();
        char *resp = i2cRead(wr, 0x32, 6);                                // read 6 bytes for XYZ data

        // convert to meters/sec -- assumes full res, gain = +/- 16 (our default settings)
        rvec.x = ((double)chars2intLE(resp)) / 256.0;
        rvec.y = ((double)chars2intLE(resp+2)) / 256.0;
        rvec.z = ((double)chars2intLE(resp+4)) / 256.0;

        // DO Kalman magic here
        fvec.x = rvec.x;
        fvec.y = rvec.y;
        fvec.z = rvec.z;
        
        msg.rawVector = rvec;
        msg.filteredVector = fvec;
        header.sequenceId = wr >> 1;                            // chip ID -- use i2c addr for now
        header.frameId = msgcnt++;
        header.timestamp = MsSince1970;
        msg.header = header;
        cout << "sending Accelerometer read event " << rvec.x << ", " << rvec.y << ", " << rvec.z << endl;
        event->send(&msg);
        free(resp);
    }
}

bool Accelerometer::test() {
    i2cSetDev(wr, 3);
    i2cStop();                                                       // not clear why necessary
    char* resp = i2cRead(wr, 0x00, 1);
    if (resp == NULL)
    {
        mechio::common::error("ERROR i2cRead\n");
        return false;
    }
    if ((unsigned char) resp[0] == 0xe5)
    {
        free(resp);
        return true;
    } else {
        mechio::common::error("This is not an Analog Devices accelerometer\n");
        free(resp);
        return false;
    }
}

bool Accelerometer::init() {
    i2cSetDev(wr, 3);
    i2cStop();                                                       // not clear why necessary
    if (!initialized) {                                              // not clear the chip has a default, so for safety
        if (i2cWrite(wr, 2, 0x31, 0xb)) {                            // full-res, high gain
            mechio::common::error("Failure to write control reg\n");
            return false;
        }
        initialized = true;
    }
    if (reg != -1) {
        if (i2cWrite(wr, 2, reg, value)) {                            // wake up & start producing data
            mechio::common::error("Failure to write control reg\n");
            return false;
        }
    }
    return true;
}

bool Accelerometer::readInt(int& x) {
    mechio::common::error("ERROR: Accerlerometer::read not implemented\n");
}

bool Accelerometer::readXYZ(int& x, int& y, int& z) {
    i2cSetDev(wr, 3);
    i2cStop();
    char* resp = i2cRead(wr, 0x32, 6);                                // read 6 bytes for XYZ data
    if (resp == NULL) {
        return false;
    }
    x = chars2intLE(resp);
    y = chars2intLE(resp+2);
    z = chars2intLE(resp+4);
    free(resp);
    return true;
}

int Accelerometer::getAddr() {
    return wr >> 1;
}

string Accelerometer::getName() {
    return "Accelerometer";
}

void Compass::poll() {
    if (config==NULL) return;
    Message msg;
    if(fetchIfReady(config, msg)) {
        CompassConfigRecord* cmd = unpackMessage<CompassConfigRecord>(msg);
        average = cmd->average;
        rate = cmd->rate;
        bias = cmd->bias;
        gain = cmd->gain;
        cout << "Compass Config average=" << average <<
                " rate=" << rate << " bias=" << bias << " gain=" << gain << endl;
        initRequest = true;
    }
    if(fetchIfReady(read, msg)) {
        DeviceReadPeriodRecord* per = unpackMessage<DeviceReadPeriodRecord>(msg);
        setPeriod(per->period);
        cout << "Compass Read set period: " << getPeriod() << endl;
    }
}

void Compass::update(double dt) {
    if(initRequest) {
        init();
        initRequest = false;
    }
    bool sendReading = false;
    if(getPeriod() == 0) {
        cout << "Compass single-shot read" << endl;
        sendReading = true;
        setPeriod(-1);
    }
    if(getPeriod() > 0) {
        if(updateTimer(dt)) {
//            cout << MsSince1970-MsStartSession << " Compass periodic" << endl;
            sendReading = true;
        }
    }
    
    if(sendReading) {
        HeaderRecord header;
        FilteredVector3Record msg;
        Vector3Record rvec;
        Vector3Record fvec;
        i2cSetDev(wr, 3);
        init();                                                         // in theory this shouldn't be necessary
        char* resp = i2cRead(wr, 9, 1);                                // reg 9 bit 0 == data ready; bit 1 == lock
        int j;
        for (j=0; j<6; j++) {
            if( (resp!=NULL) && ((resp[0] & 0x03) == 1) ) {
                i2cSetDev(wr, 3);
                char* resp2 = i2cRead(wr, 3, 6);                            // read 6 bytes for XYZ data
    //            i2cWrite(wr, 2, 2, 1);                                      // set to continuous read -- WHY? WHY? WHY???

                // convert to milligauss
                double mul;
                switch (gain) {
                    case 0: mul = 1000.0 / 1370; break;
                    case 1: mul = 1000.0 / 1090; break;
                    case 2: mul = 1000.0 / 820; break;
                    case 3: mul = 1000.0 / 660; break;
                    case 4: mul = 1000.0 / 440; break;
                    case 5: mul = 1000.0 / 390; break;
                    case 6: mul = 1000.0 / 330; break;
                    case 7: mul = 1000.0 / 230; break;
                }
                rvec.x = mul * (double)chars2int(resp2);
                rvec.z = mul * (double)chars2int(resp2+2);
                rvec.y = mul * (double)chars2int(resp2+4);

                // DO Kalman magic here
                fvec.x = rvec.x;
                fvec.y = rvec.y;
                fvec.z = rvec.z;

                msg.rawVector = rvec;
                msg.filteredVector = fvec;
                header.sequenceId = wr >> 1;                            // chip ID -- use i2c addr for now
                header.frameId = msgcnt++;
                header.timestamp = MsSince1970;
                msg.header = header;
                cout << "sending Compass read event " << rvec.x << ", " << rvec.y << ", " << rvec.z << endl;
                event->send(&msg);
                free(resp2);
                i2cSetDev(wr, 3);
                i2cWrite(wr, 2, 2, 1);                                      // ask for another reading (continuous seems broke)
                break;
            }
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        }
        if (j==6) {
            cout << "ERROR -- Compass: multiple read failures/data not ready" << endl;
        }
        free(resp);
    }
}

bool Compass::test() {
    i2cStop();
    i2cSetDev(wr, 3);
    char* resp = i2cRead(wr, 10, 3);
    if (resp == NULL)
    {
        mechio::common::error("ERROR i2cRead\n");
        return false;
    }
    if (resp[0] == 'H' && resp[1] == '4' && resp[2] == '3')
    {
        free(resp);
        return true;
    } else {
        mechio::common::error("This is not a Honeywell compass\n");
        free(resp);
        return false;
    }
    free(resp);
}

bool Compass::init() {
    int success=0;
    for(int i=0; i<3; i++) {                                         // for some reason the chip fails first time trying to access these registers
        i2cSetDev(wr, 3);
//        int cmd = (1<<(average+5)) | (1<<(rate+2)) | (1<<(bias));
        unsigned int cmd = (average << 5) | (rate<<2) | bias;
        if (!i2cWrite(wr, 2, 0, cmd) ) {
            success |= 1;
        }

        i2cSetDev(wr, 3);
        cmd = gain << 5;
        if (!i2cWrite(wr, 2, 1, 1 << cmd)) {
            success |= 2;
        }

        i2cSetDev(wr, 3);
        if (!i2cWrite(wr, 2, 2, 1)) {                               // set to continuous read
            success |= 4;
        }
        if (success==7) break;
    }
    if(success != 7) {
        mechio::common::error("ERROR: multiple ack failures setting compass registers");
        return false;
    }
    return true;
}

bool Compass::readInt(int& x) {
    mechio::common::error("ERROR: Compass::read not implemented\n");
}

bool Compass::readXYZ(int& x, int& y, int& z) {
    for(int i=0; i<3; i++) {
        i2cSetDev(wr, 3);
        if (i2cWrite(wr, 2, 2, 0x01)) {                            // start continuous data
            if (i==2) {
                mechio::common::error("ERROR compass read data ack failed (nonfatal)\n");
                return false;                                          // this seems to happen occasionally
            }
        }
    }
    char* resp = i2cRead(wr, 9, 1);                                // reg 9 bit 0 == data ready; bit 1 == lock
    if (resp == NULL) {
        return false;
    }
    if((resp[0] & 0x03) == 1)
    {
        i2cSetDev(wr, 3);
        char* resp2 = i2cRead(wr, 3, 6);                            // read 6 bytes for XYZ data
        if (resp == NULL) {
            return false;
        }
        x = chars2int(resp2);
        z = chars2int(resp2+2);
        y = chars2int(resp2+4);
    }
    free(resp);
    return true;
}

int Compass::getAddr() {
    return wr >> 1;
}

string Compass::getName() {
    return "Compass";
}

Gpio::Gpio() : SensorBase::SensorBase(), active(0), output(0), outRequest(0) {}

void Gpio::poll() {
    if (config==NULL) return;
    Message msg; 
    if(fetchIfReady(config, msg)) {
        GpioConfigRecord* cmd = unpackMessage<GpioConfigRecord>(msg);
        cout << "GpioConfig mask: " << cmd->inputMask << endl;
        active = cmd->inputMask;
        initRequest = true;
    }
    if(fetchIfReady(read, msg)) {
        DeviceReadPeriodRecord* per = unpackMessage<DeviceReadPeriodRecord>(msg);
        setPeriod(per->period);
        cout << "Gpio Read set period: " << getPeriod() << endl;
    }
    if(fetchIfReady(write, msg)) {
        DeviceBoolRecord* boo = unpackMessage<DeviceBoolRecord>(msg);
        if (boo->boolValue) {
            output |= (1 << boo->channelId);
        }
        else {
            output &= (0xffff ^ (1 << boo->channelId));
        }
        outRequest = true;
        cout << "Gpio Write channel: " << boo->channelId << " value: " << boo->boolValue << " output: " << output << endl;
    }
}

void Gpio::update(double dt) {
    if(initRequest) {
        init();
        initRequest = false;
    }
    if (outRequest) {
        setOut(wr, output);
        outRequest = false;
    }
    bool sendReading = false;
    if(getPeriod() == 0) {
        cout << "Gpio single-shot read" << endl;
        sendReading = true;
        setPeriod(-1);
    }
    if(getPeriod() > 0) {
        if(updateTimer(dt)) {
//            cout << MsSince1970-MsStartSession << " Gpio periodic" << endl;
            sendReading = true;
        }
    }
    
    if(sendReading) {
        HeaderRecord header;
        DeviceBoolRecord boo;
        i2cSetDev(wr, 3);
        setIO(wr, active);                                                  // nasty. not tasty
        setOut(wr, output);
        char* resp = i2cRead(wr, 0x00, 2);                                // read 2 bytes
        int x = chars2int(resp);
        printf("GPIO input reading: %x\n", x);
        for (int i=0; i<16; i++) {
            if (active & (1<<i)) {
                bool chanVal = (x & (1 << i)) != 0;
                bool send = false;
                if(!channelVals[i][0]){
                    channelVals[i][0] = true;
                    channelVals[i][1] = chanVal;
                    send = true;
                }else{
                    send = channelVals[i][1] != chanVal;
                    channelVals[i][1] = chanVal;
                }
                if(send){
                    boo.boolValue = chanVal;
                    boo.channelId = i;
                    header.sequenceId = wr >> 1;                            // chip ID -- use i2c addr for now
                    header.frameId = msgcnt++;
                    header.timestamp = MsSince1970;
                    boo.header = header;
                    cout << "sending Gpio read event for channel " << i << " value=" << boo.boolValue << endl;
                    event->send(&boo);
                }
            }
        }
        free(resp);
    }
}

void Gpio::setIO(int wr, int io) {
    i2cSetDev(wr, 3);
    if (i2cWrite(wr, 2, 0x03, io & 0xff)) {                            // set I/O direction bits 0-7
        printf("Failure to write control reg\n");
    }
    i2cSetDev(wr, 3);
    if (i2cWrite(wr, 2, 0x02, io >> 8)) {                            // set I/O direction bits 8-15
        printf("Failure to write control reg\n");
    }
}

void Gpio::setOut(int wr, int out) {
    i2cSetDev(wr, 3);
    if (i2cWrite(wr, 2, 0x01, out & 0xff)) {
        printf("Failure to write control reg\n");
    }
    i2cSetDev(wr, 3);
    if (i2cWrite(wr, 2, 0x00, out >> 8)) {
        printf("Failure to write control reg\n");
    }
}

bool Gpio::test() {
    mechio::common::error("WARNING: no known test for Gpio\n");
    return true;
}

bool Gpio::init() {
    for(int i=0; i<16; i++){
        channelVals[i][0] = false;
    }
    setIO(wr, active);
    setOut(wr, output);
}

bool Gpio::readInt(int& x) {
    char* resp = i2cRead(wr, 0x00, 2);                                // read 2 bytes
    if (resp == NULL) {
        return false;
    }
    x = chars2int(resp);
    free(resp);
}

bool Gpio::readXYZ(int& x, int& y, int& z) {
    mechio::common::error("ERROR: Gpio::readXYZ not implemented\n");
}

int Gpio::getAddr() {
    return wr >> 1;
}

string Gpio::getName() {
    return "Gpio";
}

Adc::Adc() : SensorBase::SensorBase(), active(0) {}

void Adc::poll() {
    if (config==NULL) return;
    Message msg; 
    if(fetchIfReady(config, msg)) {
        AdcConfigRecord* cmd = unpackMessage<AdcConfigRecord>(msg);
        cout << "AdcConfig mask: " << cmd->channelMask << endl;
        active = cmd->channelMask;
        initRequest = true;
    }
    if(fetchIfReady(read, msg)) {
        DeviceReadPeriodRecord* per = unpackMessage<DeviceReadPeriodRecord>(msg);
        setPeriod(per->period);
        cout << "Adc Read set period: " << getPeriod() << endl;
    }
}

void Adc::update(double dt) {
    if(initRequest) {
        init();
        initRequest = false;
    }
    bool sendReading = false;
    if(getPeriod() == 0) {
        cout << "Compass single-shot read" << endl;
        sendReading = true;
        setPeriod(-1);
    }
    if(getPeriod() > 0) {
        if(updateTimer(dt)) {
//            cout << MsSince1970-MsStartSession << " here is where Adc needs to do things" << endl;
            sendReading = true;
        }
    }
    
    if(sendReading) {
        HeaderRecord header;
        DeviceAnalogRecord val;
        if (active != 0) {
            i2cSetDev(wr, 3);
            for (int i=0; i<8; i++) {
                if (active & (1<<i)) {
                    int cmd = 0x8c | (i<<4);                   // cmd reg: bit 7 = single-ended, 6-4 = channel, 3-2 = power mode
                    char* resp = i2cRead(wr, cmd, 2);             // read 2 bytes of data
                    int x = chars2int(resp);
                    val.analogValue = (double) x;
                    val.channelId = i;
                    header.sequenceId = wr >> 1;             // chip ID -- use i2c addr for now
                    header.frameId = msgcnt++;
                    header.timestamp = MsSince1970;
                    val.header = header;
                    cout << "sending Adc read event for channel " << i << " value=" << val.analogValue << " raw: " << x << endl;
                    event->send(&val);
                    free(resp);
                }
            }
        }
    }
}

void Adc::activateChannel(int c) {
    active |= 1 << c;
}

void Adc::deactivateChannel(int c) {
    active &= 0xffff ^ (1 << c);
}

bool Adc::test() {
    mechio::common::error("WARNING: no known test for Adc\n");
    return true;
}

bool Adc::init() {
}

bool Adc::readInt(int& x) {                              // input is channel to read
    int cmd = 0x8c | (x<<4);                          // cmd reg: bit 7 = single-ended, 6-4 = channel, 3-2 = power mode
    char* resp = i2cRead(wr, cmd, 2);                 // read 2 bytes of data
    if (resp == NULL) {
        return false;
    }
    x = chars2int(resp);
    free(resp);
}

bool Adc::readXYZ(int& x, int& y, int& z) {
    mechio::common::error("ERROR: Adc::readXYZ not implemented\n");
}

int Adc::getAddr() {
    return wr >> 1;
}

string Adc::getName() {
    return "Adc";
}

void Led::poll() {
    if (config==NULL) return;
    Message msg; 
    if(fetchIfReady(config, msg)) {
        StampedVector3Record* cmd = unpackMessage<StampedVector3Record>(msg);
        r = cmd->vector.x;
        g = cmd->vector.y;
        b = cmd->vector.z;
        cout << "LedConfig rgb: " << r << ", " << g << ", " << b << endl;
        initRequest = true;
    }
}

void Led::update(double dt) {
    if(initRequest) {
        init();
        i2cWrite(wr, 4, 0x82, (char)b, (char)g, (char)r);
        i2cStop();
        initRequest = false;
    }
}

bool Led::test() {
    mechio::common::error("WARNING: no known test for Led\n");
    return true;
}

bool Led::init() {
    i2cSetDev(wr, 3);
    i2cWrite(wr, 3, 0x80, 1, 1);
    i2cStop();
    i2cWrite(wr, 2, 0x0c, 0xaa);
    i2cStop();
}

bool Led::readInt(int& x) {
    mechio::common::error("ERROR: Led::readInt not implemented\n");
}

bool Led::readXYZ(int& x, int& y, int& z) {
    mechio::common::error("ERROR: Led::readXYZ not implemented\n");
}

int Led::getAddr() {
    return wr >> 1;
}

string Led::getName() {
    return "Led";
}
