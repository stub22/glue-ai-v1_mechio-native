#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>

#include "mechio/common/logger.h"
#include "mechio/messaging/MessagingProvider.h"
#include "mechio/messaging/MessageSender.h"
#include "mechio/messaging/RemoteServiceHost.h"
#include "mechio/speech/mssapi/SAPIEngine.h"
#include "mechio/speech/protocol/SpeechConfigRecord.h"
#include "mechio/speech/SpeechService.h"
#include <boost/thread.hpp>

using namespace std;
using namespace mechio;
using namespace mechio::common;
using namespace mechio::speech;

SpeechConfigRecord* speechConfig(double sampleRate){
    SpeechConfigRecord* conf = new SpeechConfigRecord();
    conf->voiceName = "Nelly";
    conf->sampleRate = sampleRate;
    conf->speechServiceId = "acapelaSpeechService";
    conf->configSourceId = "speechClient";
    return conf;
}

MessageSender* sender(MessagingProvider *provider, string prefix, string queue){
    string dest(prefix.c_str());
    dest.append(queue);
	dest.append("; {assert: always, node: {type: topic}}");
    MessageSender* sender = provider->createSender(dest.c_str());
    return sender;
}

MessageReceiver* receiver(
                MessagingProvider *provider, string prefix, string queue){
    string dest(prefix.c_str());
    dest.append(queue);
    dest.append("; {assert: always, node: {type: queue}}");
    MessageReceiver* receiver = provider->createReceiver(dest.c_str());
    return receiver;
}

void serviceProvider(string broker, string prefix, string configFile){
    cout << "Creating service at addr: " << broker << 
            ", queue prefix: " << prefix << 
            //", config file: " << configFile << 
			"\n";
	MessagingProvider* aqs1 = new MessagingProvider(broker.c_str());
	while(!aqs1->connect()){
		cout << "Unable to connect to broker at addr: " << broker << 
			"\nattempting again in 5 seconds.\n";
		boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
	}
	MessagingProvider* aqs2 = new MessagingProvider(broker.c_str());
	aqs2->connect();
	MessagingProvider* aqs3 = new MessagingProvider(broker.c_str());
	aqs3->connect();
	
	MessageReceiver* requestReceiver = NULL;
	MessageSender* eventSender = NULL;
	MessageReceiver* commandReceiver = NULL;
	MessageSender* errorSender = NULL;
	while(requestReceiver == NULL || commandReceiver == NULL || eventSender == NULL || errorSender == NULL){
		try {
			
			if(requestReceiver == NULL){
				requestReceiver = receiver(aqs1, prefix, "Request");
			}
			if(commandReceiver == NULL){
				commandReceiver = receiver(aqs3, prefix, "Command");
			}
			if(eventSender == NULL){
				eventSender = sender(aqs2, prefix, "Event");
			}
			if(errorSender == NULL){
				errorSender = sender(aqs3, prefix, "Error");
			}
		} catch(const exception& error) {
			cout << "Unable to connect to topics.\nattempting again in 5 seconds.\n";
			boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
		} catch(...){
			cout << "Unable to connect to topics.\nattempting again in 5 seconds.\n";
			boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
		}
	}
	RemoteService<SpeechConfigRecord>* service = 
			(RemoteService<SpeechConfigRecord>*)
					new SpeechService((SpeechEngine*)SAPIEngine::instance());
	SpeechConfigRecord* conf = speechConfig(16000);
			//loadFromJsonFile<SpeechConfigRecord>((char*)configFile.c_str());
	if(!service->initialize(conf)){
		warn("Unable to initialize speech service.");
		return;
	}
	((SpeechService*)service)->setEventSender(eventSender);
	((SpeechService*)service)->setRequestReceiver(requestReceiver);
	service->start();
    
	RemoteServiceHost<SpeechConfigRecord> provider(
			service, commandReceiver, errorSender);
	provider.start();
	aqs1->disconnect();
	aqs2->disconnect();
	aqs3->disconnect();
		
}

int main(int argc, char **argv) {
    string broker;
    string prefix;
    string configFile = "/usr/etc/acapela/speechConfig.json";
    if(argc <= 1){
        broker = "127.0.0.1:5672";
        prefix = "speech";
    }else if(argc == 2){
        broker = argv[1];
        broker.append(":5672");
        prefix = "speech";
    }else if(argc == 3){
        broker = argv[1];
        broker.append(":5672");
        prefix = argv[2];
    }else if(argc == 4){
        broker = argv[1];
        broker.append(":5672");
        prefix = argv[2];
        configFile = argv[3];
    }
    serviceProvider(broker, prefix, configFile);
    return 0;
}
