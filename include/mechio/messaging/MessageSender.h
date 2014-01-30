/* 
 * File:   MessageSender.h
 * Author: matt
 *
 * Created on July 5, 2011, 12:48 AM
 */

#ifndef MESSAGE_SENDER_H
#define	MESSAGE_SENDER_H

#include <qpid/messaging/Message.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Session.h>
#include "mechio/messaging/MessagingUtils.h"

using namespace std;
using namespace qpid::messaging;


namespace mechio{
    class MessageSender{
        private:
            Sender mySender;

        public:
            MessageSender(const Sender &sender) :mySender(sender){}

            MessageSender(Session* session, const char* destination){
                mySender = session->createSender(destination);
            }

            template<typename T> void send(T* t, string contentType="", bool sync=false){
                Message message = packMessage<T>(t);
                message.setContentType(contentType);
                mySender.send(message, sync);
            }
            
            void close(){
                mySender.close();
            }
    };
}
#endif	/* MESSAGE_SENDER_H */

