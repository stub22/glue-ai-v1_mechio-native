/* 
 * File:   CameraService.h
 * Author: matt
 *
 * Created on July 15, 2011, 4:17 AM
 */

#ifndef CAMERASERVICE_H
#define	CAMERASERVICE_H

#include <string.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include "mechio/messaging/MessagingProvider.h"
#include "mechio/messaging/MessageReceiver.h"
#include "mechio/messaging/MessageSender.h"
#include "mechio/messaging/RemoteService.h"
#include "mechio/vision/imageUtils.h"
#include "mechio/vision/protocol/ImageRecord.h"
#include "mechio/vision/camera/protocol/CameraConfig.h"

namespace mechio{
    class CameraService : public RemoteService<CameraConfig> {
        private:
            MessageSender* myVideoSender;
            int myDevice;
            int myFrameLength;
            CvCapture* myCapture;

            bool myConvertColorFlag;
            IplImage *myConvertColorHeader;
            bool myResizeFlag;
            IplImage *myResizeHeader;
            bool myFlipVerticalFlag;
            bool myFlipHorizontalFlag;
            IplImage *myFlipHeader;
            
            void threadSafeWorker();
            bool _initialize(CameraConfig* conf);
            void close();
        public:
            void handleCommandMessage(Message &msg);
            int getFrameLength();
            void setFrameLength(int len);
            void handleServiceCommand(ServiceCommandRecord *cmd){}
            void setFlipVertical(bool flip);
            void setFlipHorizontal(bool flip);
    };
}

#endif	/* CAMERASERVICE_H */

