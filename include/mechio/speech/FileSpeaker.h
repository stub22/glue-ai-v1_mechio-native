/* 
 * File:   FileSpeaker.h
 * Author: Matthew Stevenson
 *
 * Created on January 3, 2012, 3:10 PM
 */

#ifndef FILESPEAKER_H
#define	FILESPEAKER_H

#include <string>
#include <istream>
#include <fstream>
#include <boost/thread.hpp>
#include "mechio/speech/StringSpeaker.h"

#define STOP_PHRASE ":stop:"


namespace mechio{
  namespace speech{
    
    class FileSpeaker{
        private:
            StringSpeaker* myStringSpeaker;
            string myPipePath;
            int myFileDescriptor;
            boost::thread* myWorkerThread;
            bool myRunningFlag;
            bool myStopFlag;


            void launchLoopThread();
            void startLoop();
            void loopWorker();
            string readString();
            bool hasReadErrors();
            void handleReadErrors();
            bool hasStringErrors(string text);
            void handleStringErrors(string text);
            string sanitizeSpeechInput(string input);
            void speakString(string text);

            static void runLoop(FileSpeaker* fs){
                fs->startLoop();
            }

        public:
            FileSpeaker(StringSpeaker* speaker, string path);
            void start(bool async=false);
            void stop();
            void stopAndWait();
    };
}}

#endif	/* FILESPEAKER_H */