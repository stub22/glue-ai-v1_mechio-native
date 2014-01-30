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

using namespace std;
using namespace qpid::messaging;
using namespace mechio;

class SensorBase {
    double time;
    double timer;
    double period;
public:
    SensorBase();
    MessageReceiver* config;
    MessageReceiver* read;
    MessageReceiver* write;
    MessageSender* event;
    bool initRequest;
    void initMsg(MessagingProvider *strm, string config, string read, string event, string write);
    void initMsg(MessagingProvider *strm, string config, string read, string event);
    void initMsg(MessagingProvider *strm, string config);
    bool updateTimer(double dt);
    void setPeriod(double p);
    double getPeriod();
    virtual bool test() = 0;
    virtual bool init() = 0;
    virtual bool readInt(int& x) = 0;
    virtual bool readXYZ(int& x, int& y, int& z) = 0;
    virtual int getAddr() = 0;
    virtual string getName() = 0;
    virtual void poll() = 0;
    virtual void update(double dt) = 0;
};

class Gyro : public SensorBase {
    static const int wr = 0xd2;
    int ctl1, ctl2, ctl3, ctl4, ctl5;
public:
    bool test();
    bool init();
    bool readInt(int& x);
    bool readXYZ(int& x, int& y, int& z);
    int getAddr();
    string getName();
    void poll();
    void update(double dt);
};

class Accelerometer : public SensorBase {
    static const int wr = 0xa6;
    int reg;
    int value;
    bool initialized;
public:
    Accelerometer();
    bool test();
    bool init();
    bool readInt(int& x);
    bool readXYZ(int& x, int& y, int& z);
    int getAddr();
    string getName();
    void poll();
    void update(double dt);
};

class Compass : public SensorBase {
    static const int wr = 0x3c;
	unsigned int average;		//# of samples to average together: 0=1, 1=2, 2=4, 3=8
	unsigned int rate;			//sample rate: 0=.75, 1=1.5, 2=3, 3=7.5, 4=15, 5=30, 6=75
	unsigned int bias;			// 0=normal; 1=positive; 2=negative
	unsigned int gain;			//(LSB/Gauss): 0=1370 to 7=230
public:
    bool test();
    bool init();
    bool readInt(int& x);
    bool readXYZ(int& x, int& y, int& z);
    int getAddr();
    string getName();
    void poll();
    void update(double dt);
};

class Gpio : public SensorBase {
    static const int wr = 0x40;
    unsigned int active, output;
    bool channelVals[16][2];
public:
    Gpio();
    void setIO(int wr, int io);
    void setOut(int wr, int out);
    bool test();
    bool init();
    bool readInt(int& x);
    bool readXYZ(int& x, int& y, int& z);
    int getAddr();
    string getName();
    void poll();
    void update(double dt);
    bool outRequest;
};

class Adc : public SensorBase {
    static const int wr = 0x90;
    unsigned int active;
public:
    Adc();
    bool test();
    bool init();
    bool readInt(int& x);
    bool readXYZ(int& x, int& y, int& z);
    int getAddr();
    string getName();
    void poll();
    void update(double dt);
    void activateChannel(int);
    void deactivateChannel(int);
};

class Led : public SensorBase {
    static const int wr = 0xe0;
    int r;
    int g;
    int b;
public:
    bool test();
    bool init();
    bool readInt(int& x);
    bool readXYZ(int& x, int& y, int& z);
    int getAddr();
    string getName();
    void poll();
    void update(double dt);
};
