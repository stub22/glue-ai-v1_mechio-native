/* 
 * File:   MessagingProvider.h
 * Author: matt
 *
 * Created on July 4, 2011, 8:47 PM
 */
#ifndef MessagingProvider_H
#define	MessagingProvider_H

#include <string.h>
#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Message.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Session.h>
#include "mechio/messaging/MessageReceiver.h"
#include "mechio/messaging/MessageSender.h"
#include "mechio/messaging/MessageDrain.h"

#define BROKER_LEN 1024
#define OPTIONS_LEN 1024

using namespace std;
using namespace qpid::messaging;

/*Avro-Qpid Stream*/
namespace mechio{
    class MessagingProvider{
        public:
            MessagingProvider(const char* broker="localhost:5672", const char* options="{username:admin,password:admin}");

            char* getBrokerAddress();
            void setBrokerAddress(const char* address);

            char* getConnectionOptions();
            void setConnectionOptions(const char* options);

            bool connect();
            void disconnect();

            MessageSender* createSender(const char* destination);
            MessageReceiver* createReceiver(const char* destination);
            MessageDrain* createDrain(const char* destination);
        protected:
            char* myBrokerAddress;
            char* myConnectionOptions;

            Connection* myConnection;
            Session mySession;
    };
}

#endif	/* MessagingProvider_H */

