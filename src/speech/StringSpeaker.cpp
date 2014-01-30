#include "mechio/speech/StringSpeaker.h"

using namespace mechio::speech;

StringSpeaker::StringSpeaker(MessageSender* sender, string serviceId){
    mySpeechSender = sender;
    myServiceId.assign(serviceId);
}

void StringSpeaker::speak(string text){
    SpeechRequestRecord* req = new SpeechRequestRecord();
    req->phrase.assign(text);
    req->speechServiceId.assign(myServiceId);
    mySpeechSender->send<SpeechRequestRecord>(req);
}
