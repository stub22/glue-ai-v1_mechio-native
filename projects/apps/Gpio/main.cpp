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

void setIO(int wr, int io) {
    i2cSetDev(wr, 3);
    if (i2cWrite(wr, 2, 0x03, io & 0xff)) {                            // set I/O direction bits 0-7
        printf("Failure to write control reg\n");
    }
    i2cSetDev(wr, 3);
    if (i2cWrite(wr, 2, 0x02, io >> 8)) {                            // set I/O direction bits 8-15
        printf("Failure to write control reg\n");
    }
}

void setOut(int wr, int out) {
    i2cSetDev(wr, 3);
    if (i2cWrite(wr, 2, 0x01, out & 0xff)) {
        printf("Failure to write control reg\n");
    }
    i2cSetDev(wr, 3);
    if (i2cWrite(wr, 2, 0x00, out >> 8)) {
        printf("Failure to write control reg\n");
    }
}

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
    MessageSender* sender = myStream->createSender("GpioEvent; {create: always, node: {type: queue}}");
    MessageReceiver* inRecv = myStream->createReceiver("GpioCommand; {create: always, node: {type: queue}}");
    MessageReceiver* outRecv = myStream->createReceiver("GpioWriteCommand; {create: always, node: {type: queue}}");
    HeaderRecord header;
    DeviceBoolRecord boo;
    boost::posix_time::ptime t1970(boost::gregorian::date(1970,1,1));
    int io = 0xffff;                                                // 1 means in, 0 out
    int watch = 0;
    int out = 0x0000;
    int sleepUsec = 200000;

    if (argc > 1) {
        sleepUsec = atoi(argv[1]);
    }
    
    i2cInit();

    /* no explicit device test for GPIO */
    int wr = 0x40;
    setIO(wr, io);
    setOut(wr, out);
    
    cout << "GPIO test server" << endl;

    bool channelVals[16][2];
    for(int i=0; i<16; i++){
        channelVals[i][0] = false;
    }
    while (true)   
    {
        Message msg; 
        if (fetchIfReady(inRecv, msg)) {
            DeviceBoolRecord* boo = unpackMessage<DeviceBoolRecord>(msg);
            if (boo->boolValue) {
                io |= 1 << boo->channelId;                
                watch |= 1 << boo->channelId;                
            }
            else {
                io &= 0xffff ^ (1 << boo->channelId);
                watch &= 0xffff ^ (1 << boo->channelId);
            }
            setIO(wr, io);
            //cout << "GpioCommand: " << boo->header.frameId << boo->boolValue << " chan: " 
            //        << boo->channelId << " watch: " << hex << watch << endl;
        }

        if (fetchIfReady(outRecv, msg)) {
            DeviceBoolRecord* boo = unpackMessage<DeviceBoolRecord>(msg);
            if (boo->boolValue) {
                out |= 1 << boo->channelId;
            }else{
                out &= 0xffff ^ (1 << boo->channelId);
            }
            setOut(wr, out);
            //cout << "GpioWriteCommand: " << boo->header.frameId << boo->boolValue << " chan: " 
            //        << boo->channelId << " data reg: " << hex << out << endl;
        }

        i2cSetDev(wr, 3);
        if (watch != 0) {
            char* resp = i2cRead(wr, 0x00, 2);                                // read 2 bytes
            int x = chars2int(resp);
            //printf("GPIO input reading: %x\n", x);
            for (int chan=0; chan<16; chan++) {
                if( ((1 << chan) & watch) != 0) {
                    bool chanVal = (x & (1 << chan)) != 0;
                    bool send = false;
                    if(!channelVals[chan][0]){
                        channelVals[chan][0] = true;
                        channelVals[chan][1] = chanVal;
                        send = true;
                    }else{
                        send = channelVals[chan][1] != chanVal;
                        channelVals[chan][1] = chanVal;
                    }
                    if(send){
                        boo.boolValue = chanVal;
                        boo.channelId = chan;
                        header.sequenceId = wr >> 1;                            // chip ID -- use i2c addr for now
                        header.frameId = msgcnt++;
                        header.timestamp = (boost::posix_time::microsec_clock().local_time()-t1970).total_milliseconds();
                        boo.header = header;
                        sender->send(&boo);
                    }
                }
            }
            free(resp);
        }
        usleep(sleepUsec);
    }
    return 0;
}
