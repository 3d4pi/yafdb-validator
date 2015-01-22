/*
 * yafdb-validate - Yafdb validation tool
 *
 * Copyright (c) 2014-2015 FOXEL SA - http://foxel.ch
 * Please read <http://foxel.ch/license> for more information.
 *
 *
 * Author(s):
 *
 *      Kevin Velickovic <k.velickovic@foxel.ch>
 *
 *
 * This file is part of the FOXEL project <http://foxel.ch>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Additional Terms:
 *
 *      You are required to preserve legal notices and author attributions in
 *      that material or in the Appropriate Legal Notices displayed by works
 *      containing it.
 *
 *      You are required to attribute the work as explained in the "Usage and
 *      Attribution" section of <http://foxel.ch/license>.
 */

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
