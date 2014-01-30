#include <fstream>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h> 
#include "mechio/vision/imageUtils.h"
#include <avro/Specific.hh>
#include <avro/Encoder.hh>
#include <avro/Decoder.hh>
#include "mechio/common/logger.h"

using namespace std;
using namespace avro;
using namespace mechio;
using namespace mechio::common;

IplImage* unpackImage(ImageRecord* pimg, uint8_t** buff){
    if(pimg == NULL){
        return NULL;
    }
    int width = pimg->width;
    int height = pimg->height;
    int channels = pimg->nChannels;
    int widthStep = pimg->widthStep;
    CvSize size = cvSize(width, height);
    IplImage* image = NULL;
    uint8_t* buf = NULL;
    try{
        image = cvCreateImageHeader(size, IPL_DEPTH_8U, channels);
        cvInitImageHeader(image, size, IPL_DEPTH_8U, channels);
        int dataSize = widthStep*height;
        buf = new uint8_t[dataSize];
        for(int i=0; i<pimg->data.size(); i++){
            buf[i] = pimg->data[i];
        }

        cvSetData(image, buf, widthStep);
        image->imageDataOrigin = (char*)buf;
    }catch(Exception &ex){
        mechio::common::error("Bad Image Data", ex);
        image = NULL;
    }catch(...){
        mechio::common::error("Unable to unpack PortableImage");
        image = NULL;
    }
    buff[0] = buf;
    return image;
}

ImageRecord* packImage(IplImage *img){
    ImageRecord* pi = new ImageRecord();
	pi->imageId = 0;
    pi->imageSourceId = "cam0";
    pi->imageTimestampMillisecUTC = 0;
    pi->width = img->width;
    pi->height = img->height;
    pi->nChannels = img->nChannels;
    pi->widthStep = img->widthStep;
    int dataSize = img->imageSize;
    
    vector<uint8_t> data(dataSize);
    for(int i=0; i<dataSize; i++){
        data[i] = img->imageData[i];
    }
    pi->data = data;
    return pi;	
}
