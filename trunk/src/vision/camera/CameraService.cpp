#include "mechio/vision/camera/CameraService.h"
#include <sys/time.h>

using namespace mechio;

bool CameraService::_initialize(CameraConfig* conf){
    MessagingProvider* stream = new MessagingProvider(
            conf->brokerAddress.c_str(), conf->connectionOptions.c_str());
    stream->connect();
    myFrameLength = (int)conf->frameLength;
    myDevice = (int)conf->cameraNumber;
    string dest = conf->eventDestination;
    myVideoSender = stream->createSender(dest.c_str());
    
    myCapture = cvCreateCameraCapture(myDevice);
    cvSetCaptureProperty(myCapture, CV_CAP_PROP_FRAME_WIDTH, conf->imageWidth);
    cvSetCaptureProperty(myCapture, CV_CAP_PROP_FRAME_HEIGHT, conf->imageHeight);
    int imgWidth = conf->imageWidth;
    int imgHeight = conf->imageHeight;
    int imgChannels = conf->imageChannels;
    
    int camWidth = conf->cameraWidth;
    int camHeight = conf->cameraHeight;
    int camChannels = conf->cameraChannels;
    
    myConvertColorFlag = (imgChannels != camChannels);
    myConvertColorHeader = cvCreateImage(cvSize(camWidth,camHeight),IPL_DEPTH_8U,imgChannels);
    
    myResizeFlag = (imgWidth != camWidth || imgHeight != camHeight);
    myResizeHeader = cvCreateImage(cvSize(imgWidth,imgHeight),IPL_DEPTH_8U,imgChannels);
    
    myFlipVerticalFlag = conf->flipImage;
    myFlipHorizontalFlag = conf->flipImageHorizontal;
    myFlipHeader = cvCreateImage(cvSize(imgWidth,imgHeight),IPL_DEPTH_8U,imgChannels);
    return true;
}

void CameraService::setFlipVertical(bool flip){
    myFlipVerticalFlag = flip;
}

void CameraService::setFlipHorizontal(bool flip){
    myFlipHorizontalFlag = flip;
}

void CameraService::close(){
    cvReleaseCapture(&myCapture);
}

void CameraService::threadSafeWorker(){
    double freq = cvGetTickFrequency();
    int64 start = cvGetTickCount();
    try{
        IplImage* img = cvQueryFrame(myCapture);
        //cout << "image received" << endl;
        IplImage* temp = img;
        if(myConvertColorFlag){
            cvConvertImage(temp,myConvertColorHeader);
            temp = myConvertColorHeader;
        }
        if(myResizeFlag){
            cvResize(temp,myResizeHeader);
            temp = myResizeHeader;
        }
        if(myFlipVerticalFlag && myFlipHorizontalFlag){
            cvFlip(temp, myFlipHeader, -1);
            temp = myFlipHeader;
        } else if(myFlipVerticalFlag){
            cvFlip(temp, myFlipHeader, 0);
            temp = myFlipHeader;
        } else if(myFlipHorizontalFlag){
            cvFlip(temp, myFlipHeader, 1);
            temp = myFlipHeader;
        }
        ImageRecord *pimg = packImage(temp);
        myVideoSender->send<ImageRecord>(pimg, "application/image-record", true);
        delete pimg;
		
        int64 end = cvGetTickCount();
        long elapsed = end - start;
        elapsed = elapsed/(freq*1000);
        long sleep = myFrameLength - elapsed;
        if(sleep > 0){
            //cout << "sleeping " << sleep << " msecs\n";
            boost::this_thread::sleep(boost::posix_time::milliseconds(sleep));
        }
    }catch(const Exception &ex){
        cout << ex.what() << endl;
    }
}

void CameraService::handleCommandMessage(Message &msg){}


int CameraService::getFrameLength(){
    return myFrameLength;
}

void CameraService::setFrameLength(int len){
    myFrameLength = len;
}
