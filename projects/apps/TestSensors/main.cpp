/* 
 * File:   main.cpp
 * Author: matt
 *
 * Created on November 12, 2011, 3:59 PM
 */

#include <cstdlib>
#include <termios.h>
#include "mechio/messaging/MessagingProvider.h"
#include "mechio/messaging/MessageSender.h"
#include "mechio/messaging/RemoteService.h"
#include "mechio/messaging/RemoteServiceHost.h"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"
#include "mechio/i2c/protocol/SensorProtocol.h"

using namespace std;
using namespace qpid::messaging;
using namespace mechio;

const char* connectionStr = "localhost:5672";
const char* connectionOpt = "{username:admin,password:admin}";

bool fetchIfReady(MessageReceiver* recv, Message& msg) {
    try {
        msg = recv->fetchRaw(Duration(0));
        recv->acknowledge();
        return true;
    } catch (const NoMessageAvailable &ex) {
        return false;
    }
}

double xy2ang(double x, double y) {
    if (y == 0.0) y = 0.0001; // relax, it's just test code
    double tan = x / y;
    double deg = atan(tan) * 57.2957795;
    if (y < 0) deg += 180;
    if (deg > 180) deg -= 360;
    return deg;
}

int main(int argc, char **argv) {
    MessagingProvider* myStream = new MessagingProvider(connectionStr, connectionOpt);
    myStream->connect();
    MessageSender* sender = NULL;
    cout << "myStream: " << myStream << endl;
    HeaderRecord header;
    header.frameId = 333;
    header.timestamp = 4444.4444;
    header.sequenceId = 98;
    if (argc == 3) {
        string cmd = argv[1];
        cmd.append("Read; {create:always, node:{type:topic}}");
        DeviceReadPeriodRecord msg;
        msg.header = header;
        msg.period = atof(argv[2]);
        cout << "sending msg: " << cmd << endl;
        sender = myStream->createSender(cmd.c_str());
        sender->send(&msg);
    }
    if (argc != 3) {
        string cmd = argv[1];
        if (cmd == "adc") {
            AdcConfigRecord boo;
            boo.header = header;
            boo.channelMask = 4;
            cmd = "adcConfig; {create: always, node: {type: topic}}";
            cout << "sending msg: " << cmd << endl;
            sender = myStream->createSender(cmd.c_str());
            sender->send(&boo);
        }
        if (cmd == "gpio") {
            GpioConfigRecord boo;
            boo.header = header;
            if (argc > 3) {
                boo.inputMask = atoi(argv[3]);
            }
            else {
                 boo.inputMask = 0x20;
            }
            cmd = "gpioConfig; {create: always, node: {type: topic}}";
            cout << "sending msg: " << cmd << endl;
            sender = myStream->createSender(cmd.c_str());
            sender->send(&boo);
        }
        if (cmd == "gpio_w") {
            DeviceBoolRecord boo;
            boo.header = header;
            boo.channelId = atoi(argv[2]);
            boo.boolValue = atoi(argv[3]);
            cmd = "gpioWrite; {create: always, node: {type: topic}}";
            cout << "sending msg: " << cmd << endl;
            sender = myStream->createSender(cmd.c_str());
            sender->send(&boo);
        }
        if (cmd == "compass") {
            CompassConfigRecord comp;
            comp.header = header;
            comp.average = 3;
            comp.rate = 2;
            comp.bias = 0;
            comp.gain = 7;
            cmd = "compassConfig; {create: always, node: {type: topic}}";
            cout << "sending msg: " << cmd << endl;
            sender = myStream->createSender(cmd.c_str());
            sender->send(&comp);
        }
        if (cmd == "gyro") {
            GyroConfigRecord gyro;
            gyro.header = header;
            gyro.ctl1 = 0x0f;
            cmd = "gyroConfig; {create: always, node: {type: topic}}";
            cout << "sending msg: " << cmd << endl;
            sender = myStream->createSender(cmd.c_str());
            sender->send(&gyro);
        }
        if (cmd == "accelerometer") {
            AccelerometerConfigRecord acc;
            acc.header = header;
            acc.reg = 0x2d;
            acc.value = 8;
            cmd = "accelerometerConfig; {create: always, node: {type: topic}}";
            cout << "sending msg: " << cmd << endl;
            sender = myStream->createSender(cmd.c_str());
            sender->send(&acc);
        }
        if (cmd == "led") {
            Vector3Record vec;
            vec.x = atoi(argv[2]);
            vec.y = atoi(argv[3]);
            vec.z = atoi(argv[4]);
            StampedVector3Record sv;
            sv.header = header;
            sv.vector = vec;
            cmd = "ledConfig; {create: always, node: {type: topic}}";
            cout << "sending msg: " << cmd << endl;
            sender = myStream->createSender(cmd.c_str());
            sender->send(&sv);
        }
    }
    usleep(200000);
    cout << "closing send queue" << endl;
    if (sender != NULL) sender->close();
    cout << "quitting" << endl;
}
