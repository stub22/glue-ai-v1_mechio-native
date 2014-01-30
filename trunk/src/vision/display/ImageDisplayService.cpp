#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "mechio/messaging/MessagingProvider.h"
#include "mechio/vision/protocol/ImageRecord.h"
#include "mechio/vision/protocol/ImageRegionListRecord.h"
#include "mechio/vision/imageUtils.h"
#include "mechio/vision/display/ImageDisplayService.h"

using namespace mechio;

bool ImageDisplayService::_initialize(ImageDisplayConfig* conf){
    MessagingProvider* stream = new MessagingProvider(
            conf->brokerAddress.c_str(), conf->connectionOptions.c_str());
    stream->connect();
    myImageReceiver = stream->createReceiver(conf->imageEventDestination.c_str());
    myAnnotationReceiver = stream->createReceiver(conf->procEventDestination.c_str());
    myDrawTime = conf->drawRate;
    myWinName = conf->windowName;
	return true;
}

void ImageDisplayService::close(){
    cvDestroyWindow(myWinName.c_str());
}

void drawFaces(IplImage* image, ImageRegionListRecord* faceRegions){
    std::vector<ImageRegionRecord> faces = faceRegions->regions;
	for( std::vector<ImageRegionRecord>::const_iterator r = faces.begin(); r != faces.end(); r++){
        CvPoint center;
		center.x = cvRound((r->x + r->width*0.5));
        center.y = cvRound((r->y + r->height*0.5));
        int radius = cvRound((r->width + r->height)*0.25);
        CvScalar color = CV_RGB(255,255,255);
		cvDrawCircle(image,center,radius,color,1);
    }
}

void ImageDisplayService::threadSafeWorker(){
    Message msg;
    try{
        msg = myImageReceiver->fetchRaw(Duration(100));
    }catch(const NoMessageAvailable &ex){
        return;
    }catch(const MessagingException &ex){
        cout << "unknown messaging error: " << ex.what() << endl;
        return;
    }
    ImageRecord* pimg = unpackMessage<ImageRecord>(msg);
    myImageReceiver->acknowledge();
    if(pimg == NULL){
        cout << "Could not unpack message." << endl;
        return;
    }
	ImageRegionListRecord* regions = myAnnotationReceiver->fetch<ImageRegionListRecord>();
    uint8_t** buf = new uint8_t*[1];
    IplImage *img = unpackImage(pimg, buf);
    if(img == NULL){
        cout << "Could not unpack portable image." << endl;
        return;
    }
    drawFaces(img, regions);
    cvShowImage(myWinName.c_str(), img);
    cvWaitKey(myDrawTime); 
   
    pimg->data.clear();
    delete pimg;
    uint8_t* buf2 = buf[0];
    if(buf2 != NULL){
        delete[] buf2;
        delete[] buf;
    }
}

void ImageDisplayService::handleCommandMessage(Message &msg){}

