/* 
 * File:   MotionDetectService.h
 * Author: matt
 *
 * Created on July 27, 2013, 8:40 PM
 */

#ifndef MOTIONDETECTSERVICE_H
#define	MOTIONDETECTSERVICE_H


#include <sys/time.h>
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <cstdlib>
#include <vector>
#include <qpid/messaging/Message.h>
#include "string.h"

#include "mechio/messaging/RemoteService.h"
#include "mechio/vision/protocol/ImageRecord.h"
#include "mechio/messaging/MessageReceiver.h"
#include "mechio/messaging/MessageSender.h"
#include "mechio/messaging/MessagingProvider.h"
#include "mechio/vision/proc/motiondetect/MotionDetector.h"
#include "mechio/vision/proc/motiondetect/protocol/MotionDetectConfig.h"


using namespace std;
using namespace qpid::messaging;

namespace mechio{
    class MotionDetectService : public RemoteService<MotionDetectConfig>{
        private:
            MessagingProvider* myStream;
            MessageReceiver* myImageReceiver;
            MessageSender* myResultsSender;
            MotionDetector* myMotionDetector;
            MotionDetectConfig* myConfig;
            
            void threadSafeWorker();
            bool _initialize(MotionDetectConfig* conf);
            void close();

        public:
            virtual void _start();
            void handleCommandMessage(Message &msg);
            void handleServiceCommand(ServiceCommandRecord *cmd){}
    };
}

#endif	/* MOTIONDETECTSERVICE_H */

