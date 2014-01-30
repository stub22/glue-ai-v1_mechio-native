/* 
 * File:   FaceDetector.h
 * Author: matt
 *
 * Created on July 9, 2011, 2:12 AM
 */

#ifndef FACEDETECTOR_H
#define	FACEDETECTOR_H


#include <vector>
#include <opencv/cv.h>
#include "mechio/vision/protocol/ImageRegionListRecord.h"

class FaceDetector{
    public:
        FaceDetector(CvHaarClassifierCascade* classifier, 
                double scale=1.1, 
                int neighbors=5, 
                int flags=CV_HAAR_DO_CANNY_PRUNING, 
                CvSize detectSize=cvSize(40,40));
        ImageRegionListRecord* findFaces(IplImage* image);
        
    private:
        CvHaarClassifierCascade *myClassifier;
        CvMemStorage *myStorage;
        
        double myScaleFactor;
        int myNeighbors;
        int myHaarFlags;
        CvSize myDetectSize;
};

#endif	/* FACEDETECTOR_H */

