/* 
 * File:   ImageDisplayService.h
 * Author: matt
 *
 * Created on July 15, 2011, 5:35 AM
 */

#ifndef IMAGEDISPLAYSERVICE_H
#define	IMAGEDISPLAYSERVICE_H


#include <cstdlib>
#include <string.h>
#include <qpid/messaging/Message.h>
#include "mechio/messaging/RemoteService.h"
#include "mechio/messaging/MessageReceiver.h"
#include "mechio/vision/display/protocol/ImageDisplayConfig.h"


using namespace std;
using namespace qpid::messaging;

namespace mechio{
    class ImageDisplayService : public RemoteService<ImageDisplayConfig>{
        private:
            MessageReceiver* myImageReceiver;
            MessageReceiver* myAnnotationReceiver;
            int myDrawTime;
            Message myMessage;
            string myWinName;

            void threadSafeWorker();
            
            bool _initialize(ImageDisplayConfig* conf);
            void close();
        public:
            void handleCommandMessage(Message &msg);
            int getFrameLength();
            void setFrameLength(int len);    
            void handleServiceCommand(ServiceCommandRecord *cmd){}
    };
}

#endif	/* IMAGEDISPLAYSERVICE_H */

