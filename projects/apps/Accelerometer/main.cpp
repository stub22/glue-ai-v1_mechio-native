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
    MessageSender* sender = myStream->createSender("AccelerometerEvent; {create: always, node: {type: queue}}");
    HeaderRecord header;
    Vector3Record data;
    StampedVector3Record vec3;
    boost::posix_time::ptime t1970(boost::gregorian::date(1970,1,1));
    int sleepUsec = 200000;

    if (argc > 1) {
        sleepUsec = atoi(argv[1]);
    }

    i2cInit();

    /* device test */
    int wr = 0xa6;
    i2cSetDev(wr, 3);
    i2cStop();                                                       // not clear why necessary
    char* resp = i2cRead(wr, 0x00, 1);
    if (resp == NULL)
    {
        mechio::common::error("ERROR i2cRead\n");
        return 0;
    }
    if ((unsigned char) resp[0] == 0xe5)
    {
        cout << "Analog Devices accelerometer detected; running main loop." << endl;
    } else {
        mechio::common::error("This is not an Analog Devices accelerometer, quitting");
        return 0;
    }
    free(resp);

    i2cSetDev(wr, 3);
    i2cStop();                                                       // not clear why necessary
    if (i2cWrite(wr, 2, 0x2d, 0x08)) {                            // wake up & start producing data
        mechio::common::error("Failure to write control reg\n");
        return 0;
    }

    while(true)
    {
        i2cSetDev(wr, 3);
        i2cStop();
        resp = i2cRead(wr, 0x32, 6);                                // read 6 bytes for XYZ data
        data.x = chars2intLE(resp);
        data.y = chars2intLE(resp+2);
        data.z = chars2intLE(resp+4);
        header.sequenceId = wr >> 1;                            // chip ID -- use i2c addr for now
        header.frameId = msgcnt++;
        header.timestamp = (boost::posix_time::microsec_clock().local_time()-t1970).total_milliseconds();
        vec3.header = header;
        vec3.vector = data;
        //cout << "vec3: " << vec3.vector.x << ", " << vec3.vector.y << ", " << vec3.vector.z << endl;
        sender->send(&vec3);
        usleep(sleepUsec);
    }
    sender->close();
    return 0;
}
