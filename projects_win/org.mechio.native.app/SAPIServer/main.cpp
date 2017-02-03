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

MessageSender* connectSender(MessagingProvider *provider, string prefix, string destination){
	MessageSender* msgSender = NULL;
	try {
		msgSender = sender(provider, prefix, destination);
	} catch(const exception& error) {
		cout << "Unable to connect sender: " << prefix << destination <<".\nattempting again in 5 seconds.\n";
		msgSender = NULL;
		boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
	} catch(...){
		cout << "Unable to connect sender: " << prefix << destination <<".\nattempting again in 5 seconds.\n";
		msgSender = NULL;
		boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
	}
	return msgSender;
}

MessageReceiver* connectReceiver(MessagingProvider *provider, string prefix, string destination){
	MessageReceiver* msgReceiver = NULL;
	try {
		msgReceiver = receiver(provider, prefix, destination);
	} catch(const exception& error) {
		cout << "Unable to connect receiver: " << prefix << destination <<".\nattempting again in 5 seconds.\n";
		msgReceiver = NULL;
		boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
	} catch(...){
		cout << "Unable to connect receiver: " << prefix << destination <<".\nattempting again in 5 seconds.\n";
		msgReceiver = NULL;
		boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
	}
	return msgReceiver;
}

MessagingProvider* connectProvider(string address){
	MessagingProvider* msgProvider = NULL;
	try {
		msgProvider = new MessagingProvider(address.c_str());
		while(!msgProvider->connect()){
			cout << "Unable to connect to broker at addr: " << address << "\nattempting again in 5 seconds.\n";
			boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
		}
	} catch(const exception& error) {
		cout << "Unable to connect to broker at addr: " << address << "\nattempting again in 5 seconds.\n";
		msgProvider = NULL;
		boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
	} catch(...){
		cout << "Unable to connect to broker at addr: " << address << "\nattempting again in 5 seconds.\n";
		msgProvider = NULL;
		boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
	}
	return msgProvider;
}

void closeConnections(MessagingProvider* aqs1, MessagingProvider* aqs2, MessagingProvider* aqs3){
	if(aqs1 != NULL){
		try{
			aqs1->disconnect();
		} catch(...){}
	}
	if(aqs2 != NULL){
		try{
			aqs2->disconnect();
		} catch(...){}
	}
	if(aqs3 != NULL){
		try{
			aqs3->disconnect();
		} catch(...){}
	}
}

void serviceProvider(string broker, string prefix, string configFile){

		cout << "Creating service at addr: " << broker << 
		        ", queue prefix: " << prefix << 
		        //", config file: " << configFile << 
				"\n";
		MessagingProvider* aqs1 = NULL;
		MessagingProvider* aqs2 = NULL;
		MessagingProvider* aqs3 = NULL;
		bool connected = false;
		while(!connected){
			try{
				closeConnections(aqs1, aqs2, aqs3);
				aqs1 = connectProvider(broker);
				if(aqs1 == NULL) continue;
				aqs2 = connectProvider(broker);
				if(aqs2 == NULL) continue;
				aqs3 = connectProvider(broker);
				if(aqs3 == NULL) continue;
		
				MessageReceiver* requestReceiver = connectReceiver(aqs1, prefix, "Request");
				if(requestReceiver == NULL) continue;
				MessageSender* eventSender = connectSender(aqs2, prefix, "Event");
				if(eventSender == NULL) continue;
				MessageReceiver* commandReceiver = connectReceiver(aqs3, prefix, "Command");
				if(commandReceiver == NULL) continue;
				MessageSender* errorSender = connectSender(aqs3, prefix, "Error");
				if(errorSender == NULL) continue;

				connected = true;

				RemoteService<SpeechConfigRecord>* service = 
						(RemoteService<SpeechConfigRecord>*)
								new SpeechService((SpeechEngine*)SAPIEngine::instance());
				SpeechConfigRecord* conf = speechConfig(16000);

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
			} catch(const exception& error) {
				cout << "Speech server error.  Attempting to shutdown.";
			} catch(...){
				cout << "Speech server error.  Attempting to shutdown.";
			}
		}

		if(aqs1 != NULL){
			try{
				aqs1->disconnect();
			} catch(...){}
		}
		if(aqs1 != NULL){
			try{
				aqs2->disconnect();
			} catch(...){}
		}
		if(aqs1 != NULL){
			try{
				aqs3->disconnect();
			} catch(...){}
		}
		return;
	
		
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
