/* 
 * File:   StringSpeaker.h
 * Author: mechio
 *
 * Created on January 3, 2012, 3:10 PM
 */

#ifndef STRINGSPEAKER_H
#define	STRINGSPEAKER_H

#include <string>

#include "mechio/messaging/MessageSender.h"
#include "mechio/speech/protocol/SpeechRequestRecord.h"


namespace mechio{
  namespace speech{

    class StringSpeaker{
        private:
            MessageSender* mySpeechSender;
            string myServiceId;

        public:
            StringSpeaker(MessageSender* sender, string serviceId);
            void speak(string text);
    };
}}
#endif	/* STRINGSPEAKER_H */
