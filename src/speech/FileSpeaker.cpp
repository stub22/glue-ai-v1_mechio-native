#include "mechio/speech/FileSpeaker.h"
#include "mechio/common/logger.h"
#include <fcntl.h>

#define BUFFER_LENGTH 512

using namespace std;
using namespace mechio::speech;

FileSpeaker::FileSpeaker(StringSpeaker* speaker, string path){
    myStringSpeaker = speaker;
    myPipePath = path;
    myRunningFlag = false;
    myStopFlag = true;
}

void FileSpeaker::start(bool async){
    myStopFlag = false;
    if(async){
        launchLoopThread();
    }else{
        startLoop();
    }
}
void FileSpeaker::stop(){
    myStopFlag = true;
}

void FileSpeaker::stopAndWait(){
    myStopFlag = true;
    while(myRunningFlag){
        usleep(1000);
    }
}

void FileSpeaker::launchLoopThread(){
    if(myWorkerThread != NULL){
        return;
    }
    myWorkerThread = new boost::thread(FileSpeaker::runLoop, this);
}

void FileSpeaker::startLoop(){
    myRunningFlag = true;
    cout << "starting file loop\n";
    while(!myStopFlag){
        loopWorker();
    }
    cout << "ending file loop\n";
    myRunningFlag = false;
}

void FileSpeaker::loopWorker(){
    string text = readString();
    if(text == STOP_PHRASE){
        myStopFlag = true;
    }else if(hasReadErrors()){
        handleReadErrors();
    }else if(hasStringErrors(text)){
        handleStringErrors(text);
    }else{
        string cleanText = sanitizeSpeechInput(text);
        cout << "Speaking: " << cleanText << "\n";
        speakString(cleanText);
    }
}

string FileSpeaker::readString(){
    cout << "Waiting for text.\n";
    myFileDescriptor = open(myPipePath.c_str(), O_RDONLY);
    string text = "";
    char buffer[BUFFER_LENGTH];
    int t = 0;
    int n = read(myFileDescriptor, buffer, BUFFER_LENGTH);
    while(n > 0){
        if(n < BUFFER_LENGTH){
            buffer[n] = '\0';
        }
        text.append(buffer);
        t += n;
        n = read(myFileDescriptor, buffer, BUFFER_LENGTH);
    }
    text.assign(buffer);
    close(myFileDescriptor);
    return text;
}

bool FileSpeaker::hasReadErrors(){
    return false;
}

void FileSpeaker::handleReadErrors(){
    cout << "Read Error.\n";
}

bool FileSpeaker::hasStringErrors(string text){
    return false;
}

void FileSpeaker::handleStringErrors(string text){
    cout << "String Error.\n";
}

string FileSpeaker::sanitizeSpeechInput(string input){
    int len = input.length();
    if(input.at(len-1) == '\n'){
        input.assign(input.c_str(), len-1);
    }
    return input;
}

void FileSpeaker::speakString(string text){
    if(text.empty()){
        return;
    }
    myStringSpeaker->speak(text);
}