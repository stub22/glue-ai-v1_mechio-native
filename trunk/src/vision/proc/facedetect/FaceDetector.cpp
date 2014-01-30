#include "mechio/vision/proc/facedetect/FaceDetector.h"
#include <iostream>
#include <vector>

FaceDetector::FaceDetector(CvHaarClassifierCascade* classifier, double scale, int neighbors, int flags, CvSize detectSize) : 
        myClassifier(classifier), 
        myScaleFactor(scale), 
        myNeighbors(neighbors), 
        myHaarFlags(flags), 
        myDetectSize(detectSize){
	myStorage = cvCreateMemStorage();
}

ImageRegionListRecord* FaceDetector::findFaces(IplImage* image){
    //double t = (double)cvGetTickCount();
	CvSeq* detectedFaces = 0;
    ImageRegionListRecord* regions = new ImageRegionListRecord();  
    regions->imageId = 0;
    regions->imageProcessorId = "faceDetect0";
    regions->imageRegionsId = 0;
    regions->imageSourceId = "cam0";
    regions->imageTimestampMillisecUTC = 0;
    regions->processorStartTimestampMillisecUTC = 0;
    regions->processorCompleteTimestampMillisecUTC = 0;
    detectedFaces = cvHaarDetectObjects(image, myClassifier, myStorage, myScaleFactor, myNeighbors, 
			      myHaarFlags, myDetectSize);

	int numFaces = (detectedFaces ? detectedFaces->total : 0);
  
	for(int i = 0; i < numFaces; ++i){
		CvRect* r = (CvRect*)cvGetSeqElem(detectedFaces, i);
        ImageRegionRecord ir;
        ir.x = r->x;
        ir.y = r->y;
        ir.height = r->height;
        ir.width = r->width;
        ir.regionId = 0;
		regions->regions.push_back(ir);
	}
	//t = (double)cvGetTickCount() - t;
	//std::cout << "num faces = " << numFaces << ",\tdetection time = " << t/((double)cvGetTickFrequency())*1000 << " ms\n";
	if (detectedFaces != 0){
		cvClearSeq(detectedFaces);
	}
	cvClearMemStorage(myStorage);
    return regions;
}