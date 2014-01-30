/* 
 * File:   main.cpp
 * Author: mechio
 *
 * Created on January 3, 2012, 4:40 PM
 */

#include <cstdlib>
#include <fstream>
#include <sys/stat.h>
#include "mechio/messaging/MessagingProvider.h"
#include "mechio/messaging/MessageSender.h"
#include "mechio/speech/StringSpeaker.h"
#include "mechio/speech/FileSpeaker.h"

using namespace std;
using mechio::MessagingProvider;
using mechio::MessageSender;
using namespace mechio::speech;

MessagingProvider* theMessagingProvider;
MessageSender* theMessageSender;
StringSpeaker* theStringSpeaker;
ifstream* theFileStream;
FileSpeaker* theFileSpeaker;

void connectStringSpeaker(string ip, string opts, string dest, string service){
    cout << "connecting to qpid tts.\n";
    theMessagingProvider = new MessagingProvider(ip.c_str(), opts.c_str());
    theMessagingProvider->connect();
    theMessageSender = theMessagingProvider->createSender(dest.c_str());
    theStringSpeaker = new StringSpeaker(theMessageSender, service);
    cout << "connected.\n";
}

void openOrCreateFileStream(string path){
    cout << "opening named pipe: " << path << "\n";
    theFileStream = new ifstream(path.c_str());
    if(!theFileStream){
        cout << "could not open pipe, attempting to create\n";
        mkfifo(path.c_str(), 0777);
        theFileStream = new ifstream(path.c_str());
        if(!theFileStream || !theFileStream->is_open()){
            cout << "unable to open or create pipe\n";
            theFileStream = NULL;
            return;
        }
    }
    cout << "pipe opened\n";
}

bool initializeFileReader(string path){
    //openOrCreateFileStream(path);
    //if(!theFileStream){
    //    cout << "unable to open pipe.\n";
    //    return false;
    //}
    theFileSpeaker = new FileSpeaker(theStringSpeaker, path);
    return true;
}


/*
 * 
 */
int main(int argc, char** argv) {
    string path, ip, opts, dest, service;
    if(argc > 1){
        path.assign(argv[1]);
    }else{
        path.assign("/tmp/speech_pipe");
    }
    if(argc > 2){
        ip.assign(argv[2]);
        ip.append(":5672");
    }else{
        ip.assign("localhost:5672");
    }
    if(argc > 3){
        opts.assign(argv[3]);
    }else{
        opts.assign("{username:admin,password:admin}");
    }
    if(argc > 4){
        dest.assign(argv[4]);
    }else{
        dest.assign("speechRequest; {create: always, node: {type: queue}}");
    }
    if(argc > 5){
        service.assign(argv[5]);
    }else{
        service.assign("acapelaSpeechService");
    }
    connectStringSpeaker(ip, opts, dest, service);
    if(!initializeFileReader(path)){
        return 0;
    }
    theFileSpeaker->start(false);
    return 0;
}

