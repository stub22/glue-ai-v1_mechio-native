#include "mechio/vision/proc/motiondetect/MotionDetector.h"
#include <iostream>
#include <vector>
#include <opencv/highgui.h>

MotionDetector::MotionDetector(string imgProcId, string imgSrcId, int width, int height) 
        : m_first(true), m_timestamp(0),myWidth(width), myHeight(height) {
    myProcId.assign(imgProcId);
    myImgSourceId.assign(imgSrcId),
    
    m_edge = cvCreateMat(3, 3, CV_8SC1);
    cvSet2D(m_edge, 0, 0, cvRealScalar(0));
    cvSet2D(m_edge, 0, 1, cvRealScalar(-1));
    cvSet2D(m_edge, 0, 2, cvRealScalar(0));
    cvSet2D(m_edge, 1, 0, cvRealScalar(-1));
    cvSet2D(m_edge, 1, 1, cvRealScalar(4));
    cvSet2D(m_edge, 1, 2, cvRealScalar(-1));
    cvSet2D(m_edge, 2, 0, cvRealScalar(0));
    cvSet2D(m_edge, 2, 1, cvRealScalar(-1));
    cvSet2D(m_edge, 2, 2, cvRealScalar(0));

    m_mhi_memory = cvCreateMemStorage();

    CvSize s = cvSize(myWidth, myHeight);
    m_previous = 0;
    m_greyed = 0;
    m_calc = cvCreateImage(s, IPL_DEPTH_8U, 1);
    m_mhi = cvCreateImage(s, IPL_DEPTH_32F, 1);
    m_mhi_seg = cvCreateImage(s, IPL_DEPTH_32F, 1);
}

ImageRegionListRecord* MotionDetector::findMotions(IplImage* image) {
    // ProcessFrame
    // @note In one early run this method crashed with "Pure Virtual Function call" error calling WrapDataInIplImage()
    m_greyed = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
    cvConvertImage(image, m_greyed);
//    cvCvtColor(image, m_greyed, CV_RGB2GRAY);
    if (m_previous != 0) {
        cvAbsDiff(m_greyed, m_previous, m_calc);
        cvThreshold(m_calc, m_calc, 15, 255, CV_THRESH_BINARY);
        // Update the motion history image
        cvUpdateMotionHistory(m_calc, m_mhi, m_timestamp, timeduration);
        // reset the previous
        cvReleaseImage(&m_previous);
        m_previous = m_greyed;
        m_greyed = 0;
    } else {
        m_previous = m_greyed;
        m_greyed = 0;
    }
    ImageRegionListRecord* regions = new ImageRegionListRecord();
    regions->imageId = 0;
    regions->imageProcessorId.assign(myProcId);
    regions->imageRegionsId = m_timestamp;
    regions->imageSourceId.assign(myImgSourceId);
    regions->imageTimestampMillisecUTC = 0;
    regions->processorStartTimestampMillisecUTC = 0;
    regions->processorCompleteTimestampMillisecUTC = 0;
    if (!m_first) {
        IplImage* eroded_mhi = cvCloneImage(m_mhi);
        cvErode(eroded_mhi, eroded_mhi);
        CvSeq* motion_seg = cvSegmentMotion(eroded_mhi, m_mhi_seg, m_mhi_memory, m_timestamp, 5);
        cvReleaseImage(&eroded_mhi);
        schar* t = 0;
        int idx = 0;
        t = cvGetSeqElem(motion_seg, idx);
        while (t != 0) {
            CvRect r = ((CvConnectedComp*) t)->rect;
            ImageRegionRecord ir;
            ir.x = r.x;
            ir.y = r.y;
            ir.height = r.height;
            ir.width = r.width;
            ir.regionId = 0;
            regions->regions.push_back(ir);
            idx++;
            t = cvGetSeqElem(motion_seg, idx);
        }
        cvClearSeq(motion_seg);
        cvClearMemStorage(m_mhi_memory);
    } else {
        m_first = false;
    }
    m_timestamp++;
    return regions;
}