/* 
 * File:   RemoteService.h
 * Author: matt
 *
 * Created on July 14, 2011, 5:19 PM
 */

#ifndef RemoteService_H
#define	RemoteService_H

#include <iostream>
#include <boost/thread.hpp>
#include <qpid/messaging/Message.h>
#include "mechio/common/logger.h"
#include "mechio/messaging/protocol/ServiceCommandRecord.h"

using namespace std;
using namespace qpid::messaging;

namespace mechio{
    
    template<class T>
    class RemoteService{
        private:
            bool myInitFlag;
            bool myRunFlag;
            bool myPauseFlag;
            bool myCloseFlag;
            boost::thread* myWorkerThread;
            
            virtual void threadSafeWorker() = 0;
            virtual bool _initialize(T* t) = 0;
            virtual void close() = 0;
            
            static void runLoop(RemoteService* service){
                service->myCloseFlag = false;
                try{
                    while(service->myRunFlag){
                        if(service->myPauseFlag){
                            boost::this_thread::sleep(boost::posix_time::milliseconds(10));
                        }else{
                            service->threadSafeWorker();
                        }
                    }
                    mechio::common::info("MechIO Service Closing");
                }catch(...){}
                service->close();
                service->myCloseFlag = true;
            }
        public:
            RemoteService() : myInitFlag(false), myRunFlag(false), myPauseFlag(false){}
            
            virtual void handleCommandMessage(Message &msg) = 0;
            virtual void handleServiceCommand(ServiceCommandRecord *cmd) = 0;
            
            bool initialize(T* t){
                if(!myInitFlag){
                    mechio::common::info("Service Initializing.");
                    myInitFlag = _initialize(t);
                }
                return myInitFlag;
            }
            
            bool isInitialized(){
                return myInitFlag;
            }

            virtual void _start(){}
            void start(){
                if(myInitFlag && !myRunFlag){
                    mechio::common::info("MechIO Service received Start Command");
                    _start();
                    myRunFlag = true;
                    myWorkerThread = new boost::thread(RemoteService::runLoop, this);
                }
            }
            void pause(){
                if(myRunFlag){
                    mechio::common::info("MechIO Service received Pause Command");
                    myPauseFlag = true;
                }
            }
            void resume(){
                if(myRunFlag && myPauseFlag){
                    mechio::common::info("MechIO Service received Resume Command");
                    myPauseFlag = false;
                }
            }
            void stop(){
                mechio::common::info("MechIO Service received Stop Command");
                if(myRunFlag || myPauseFlag){
                    myRunFlag = false;
                    myPauseFlag = true;
                    return;
                }else{
                    int elapsed = 0;
                    int maxWait = 5000;
                    while(!myCloseFlag && elapsed < maxWait){
                        boost::this_thread::sleep(boost::posix_time::milliseconds(5));
                        elapsed += 5;
                    }
                }
            }
    };
}
#endif	/* RemoteService_H */

