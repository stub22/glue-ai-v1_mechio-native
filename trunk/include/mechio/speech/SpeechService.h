/* 
 * File:   AcapelaService.h
 * Author: matt
 *
 * Created on November 15, 2011, 11:28 AM
 */

#ifndef SPEECH_SERVICE_H
#define	SPEECHS_ERVICE_H

#include <string.h>
#include "mechio/messaging/RemoteService.h"
#include "mechio/messaging/MessagingProvider.h"
#include "mechio/messaging/MessageReceiver.h"
#include "mechio/messaging/MessageSender.h"
#include "mechio/speech/SpeechEngine.h"
#include "mechio/speech/protocol/SpeechConfigRecord.h"

using namespace std;
using mechio::RemoteService;

namespace mechio{
  namespace speech{

    class SpeechService : public RemoteService<SpeechConfigRecord>{
        private:
            SpeechEngine* mySpeechEngine;
            MessagingProvider* myMessagingProvider;
            MessageReceiver* myRequestReceiver;
            MessageSender* myEventSender;
            string mySpeechServiceId;

            void threadSafeWorker();
            bool _initialize(SpeechConfigRecord* conf);

        public:
            SpeechService(SpeechEngine* engine);
            void setRequestReceiver(MessageReceiver* requestReceiver);
            void setEventSender(MessageSender* eventSender);
            void handleCommandMessage(Message &msg);
            void handleServiceCommand(ServiceCommandRecord *cmd);
            void close();
    };
}}

#endif	/* SPEECH_SERVICE_H */
