#include "mechio/messaging/MessagingProvider.h"
using namespace mechio;

MessagingProvider::MessagingProvider(const char* broker, const char* options){
    myBrokerAddress = new char[BROKER_LEN];
    myConnectionOptions = new char[OPTIONS_LEN];
    strncpy(myBrokerAddress,broker,BROKER_LEN-1);
    strncpy(myConnectionOptions,options,OPTIONS_LEN-1);
}

char* MessagingProvider::getBrokerAddress(){
    return myBrokerAddress;
}

void MessagingProvider::setBrokerAddress(const char* address){
    strncpy(myBrokerAddress,address,BROKER_LEN-1);
}

char* MessagingProvider::getConnectionOptions(){
    return myConnectionOptions;
}

void MessagingProvider::setConnectionOptions(const char* options){
    strncpy(myConnectionOptions,options,OPTIONS_LEN-1);
}

bool MessagingProvider::connect(){
    myConnection = new Connection(myBrokerAddress, myConnectionOptions);
    try {
        myConnection->open();
        //if(!myConnection->isOpen()){
        //    return false;
        //}
        mySession = myConnection->createSession();
        return true;
    } catch(const exception& error) {
        return false;
    } catch(...){
        return false;
    }
}

void MessagingProvider::disconnect(){
    if(!mySession.isNull()){
        mySession.close();
    }
    if(myConnection->isOpen()){
            myConnection->close();
    }
    myConnection = NULL;
}

MessageReceiver* MessagingProvider::createReceiver(const char* destination){
    return new MessageReceiver(&mySession,destination);
}

MessageSender* MessagingProvider::createSender(const char* destination){
    return new MessageSender(&mySession, destination);
}

MessageDrain* MessagingProvider::createDrain(const char* destination){
    return new MessageDrain(&mySession, destination);
}