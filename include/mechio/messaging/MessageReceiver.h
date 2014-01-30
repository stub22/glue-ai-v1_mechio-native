/* 
 * File:   MessageReceiver.h
 * Author: matt
 *
 * Created on July 5, 2011, 12:49 AM
 */

#ifndef MessageReceiver_H
#define	MessageReceiver_H

#include <qpid/messaging/Message.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Session.h>
#include "mechio/messaging/MessagingUtils.h"

using namespace std;
using namespace qpid::messaging;

namespace mechio{
    class MessageReceiver {
        private:
            Session* mySession;
            Receiver myReceiver;
            Message myMessage;
            bool myAutoAcknowledge;
        public:
            MessageReceiver(Session *session, const Receiver &receiver, bool autoAcknowledge=true)
                    :mySession(session), myReceiver(receiver), myAutoAcknowledge(autoAcknowledge){}

            MessageReceiver(Session* session, const char* destination, bool autoAcknowledge=true)
                    :mySession(session), myAutoAcknowledge(autoAcknowledge){
                myReceiver = session->createReceiver(destination);
            }

            bool isAutoAcknowledge(){
                return myAutoAcknowledge;
            }

            void setAutoAcknowledge(bool val){
                myAutoAcknowledge = val;
            }

            template<typename T> T* fetch(Duration timeout=Duration::FOREVER){
                bool fetched = myReceiver.fetch(myMessage, timeout);
                if(!fetched) {
                    return NULL;
                }
                T* t = unpackMessage<T>(myMessage);
                if(t == NULL){
                    return NULL;
                }
                if(myAutoAcknowledge){
                    mySession->acknowledge(myMessage);
                }
                return t;
            }
            
            Message fetchRaw(Duration timeout=Duration::FOREVER){
                return myReceiver.fetch(timeout);
            }
            
            Message getRaw(Duration timeout=Duration::FOREVER){
                return myReceiver.get(timeout);
            }
            
            Message* getLastMessage(){
                return &myMessage;
            }

            void acknowledge(bool sync=false){
                mySession->acknowledge(sync);
            }
            
            void close(){
                myReceiver.close();
            }
    };
}
#endif	/* MessageReceiver_H */

