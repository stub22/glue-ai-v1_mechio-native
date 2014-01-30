#include <iostream>
#include "mechio/messaging/MessagingProvider.h"
#include "mechio/messaging/MessageSender.h"
#include "mechio/messaging/RemoteService.h"
#include "mechio/messaging/RemoteServiceHost.h"
#include "mechio/speech/SpeechEngine.h"
#include "mechio/speech/SpeechService.h"
#include "mechio/speech/mssapi/SAPIEngine.h"
#include "mechio/speech/protocol/SpeechConfigRecord.h"
#include "mechio/speech/protocol/SpeechEventListRecord.h"

using namespace std;
using namespace qpid::messaging;
using namespace mechio;
using namespace mechio::speech;
const char* connectionStr = "192.168.0.101:5672";
const char* connectionOpt = "{username:admin,password:admin}";
const char* requestQueue = "speechRequest; {create: always, node: {type: queue}}";
const char* eventQueue = "speechEvent; {create: always, node: {type: topic}}";
const char* cmdQueue = "speechCommand; {create: always, node: {type: queue}}";
const char* errQueue = "speechError; {create: always, node: {type: topic}}";

SpeechConfigRecord* getDefaultSpeechConfig(){
	SpeechConfigRecord* conf = new SpeechConfigRecord();
    conf->voiceName = "nelly";
    conf->sampleRate = 160000.0;
    return conf;
}

template<typename T> void provider(RemoteService<T>* service, const char* destCmd, const char* destErr){
    MessagingProvider* myStream;
    myStream = new MessagingProvider(connectionStr);
    if(!myStream->connect())
        return;
    MessageReceiver* cmdRec = myStream->createReceiver(destCmd);
    MessageSender* errSnd = myStream->createSender(destErr);
    RemoteServiceHost<T> s(service, cmdRec, errSnd);
    s.start();
}

int main2(int argc, char **argv) {
    cout << "Starting Text-to-Speech Service." << endl;
	RemoteService<SpeechConfigRecord>* service = new SpeechService((SpeechEngine*)SAPIEngine::instance());
	SpeechConfigRecord* config;
	if(argc <= 1){
		cout << "No file specified, loading default config." << endl;
		config = getDefaultSpeechConfig();
	}else{
		config = loadFromFile<SpeechConfigRecord>(argv[1]);
	}
	if(config != NULL){
		service->initialize(config);
	}
	provider<SpeechConfigRecord>(service, cmdQueue, errQueue);
	return 0;
}

