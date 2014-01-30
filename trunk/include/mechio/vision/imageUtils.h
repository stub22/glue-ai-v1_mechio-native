/* 
 * File:   imageUtils.h
 * Author: matt
 *
 * Created on June 29, 2011, 11:19 PM
 */

#ifndef IMAGEUTILS_H
#define	IMAGEUTILS_H

#include <opencv/cv.h>
#include "mechio/vision/protocol/ImageRecord.h"

using namespace std;
using namespace avro;

IplImage* unpackImage(ImageRecord* pimg, uint8_t** buff);
ImageRecord* packImage(IplImage *img);

void showImage(IplImage *img);

void saveToFile(ImageRecord* pimg, const char* filename);
ImageRecord* loadFromFile(const char* filename);

#endif	/* IMAGEUTILS_H */

