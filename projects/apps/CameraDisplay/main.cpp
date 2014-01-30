/* 
 * File:   main.cpp
 * Author: matt
 *
 * Created on November 12, 2011, 4:29 PM
 */

#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include "mechio/messaging/MessagingProvider.h"
#include "mechio/messaging/MessageSender.h"
#include "mechio/messaging/RemoteService.h"
#include "mechio/messaging/RemoteServiceHost.h"
#include "mechio/vision/display/ImageDisplayService.h"
#include "mechio/vision/display/protocol/ImageDisplayConfig.h"
#include "mechio/vision/camera/protocol/CameraConfig.h"
#include "mechio/vision/proc/facedetect/protocol/FaceDetectConfig.h"

#include <avro/Compiler.hh>
#include <avro/Encoder.hh>
#include <avro/Decoder.hh>
#include <avro/Specific.hh>

using namespace std;
using namespace qpid::messaging;
using namespace mechio;

const char* connectionStr = "localhost:5672";
const char* connectionOpt = "{username:admin,password:admin}";
const char* destVidQ = "camera0Event; {create: always, node: {type: topic}}";
const char* destFD = "visionproc0Event; {create: always, node: {type: topic}}";
const char* destDspCmd = "display0Command; {create: always, node: {type: queue}}";
const char* destDspErr = "display0Error; {create: always, node: {type: topic}}";
const char* destCamCmd = "camera0Command; {create: always, node: {type: queue}}";
const char* destFDCmd = "visionproc0Command; {create: always, node: {type: queue}}";
const char* schemaPath = "/usr/include/mechio/vision/display/protocol/ImageDisplayConfig.json";

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

ImageDisplayConfig* getDisplayConfig(string broker){
    ImageDisplayConfig* conf = new ImageDisplayConfig();
    conf->brokerAddress = broker;
    conf->connectionOptions = connectionOpt;
    conf->imageEventDestination = destVidQ;
    conf->procEventDestination = destFD;
    conf->commandDestination = destDspCmd;
    conf->errorDestination = destDspErr;
    conf->cameraCommandDestination = destCamCmd;
    conf->procCommandDestination = destFDCmd;
    conf->drawRate = 10;
    conf->windowName = "Display";
    return conf;
}

void sendCmd(MessageSender* sender, string command){
    ServiceCommandRecord cmd;
    cmd.command = command;
    cmd.sourceId = "source";
    cmd.destinationId = "destination";
    cmd.timestampMillisecUTC = 1;
    sender->send<ServiceCommandRecord>(&cmd, "application/service-command", true);
}

void startProcs(const char* camDest, const char* fdDest, string broker){
    MessagingProvider* myStream;
	myStream = new MessagingProvider(broker.c_str(), connectionOpt);
    if(!myStream->connect())
        return;
    MessageSender* fdCmdSnd = myStream->createSender(fdDest);
    sendCmd(fdCmdSnd, "start");
    MessageSender* camCmdSnd = myStream->createSender(camDest);
    sendCmd(camCmdSnd, "start");
}

void wait(){
    bool stop = false;
    char k;
    while(!stop){
        k = getchar();
        if(k == 'q'){
            stop = true;
        }
    }
}

int main(int argc, char **argv) {
    string broker;
    ImageDisplayConfig* config;
    if(argc <= 1){
        broker = connectionStr;
        config = getDisplayConfig(broker);
    }else if(argc == 2){
        config = loadFromJsonFile<ImageDisplayConfig>(argv[1], schemaPath);
        broker = config->brokerAddress;
    }else{
        config = loadFromJsonFile<ImageDisplayConfig>(argv[1], argv[2]);
        broker = config->brokerAddress;
    }
    RemoteService<ImageDisplayConfig>* ids = new ImageDisplayService();
    ids->initialize(config);
    ids->start();
    startProcs(config->cameraCommandDestination.c_str(), config->procCommandDestination.c_str(), broker);
    wait();
    return 0;
}
