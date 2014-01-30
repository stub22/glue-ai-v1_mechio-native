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
#include "mechio/vision/proc/facedetect/FaceDetectService.h"
#include "mechio/vision/proc/facedetect/protocol/FaceDetectConfig.h"

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
const char* schemaPath = "/usr/include/mechio/vision/proc/facedetect/protocol/FaceDetectConfig.json";

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

FaceDetectConfig* getDefaultFaceDetectConfig(string broker){
	FaceDetectConfig* conf = new FaceDetectConfig();
    conf->brokerAddress = broker;
    conf->connectionOptions = connectionOpt;
    conf->imageEventDestination = destVidQ;
    conf->procEventDestination = destFD;
    conf->commandDestination = destFDCmd;
    conf->errorDestination = destFDErr;
    conf->haarFileLocation = "haarcascade_frontalface_default.xml";
    conf->scaleFactor = 1.5;
    conf->neighbors = 3;
    conf->haarFlags = 0;
    conf->minFaceWidth = 40;
    conf->minFaceHeight = 40;
    return conf;
}

void serviceProvider(string broker, FaceDetectConfig* config){
    cout << "Creating service at addr: " << broker << "\n";
    
    FaceDetectService* service = new FaceDetectService();
    if(!service->initialize(config)){
        warn("Unable to initialize face detect service.");
        return;
    }
    
    MessagingProvider* myStream = new MessagingProvider(broker.c_str(), connectionOpt);
    if(!myStream->connect()){
        warn("Unable to connect to broker at " + broker);
        return;
    }
    MessageReceiver* cmdReceiver = myStream->createReceiver(destFDCmd);
    MessageSender* errorSender = myStream->createSender(destFDErr);
    RemoteServiceHost<FaceDetectConfig> s(service, cmdReceiver, errorSender);
    s.start();
    myStream->disconnect();
}

int main(int argc, char **argv) {
    string broker;
    FaceDetectConfig* config;
    if(argc <= 1){
        broker = connectionStr;
        cout << "No file specified, loading default config." << endl;
        config = getDefaultFaceDetectConfig(broker);
    }else if(argc == 2){
        config = loadFromJsonFile<FaceDetectConfig>(argv[1], schemaPath);
        broker = config->brokerAddress;
    }else{
        config = loadFromJsonFile<FaceDetectConfig>(argv[1], argv[2]);
        broker = config->brokerAddress;
    }
    serviceProvider(broker, config);
    return 0;
}


