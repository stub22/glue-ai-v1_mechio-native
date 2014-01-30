/* 
 * File:   RemoteServiceHost.h
 * Author: matt
 *
 * Created on July 14, 2011, 5:24 PM
 */

#ifndef RemoteServiceHost_H
#define	RemoteServiceHost_H

#include <string.h>
#include <iostream>
#include <exception>

#include "mechio/common/logger.h"
#include "mechio/messaging/RemoteService.h"
#include "mechio/messaging/MessagingProvider.h"
#include "mechio/messaging/protocol/ServiceCommandRecord.h"
#include "mechio/messaging/protocol/ServiceErrorRecord.h"

using namespace qpid::messaging;
using namespace std;

namespace mechio{
    template<class T>
    class RemoteServiceHost {
        private:
            MessageReceiver* myCommandReceiver;
            MessageSender* myErrorSender;
            RemoteService<T>* myService;
            
            void close(){
                myCommandReceiver->close();
                myErrorSender->close();
            }
            
            void commandLoop(){
                bool run = true;
                while(run){
                    try{
                        Message message = myCommandReceiver->fetchRaw();
                        myCommandReceiver->acknowledge();
                        string contentType = message.getContentType();
                        cout << "Service Provider received message with content type: " << contentType << endl;
                        if(contentType == "application/service-command"){
                            run = handleCommand(message);
                        }else{
                            handleOther(message);
                        }
                    }catch(const NoMessageAvailable &ex){}
                }
                close();
            }
            
            bool handleCommand(Message &message){
                ServiceCommandRecord* cmd = unpackMessage<ServiceCommandRecord>(message);
                cout << "Command Received: " << cmd->command << endl;
                if(cmd->command.compare("start") == 0){
                    myService->start();
                }else if(cmd->command.compare("pause") == 0){
                    myService->pause();
                }else if(cmd->command.compare("resume") == 0){
                    myService->resume();
                }else if(cmd->command.compare("stop") == 0){
                    myService->stop();
                    return false;//myService->myRunFlag || myService->myPauseFlag;
                }else{
                    myService->handleServiceCommand(cmd);
                }
                return true;
            }
            
            void handleOther(Message &message){
                cout << "Handling Custon Service Command" << endl;
                myService->handleCommandMessage(message);
            }
            
            T* fetchConfig(){
                try{
                    T* t = myCommandReceiver->fetch<T>();
                    if(t == NULL){
                        Message* msg = myCommandReceiver->getLastMessage();
                        if(msg == NULL){
                            return NULL;
                        }
                        cout << "Message is not the required config.  Discarding message.  Type: " 
                                << msg->getContentType() <<endl;
                        myCommandReceiver->acknowledge();
                    }
                    return t;
                }catch(const MessagingException &ex){
                    cout << "Messaging Error fetching config: " << ex.what() << endl;
                }catch(const Exception &ex){
                    cout << "Error fetching config: " << ex.what() << endl;
                }catch(...){
                    cout << "Unknown Error fetching config" << endl;
                }
                return NULL;
            }

            void initialize(){
                if(myService->isInitialized()){
                        return;
                }
                T* t = NULL;
                while(t == NULL){
                    t = fetchConfig();
                }
                myService->initialize(t);
            }
            
        public:
            RemoteServiceHost(RemoteService<T>* service, MessageReceiver* cmdRec, MessageSender* errSnd)
                : myService(service), myCommandReceiver(cmdRec), myErrorSender(errSnd){}
                
            bool start(){
                initialize();
                commandLoop();
                return true;
            }
    };
}

#endif	/* RemoteServiceHost_H */

