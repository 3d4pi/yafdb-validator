#ifndef UTILS_H
#define UTILS_H

/* Includes */
#include <QImage>
#include <QThread>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <inter-all.h>
#include <gnomonic-all.h>
#include "objectrect.h"

/* Image info structure */
struct image_info_struct{
    QImage* image;
    int width;
    int height;
    int channels;
};

/* Function to convert an OpenCV IplImage into a QImage */
QImage*  IplImage2QImage(IplImage *iplImg);

/* Function to export an object to disk */
void exportRect(ObjectRect* rect, image_info_struct image_info, QString destination, float zoom_level = 1.5);

/* Function to clamp a specified value */
float clamp(float x, float a, float b);

/* Function to clamp a specified radian value */
float clampRad(float x, float a, float b);

#endif // UTILS_H
