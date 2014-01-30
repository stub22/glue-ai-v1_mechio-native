/* 
 * File:   FaceDetectService.h
 * Author: matt
 *
 * Created on July 15, 2011, 8:40 PM
 */

#ifndef FACEDETECTSERVICE_H
#define	FACEDETECTSERVICE_H


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
#include "mechio/vision/proc/facedetect/FaceDetector.h"
#include "mechio/vision/proc/facedetect/protocol/FaceDetectConfig.h"


using namespace std;
using namespace qpid::messaging;

namespace mechio{
    class FaceDetectService : public RemoteService<FaceDetectConfig>{
        private:
            MessagingProvider* myStream;
            MessageReceiver* myImageReceiver;
            MessageSender* myResultsSender;
            FaceDetector* myFaceDetector;
            FaceDetectConfig* myConfig;
            
            void threadSafeWorker();
            bool _initialize(FaceDetectConfig* conf);
            void close();

        public:
            virtual void _start();
            void handleCommandMessage(Message &msg);
            void handleServiceCommand(ServiceCommandRecord *cmd){}
    };
}

#endif	/* FACEDETECTSERVICE_H */

