#include "mechio/vision/camera/WinCameraService.h"
#include <sys/time.h>
#include <windows.h>

using namespace mechio;

bool WinCameraService::_initialize(CameraConfig* conf){
    MessagingProvider* stream = new MessagingProvider(
            conf->brokerAddress.c_str(), conf->connectionOptions.c_str());
    stream->connect();
    myFrameLength = (int)conf->frameLength;
    myDevice = (int)conf->cameraNumber;
    string dest = conf->eventDestination;
    myVideoSender = stream->createSender(dest.c_str());

    int imgWidth = conf->imageWidth;
    int imgHeight = conf->imageHeight;
    int imgChannels = conf->imageChannels;
    
    int camWidth = conf->cameraWidth;
    int camHeight = conf->cameraHeight;
    int camChannels = conf->cameraChannels;

	myVideo.setUseCallback(true);
	myVideo.setIdealFramerate(myDevice, myFrameLength);
	myVideo.setupDevice(myDevice, camWidth, camHeight, VI_COMPOSITE);
    
    myConvertColorFlag = (imgChannels != camChannels);
    myConvertColorHeader = cvCreateImage(cvSize(camWidth, camHeight),IPL_DEPTH_8U,imgChannels);
    
    myResizeFlag = (imgWidth != camWidth || imgHeight != camHeight);
    myResizeHeader = cvCreateImage(cvSize(imgWidth,imgHeight),IPL_DEPTH_8U,imgChannels);
	myImageData = new unsigned char[myVideo.getSize(myDevice)];
    
    myFlipVerticalFlag = conf->flipImage;
    myFlipHorizontalFlag = conf->flipImageHorizontal;
    myFlipHeader = cvCreateImage(cvSize(imgWidth,imgHeight),IPL_DEPTH_8U,imgChannels);

	return true;
}

void WinCameraService::setFlipVertical(bool flip){
    myFlipVerticalFlag = flip;
}

void CameraService::setFlipHorizontal(bool flip){
    myFlipHorizontalFlag = flip;
}

void WinCameraService::close(){

}

void WinCameraService::threadSafeWorker(){
    double freq = cvGetTickFrequency();
	int64 start = static_cast<int>(GetTickCount());
    try{
        IplImage* img = getImage();
		if(img == NULL){
            boost::this_thread::sleep(boost::posix_time::milliseconds(3));
			return;
		}
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
        
		
		int64 end = static_cast<int>(GetTickCount());
        long elapsed = end - start;
        elapsed = elapsed/(freq*1000);
        long sleep = myFrameLength - elapsed;
        if(sleep > 0){
            boost::this_thread::sleep(boost::posix_time::milliseconds(sleep));
		}
        delete pimg;
    }catch(const cv::Exception &ex){
        cout << ex.what() << endl;
    }
}

IplImage* WinCameraService::getImage(){
	if(!myVideo.isFrameNew(myDevice)){
		return NULL;
	}
	myVideo.getPixels(myDevice,myImageData, true);
	int w = myVideo.getWidth(myDevice);
	int h = myVideo.getHeight(myDevice);
	CvSize size = cvSize(w,h);
	IplImage* img = cvCreateImageHeader(size, IPL_DEPTH_8U, 3);
	if(img == NULL){
		return NULL;
	}
	cvInitImageHeader(img, size, IPL_DEPTH_8U, 3, IPL_ORIGIN_BL);
    img->imageData = (char*)myImageData;
    img->imageDataOrigin = img->imageData;

	//my camera image is inverted so we flip it
	cvFlip(img, img);
	return img;
}

void WinCameraService::handleCommandMessage(Message &msg){}


int WinCameraService::getFrameLength(){
    return myFrameLength;
}

void WinCameraService::setFrameLength(int len){
    myFrameLength = len;
}
