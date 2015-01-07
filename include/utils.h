#ifndef UTILS_H
#define UTILS_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QImage>
#include <inter-all.h>
#include <gnomonic-all.h>
#include "objectrect.h"

struct image_info_struct{
    QImage* image;
    int width;
    int height;
    int channels;
};

QImage*  IplImage2QImage(IplImage *iplImg);
void exportRect(ObjectRect* rect, image_info_struct image_info, QString destination, float zoom_level = 1.5);

#endif // UTILS_H
