/* 
 * File:   aqDrain.h
 * Author: matt
 *
 * Created on July 5, 2011, 12:52 PM
 */

#ifndef AQDRAIN_H
#define	AQDRAIN_H

#include <qpid/messaging/Message.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Session.h>

using namespace std;
using namespace qpid::messaging;

namespace mechio{
    class MessageDrain {
        private:
            Session* mySession;
            Receiver myReceiver;
            Message myMessage;
            Duration myDrainTimeout;
            int myDrainCount;

            void drainLoop();

        public:
            MessageDrain(Session *session, const Receiver &receiver, int drainTimeout=200);

            MessageDrain(Session* session, const char* destination, int drainTimeout=200);

            void drain();
    };
}

#endif	/* AQDRAIN_H */

