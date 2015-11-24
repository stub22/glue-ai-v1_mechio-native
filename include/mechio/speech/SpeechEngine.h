#ifndef SPEECH_ENGINE_H_
#define SPEECH_ENGINE_H_

#include <string.h>
#include <stdint.h> 
#include "mechio/messaging/MessageSender.h"
#include "mechio/speech/protocol/SpeechRequestRecord.h"

namespace mechio {
  namespace speech { 

    class SpeechEngine {
        public:
            virtual bool initialize(std::string voiceName, double sampleRate) = 0;
            virtual bool isInitialized() = 0;
            virtual void stop() = 0;

            virtual uint64_t speak(SpeechRequestRecord* request ) = 0;
//            virtual uint64_t speak(const std::string& phrase) = 0;
            virtual uint64_t cancelSpeech() = 0;
            
            virtual bool changeVoice(std::string voiceName) = 0;
            virtual bool changeSampleRate(int sampleRate) = 0;

            virtual void setEventSender(MessageSender* eventSender) = 0;
            virtual void startEventListener() = 0;
            virtual bool isListening() = 0;
            virtual void stopEventListener() = 0;

	};
}}
#endif
