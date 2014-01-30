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

#define FAKE false

using namespace std;
using namespace qpid::messaging;
using namespace mechio;

const char* connectionStr = "localhost:5672";
const char* connectionOpt = "{username:admin,password:admin}";

extern boost::posix_time::ptime t1970;
extern double MsStartSession;      // make global for debugging
#define MsSince1970 ((boost::posix_time::microsec_clock().local_time()-t1970).total_milliseconds())

avro::ValidSchema loadSchema(const char* filename){
    ifstream in(filename);

    avro::ValidSchema theSchema;
    avro::compileJsonSchema(in, theSchema);

    return theSchema;
}

template<typename T> T* loadFromJsonFile(const char* filename, const char* schemaPath){
    avro::ValidSchema schema = loadSchema(schemaPath);
    DecoderPtr d = jsonDecoder(schema);
    auto_ptr<InputStream> is = avro::fileInputStream(filename);
    d->init(*is);
    T* t = new T();
    avro::decode(*d, *t);
    return t;
}

SensorBase* sensor[6];

//int bugg=0;
void pollingThread() {
    while(true) {
//        cout << "  TEST THREAD " << bugg++ << endl;
        for(int i=0; i<6; i++) {
            sensor[i]->poll();
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(20));
    }
}

int main(int argc, char **argv) {
    int sleepUsec = 10000;
    int i, x, y, z;
    int adcChanEnabled = 0; /// FIXME -- should be in Adc class?

    I2CQpidConfigRecord* config = loadFromJsonFile<I2CQpidConfigRecord>(argv[1], argv[2]);
    
    cout << "IP addr: " << config->brokerIPAddress << endl;
    
    MessagingProvider* myStream = new MessagingProvider(connectionStr, connectionOpt);
    myStream->connect();

    i2cInit();

    sensor[0] = new Gpio;
    sensor[1] = new Adc;
    sensor[2] = new Accelerometer;
    sensor[3] = new Compass;
    sensor[4] = new Gyro;
    sensor[5] = new Led;

    /*
    for (i=0; i<5; i++) {
        cout << sensor[i]->getName() << " test result: " << sensor[i]->test() << endl;
        sensor[i]->initMsg(myStream, (sensor[i]->getName()));
//        sensor[i]->init(0);
    }
     */
    if (FAKE) {
        cout << "***ATTENTION***\nfake mode, assuming sensors exist regardless\n";
    }
    
    if (FAKE || sensor[0]->test()) {
        sensor[0]->initMsg(myStream, config->gpioConfigDestination, config->gpioReadDestination, config->gpioEventDestination, config->gpioWriteDestination);
    }
    else cout << "FAIL init sensor 0 gpio\n";

    if (FAKE || sensor[1]->test()) {
        sensor[1]->initMsg(myStream, config->adcConfigDestination, config->adcReadDestination, config->adcEventDestination);
    }
    else cout << "FAIL init sensor 1 adc\n";

    if (FAKE || sensor[2]->test()) {
        sensor[2]->initMsg(myStream, config->accelerometerConfigDestination, config->accelerometerReadDestination, config->accelerometerEventDestination);
    }
    else cout << "FAIL init sensor 2 accel\n";

    if (FAKE || sensor[3]->test()) {
        sensor[3]->initMsg(myStream, config->compassConfigDestination, config->compassReadDestination, config->compassEventDestination);
    }
    else cout << "FAIL init sensor 3 compass\n";

    if (FAKE || sensor[4]->test()) {
        sensor[4]->initMsg(myStream, config->gyroConfigDestination, config->gyroReadDestination, config->gyroEventDestination);
    }
    else cout << "FAIL init sensor 4 gyro\n";

    if (FAKE || sensor[5]->test()) {
        sensor[5]->initMsg(myStream, config->ledConfigDestination);
    }
    else cout << "FAIL init sensor 5 led\n";

    boost::thread th(pollingThread);
    cout << "THREAD should run: " << endl;

    double t0 = MsSince1970;
    MsStartSession = t0;
    while(true)
    {
        double t1 = MsSince1970;
        for (i=0; i<6; i++) {
            sensor[i]->update(t1 - t0);
        }
        t0 = t1;
        usleep(sleepUsec);
        /* // test unstacking logic
        if (MsSince1970-MsStartSession > 15000 && MsSince1970-MsStartSession < 15010) {
            usleep(6000000);
        }
        */
    }
    return 0;
}
