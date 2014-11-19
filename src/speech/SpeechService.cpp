#include "boost/lexical_cast.hpp"
#include "mechio/speech/SpeechService.h"
#include "mechio/speech/protocol/SpeechRequestRecord.h"

using namespace mechio;
using namespace mechio::speech;


SpeechService::SpeechService(SpeechEngine* engine):mySpeechEngine(engine){}

void SpeechService::threadSafeWorker(){
	Message msg;
    if(myRequestReceiver == NULL){
        pause();
        return;
    }
	try{
		msg = myRequestReceiver->fetchRaw(Duration(1000));
	}catch(const NoMessageAvailable &ex){
		return;
	}
    myRequestReceiver->acknowledge(); 
	SpeechRequestRecord* req = unpackMessage<SpeechRequestRecord>(msg);
	if(req == NULL){
		cout << "Could not unpack message." << endl;
		return;
/*	} else if(req->speechServiceId != mySpeechServiceId){
        cout << "This request isn't for me." << endl;
        cout << "I am " << mySpeechServiceId << endl;
        cout << "This was for " << req->speechServiceId << endl;
        return;*/
    }
	cout << "Received Phrase: " << req->phrase << endl;
    if(mySpeechEngine == NULL){
        pause();
        return;
    }
    int len = req->phrase.length();
    
    if(len > 12){
        string start;
        start.assign(req->phrase, 0, 12);
        if(strcmp(start.c_str(), "changeVoice:") == 0){
            string voice;
            voice.assign(req->phrase, 12, len-12);
            cout << "changing voice to " << voice.c_str() << ".\n";
            mySpeechEngine->changeVoice(voice);
            return;
        }
    }
    
    if(len > 17){
        string start;
        start.assign(req->phrase, 0, 17);
        if(strcmp(start.c_str(), "changeSampleRate:") == 0){
            string sampleRate;
            sampleRate.assign(req->phrase, 17, len-17);
            try{
                int rate = boost::lexical_cast<int>(sampleRate);
                cout << "changing sample rate to " << rate << ".\n";
                mySpeechEngine->changeSampleRate(rate);
                return;
            } catch(...){
            }
        }
    }

//	mySpeechEngine->speak(req->phrase);
	mySpeechEngine->speak(req);
}

bool SpeechService::_initialize(SpeechConfigRecord* conf){
    bool ret = mySpeechEngine->initialize(conf->voiceName, conf->sampleRate);
    mySpeechServiceId = conf->speechServiceId;
    return ret;
}

void SpeechService::setEventSender(MessageSender* eventSender){
    myEventSender = eventSender;
    mySpeechEngine->setEventSender(eventSender);
	mySpeechEngine->startEventListener();
}

void SpeechService::setRequestReceiver(MessageReceiver* requestReceiver){
    myRequestReceiver = requestReceiver;    
}

void SpeechService::close(){
    mySpeechEngine->stop();
	myMessagingProvider->disconnect();
}

void SpeechService::handleServiceCommand(ServiceCommandRecord *cmd){
    if(mySpeechEngine == NULL || cmd == NULL){
        return;
    }
    if(cmd->command.compare("cancelSpeech") == 0){
        mySpeechEngine->cancelSpeech();
    }else if(cmd->command.compare("changeVoice") == 0){
        mySpeechEngine->changeVoice(cmd->destinationId);
    }else if(cmd->command.compare("changeSampleRate") == 0){
        try{
            int rate = boost::lexical_cast<int>(cmd->destinationId);
            mySpeechEngine->changeSampleRate(rate);
        } catch(...){
        }
    }
}

void SpeechService::handleCommandMessage(Message &msg){
}
