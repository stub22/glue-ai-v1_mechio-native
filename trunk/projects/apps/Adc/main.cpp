/* 
 * File:   main.cpp
 * Author: matt, dbm
 *
 * Created on November 12, 2011, 3:59 PM
 */

#include <cstdlib>
#include <stdio.h>
#include "mechio/messaging/MessagingProvider.h"
#include "mechio/messaging/MessageSender.h"
#include "mechio/messaging/RemoteService.h"
#include "mechio/messaging/RemoteServiceHost.h"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"
#include "mechio/i2c/protocol/SensorProtocol.h"
#include "mechio/i2c/i2c.h"

using namespace std;
using namespace qpid::messaging;
using namespace mechio;

const char* connectionStr = "localhost:5672";
const char* connectionOpt = "{username:admin,password:admin}";
int msgcnt=0;

bool fetchIfReady(MessageReceiver* recv, Message& msg) {
    try {
        msg = recv->fetchRaw(Duration(0));
        recv->acknowledge();
        return true;
    } catch (const NoMessageAvailable &ex) {
        return false;
    }
}

int main(int argc, char **argv) {
    MessagingProvider* myStream = new MessagingProvider(connectionStr, connectionOpt);
    myStream->connect();
    MessageSender* sender = myStream->createSender("AdcEvent; {create: always, node: {type: queue}}");
    MessageReceiver* inRecv = myStream->createReceiver("AdcCommand; {create: always, node: {type: queue}}");
    HeaderRecord header;
    DeviceBoolRecord boo;
    DeviceAnalogRecord val;
    boost::posix_time::ptime t1970(boost::gregorian::date(1970,1,1));
    int watch = 0;
    int sleepUsec = 200000;

    if (argc > 1) {
        sleepUsec = atoi(argv[1]);
    }
    i2cInit();

    cout << "ADC test server" << endl;

    /* no explicit device test for ADC */
    int wr = 0x90;
    i2cSetDev(wr, 3);
    while (true)   
    {
        Message msg; 
        if(fetchIfReady(inRecv, msg)) {
            DeviceBoolRecord* boo = unpackMessage<DeviceBoolRecord>(msg);
            if (boo->boolValue) {
                watch |= 1 << boo->channelId;                
            }
            else {
                watch &= 0xff ^ (1 << boo->channelId);
            }
            //cout << "AdcCommand: " << boo->header.frameId << boo->boolValue << " chan: " 
            //        << boo->channelId << " watch: " << hex << watch << endl;
        }

        if (watch != 0) {
            int cmd;
            for (int chan=0; chan<8; chan++) {
                if ( (watch & (1 << chan)) != 0) {                  // if we care
                    cmd = 0x8c | (chan<<4);                        // cmd reg: bit 7 = single-ended, 6-4 = channel, 3-2 = power mode
                    char* resp = i2cRead(wr, cmd, 2);             // read 2 bytes of data
                    int x = chars2int(resp);
                    //printf("ADC reading: %d\n", x);
                    val.analogValue = (double) x;
                    val.channelId = chan;
                    header.sequenceId = wr >> 1;             // chip ID -- use i2c addr for now
                    header.frameId = msgcnt++;
                    header.timestamp = (boost::posix_time::microsec_clock().local_time()-t1970).total_milliseconds();
                    val.header = header;
                    sender->send(&val);
                    free(resp);
                }
            }
        }
        usleep(sleepUsec);
    }
    return 0;
}
