/* 
 * File:   MotionDetector.h
 * Author: matt
 *
 * Created on July 27, 2013, 2:12 AM
 */

#ifndef MOTIONDETECTOR_H
#define	MOTIONDETECTOR_H


#include <vector>
#include <string>
#include <opencv/cv.h>
#include "mechio/vision/protocol/ImageRegionListRecord.h"

using namespace std;

class MotionDetector {
public:
    MotionDetector(string imgProcId, string imgSrcId, int width, int height);
    ImageRegionListRecord* findMotions(IplImage* image);

private:
    int m_timestamp;
    int myWidth;
    int myHeight;
    string myProcId;
    string myImgSourceId;
    static const int timestep = 1;
    static const int timeduration = 10;

    IplImage* m_previous;
    IplImage* m_greyed;
    IplImage* m_calc;
    IplImage* m_mhi;
    IplImage* m_mhi_seg;

    CvMemStorage* m_mhi_memory;
    bool m_first;
    CvMat* m_edge;
};
#endif	/* MOTIONDETECTOR_H */

