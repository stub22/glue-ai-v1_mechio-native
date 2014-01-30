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

int main(int argc, char **argv) {
    MessagingProvider* myStream = new MessagingProvider(connectionStr, connectionOpt);
    myStream->connect();
    MessageSender* sender = myStream->createSender("CompassEvent; {create: always, node: {type: queue}}");
    HeaderRecord header;
    Vector3Record data;
    StampedVector3Record vec3;
    boost::posix_time::ptime t1970(boost::gregorian::date(1970,1,1));
    stringstream err;
    int sleepUsec = 200000;

    if (argc > 1) {
        sleepUsec = atoi(argv[1]);
    }
    i2cInit();
    
    /* compass test */
    int wr = 0x3c;
    i2cSetDev(wr, 3);
    char* resp = i2cRead(wr, 10, 3);
    if (resp == NULL)
    {
        mechio::common::error("ERROR i2cRead\n");
        return 0;
    }
    if (resp[0] == 'H' && resp[1] == '4' && resp[2] == '3')
    {
        cout << "Honeywell Compass detected; initiating data feed" << endl;
    } else {
        mechio::common::error("This is not a Honeywell compass\n");
        return 0;
    }
    free(resp);

    int success=0;
    for(int i=0; i<3; i++) {                                         // for some reason the chip fails first time trying to access these registers
        i2cSetDev(wr, 3);
        if (!i2cWrite(wr, 2, 0, 0x28)) {                              // avg 2 samples @ 3hz
            success |= 1;
        }

        i2cSetDev(wr, 3);
        if (!i2cWrite(wr, 2, 1, 0x80)) {                              // gain = 440 (lower than default which outranges)
            success |= 2;
        }
    }
    if(success != 3) {
        mechio::common::error("ERROR: multiple ack failures setting compass registers");
        return 0;
    }

    while (true)
    {
        i2cSetDev(wr, 3);
        if (i2cWrite(wr, 2, 2, 0x01)) {                            // start continuous data
            mechio::common::error("ERROR compass read data ack failed (nonfatal)\n");
            continue;                                                   // this seems to happen occasionally
        }
        resp = i2cRead(wr, 9, 1);                                // reg 9 bit 0 == data ready; bit 1 == lock
        usleep(sleepUsec);
        if((resp[0] & 0x03) == 1)
        {
            i2cSetDev(wr, 3);
            char* resp2 = i2cRead(wr, 3, 6);                            // read 6 bytes for XYZ data
            data.x = chars2int(resp2);
            data.z = chars2int(resp2+2);
            data.y = chars2int(resp2+4);
            header.sequenceId = wr >> 1;                    // chip ID -- use i2c addr for now
            header.frameId = msgcnt++;
            header.timestamp = (boost::posix_time::microsec_clock().local_time()-t1970).total_milliseconds();
            vec3.header = header;
            vec3.vector = data;
            //cout << "vec3: " << vec3.vector.x << ", " << vec3.vector.y << ", " << vec3.vector.z << endl;
            sender->send(&vec3);
            free(resp2);
        }
        free(resp);
    }
    sender->close();
    return 0;
}
