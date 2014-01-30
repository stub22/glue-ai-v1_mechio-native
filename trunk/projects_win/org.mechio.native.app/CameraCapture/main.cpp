/* 
 * File:   main.cpp
 * Author: matt
 *
 * Created on November 12, 2011, 4:06 PM
 * win
 */

#include <fstream>

#include "mechio/messaging/MessagingProvider.h"
#include "mechio/messaging/MessageSender.h"
#include "mechio/messaging/RemoteService.h"
#include "mechio/messaging/RemoteServiceHost.h"
#include "mechio/vision/camera/WinCameraService.h"
#include "mechio/vision/camera/protocol/CameraConfig.h"

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
const char* destCamCmd = "camera0Command; {create: always, node: {type: queue}}";
const char* destCamErr = "camera0Error; {create: always, node: {type: topic}}";
const char* schemaPath = "/usr/include/mechio/vision/camera/protocol/CameraConfig.json";

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

CameraConfig* getDefaultCameraConfig(string broker){
	CameraConfig* conf = new CameraConfig();
    conf->brokerAddress = broker;
    conf->connectionOptions = connectionOpt;
    conf->eventDestination = destVidQ;
    conf->commandDestination = destCamCmd;
    conf->errorDestination = destCamErr;
    conf->cameraNumber = 0;
    conf->frameLength = 100;
    conf->imageChannels = 1;
    conf->imageWidth = 320;
    conf->imageHeight = 240;
    conf->cameraWidth = 320;
    conf->cameraHeight = 240;
    conf->cameraChannels = 3;
    conf->flipImage = false;
    conf->flipImageHorizontal = false;
    return conf;
}

void serviceProvider(string broker, CameraConfig* config){
    cout << "Creating service at addr: " << broker << "\n";
    
    WinCameraService* service = new WinCameraService();
    if(!service->initialize(config)){
        warn("Unable to initialize speech service.");
        return;
    }
    
    MessagingProvider* myStream = new MessagingProvider(broker.c_str(), config->connectionOptions.c_str());
    if(!myStream->connect()){
        warn("Unable to connect to broker at " + broker);
        return;
    }
    MessageReceiver* cmdReceiver = myStream->createReceiver(destCamCmd);
    MessageSender* errorSender = myStream->createSender(destCamErr);
    RemoteServiceHost<CameraConfig> s(service, cmdReceiver, errorSender);
    s.start();
    myStream->disconnect();
}

int main(int argc, char **argv) {
    string broker;
    CameraConfig* config;
    if(argc == 2){
        config = loadFromJsonFile<CameraConfig>(argv[1], schemaPath);
        broker = config->brokerAddress;
    }else if(argc > 2){
        config = loadFromJsonFile<CameraConfig>(argv[1], argv[2]);
        broker = config->brokerAddress;
    }else{
            cout << "No file specified, loading default config." << endl;
            broker = connectionStr;
            config = getDefaultCameraConfig(broker);
    }
    serviceProvider(broker, config);
    return 0;
}

