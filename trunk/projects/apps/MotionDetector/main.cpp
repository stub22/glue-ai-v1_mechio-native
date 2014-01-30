/* 
 * File:   main.cpp
 * Author: matt
 *
 * Created on November 12, 2011, 3:59 PM
 */

#include <cstdlib>
#include <fstream>
#include "mechio/messaging/MessagingProvider.h"
#include "mechio/messaging/MessageSender.h"
#include "mechio/messaging/RemoteService.h"
#include "mechio/messaging/RemoteServiceHost.h"
#include "mechio/vision/proc/motiondetect/MotionDetectService.h"
#include "mechio/vision/proc/motiondetect/protocol/MotionDetectConfig.h"

#include <avro/Compiler.hh>
#include <avro/Encoder.hh>
#include <avro/Decoder.hh>
#include <avro/Specific.hh>

using namespace std;
using namespace qpid::messaging;
using namespace mechio;
using namespace mechio::common;

const char* connectionStr = "localhost:5672";
const char* connectionOpt = "{username:admin,password:admin}";
const char* destVidQ = "camera0Event; {create: always, node: {type: topic}}";
const char* destFD = "visionproc0Event; {create: always, node: {type: topic}}";
const char* destFDCmd = "visionproc0Command; {create: always, node: {type: queue}}";
const char* destFDErr = "visionproc0Error; {create: always, node: {type: topic}}";
const char* imageProcId = "motiondetect0";
const char* imageSourceId = "camera0";
const char* schemaPath = "/usr/include/mechio/vision/proc/motiondetect/protocol/MotionDetectConfig.json";

avro::ValidSchema loadSchema(const char* filename){
    ifstream in(filename);

    avro::ValidSchema theSchema;
    avro::compileJsonSchema(in, theSchema);

    return theSchema;
}

template<typename T> T* loadFromJsonFile(const char* filename, const char* schemaname){
    avro::ValidSchema schema = loadSchema(schemaname);
    DecoderPtr d = jsonDecoder(schema);
    auto_ptr<InputStream> is = avro::fileInputStream(filename);
    d->init(*is);

    T* t = new T();
    avro::decode(*d, *t);
    return t;
}

MotionDetectConfig* getDefaultMotionDetectConfig(string broker){
	MotionDetectConfig* conf = new MotionDetectConfig();
    conf->brokerAddress = broker;
    conf->connectionOptions = connectionOpt;
    conf->imageEventDestination = destVidQ;
    conf->procEventDestination = destFD;
    conf->commandDestination = destFDCmd;
    conf->errorDestination = destFDErr;
    conf->imageProcessorId.assign(imageProcId);
    conf->imageSourceId.assign(imageSourceId);
    conf->imageWidth = 320;
    conf->imageHeight = 240;
    return conf;
}

void serviceProvider(string broker, MotionDetectConfig* config){
    cout << "Creating service at addr: " << broker << "\n";
    
    MotionDetectService* service = new MotionDetectService();
    if(!service->initialize(config)){
        warn("Unable to initialize motion detect service.");
        return;
    }
    
    MessagingProvider* myStream = new MessagingProvider(broker.c_str(), connectionOpt);
    if(!myStream->connect()){
        warn("Unable to connect to broker at " + broker);
        return;
    }
    MessageReceiver* cmdReceiver = myStream->createReceiver(destFDCmd);
    MessageSender* errorSender = myStream->createSender(destFDErr);
    RemoteServiceHost<MotionDetectConfig> s(service, cmdReceiver, errorSender);
    s.start();
    myStream->disconnect();
}

int main(int argc, char **argv) {
    string broker;
    MotionDetectConfig* config;
    if(argc <= 1){
        broker = connectionStr;
        cout << "No file specified, loading default config." << endl;
        config = getDefaultMotionDetectConfig(broker);
    }else if(argc == 2){
        config = loadFromJsonFile<MotionDetectConfig>(argv[1], schemaPath);
        broker = config->brokerAddress;
    }else{
        config = loadFromJsonFile<MotionDetectConfig>(argv[1], argv[2]);
        broker = config->brokerAddress;
    }
    serviceProvider(broker, config);
    return 0;
}


