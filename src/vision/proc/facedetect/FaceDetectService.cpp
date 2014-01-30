#include "mechio/vision/proc/facedetect/FaceDetectService.h"
#include "mechio/messaging/MessagingProvider.h"
#include "mechio/vision/imageUtils.h"

using namespace mechio;

bool FaceDetectService::_initialize(FaceDetectConfig* conf){
    myConfig = conf;
	myStream = new MessagingProvider(
            conf->brokerAddress.c_str(), conf->connectionOptions.c_str());
    myStream->connect();
    
    string haarLocation = conf->haarFileLocation;
    CvSize dSize = cvSize(conf->minFaceWidth, conf->minFaceHeight);

    CvHaarClassifierCascade* myClassifier = (CvHaarClassifierCascade *)cvLoad(haarLocation.c_str());
    myFaceDetector = new FaceDetector(myClassifier, 
            conf->scaleFactor, 
            conf->neighbors,
            conf->haarFlags,
            dSize);

	return true;
}

void FaceDetectService::close(){}

void FaceDetectService::_start(){
    string recDest = myConfig->imageEventDestination;
    string sendDest = myConfig->procEventDestination;
    myImageReceiver = myStream->createReceiver(recDest.c_str());
    myResultsSender = myStream->createSender(sendDest.c_str());
}

void FaceDetectService::threadSafeWorker(){
    Message msg;
    try{
        msg = myImageReceiver->fetchRaw(Duration(100));
    }catch(const NoMessageAvailable &ex){
        return;
    }catch(const MessagingException &ex){
        cout << "unknown messaging error: " << ex.what() << endl;
        return;
    }
    myImageReceiver->acknowledge(); 
    ImageRecord* pimg = unpackMessage<ImageRecord>(msg);
    if(pimg == NULL){
        cout << "Could not unpack message." << endl;
        return;
    }
    uint8_t** buf = new uint8_t*[1];
    IplImage *img = unpackImage(pimg, buf);
    if(img == NULL){
        cout << "Could not unpack portable image." << endl;
        return;
    }
    ImageRegionListRecord* regions = myFaceDetector->findFaces(img);
    myResultsSender->send<ImageRegionListRecord>(regions, "application/image-regions");
   
    pimg->data.clear();
    delete pimg;
    uint8_t* buf2 = buf[0];
    if(buf2 != NULL){
        delete[] buf2;
        delete[] buf;
    }
}

void FaceDetectService::handleCommandMessage(Message &msg){}
