#include "mechio/messaging/MessageDrain.h"
#include <iostream>
#include "mechio/common/logger.h"

using namespace mechio;
using namespace mechio::common;

MessageDrain::MessageDrain(Session *session, const Receiver &receiver, int drainTimeout)
        :mySession(session), myReceiver(receiver), 
         myDrainTimeout(drainTimeout), myDrainCount(0){}

MessageDrain::MessageDrain(Session* session, const char* destination, int drainTimeout)
        :mySession(session), myDrainTimeout(drainTimeout), myDrainCount(0){
    myReceiver = session->createReceiver(destination);
}

void MessageDrain::drain(){
    cout << "Draining from receiver: " <<  myReceiver.getName() << endl;
    drainLoop();
    cout << myDrainCount << " messages drained." << endl;
    cout << "Draining complete - " << myReceiver.getName() << endl;
}

void MessageDrain::drainLoop(){
    try{
        while(myReceiver.fetch(myMessage, myDrainTimeout)){
            mySession->acknowledge();
            myDrainCount++;
            cout << ".";
        }
    }catch(const NoMessageAvailable &ex){
        return;
    }catch(const MessagingException &ex){
        mechio::common::error("Error while draining.", ex);
    }catch(...){
        mechio::common::error("Unspecified error while draining");
    }
}
