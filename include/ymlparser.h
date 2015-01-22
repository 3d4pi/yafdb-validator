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

#ifndef YMLREADER_H
#define YMLREADER_H

/* Includes */
#include <opencv2/core/core.hpp>
#include "objectrect.h"
#include <QString>
#include <QList>

/* YML type structure */
struct YMLType
{
    enum Type
    {
        Detector  = 1, /* YML Comming form yafdb-detect */
        Validator = 2  /* YML Comming form yafdb-validate (this software) */
    };
};

/* Main class */
class YMLParser
{

/* Public functions / variables */
public:

    /* Constructor */
    YMLParser();

    /* Function to write ObjectRect list to YML file on disk */
    void writeYML(QList<ObjectRect*> objects, QString path);

    /* Function load ObjectRect list from YML file on disk */
    QList<ObjectRect*> loadYML(QString path, int ymltype = YMLType::Validator);

/* Private functions / variables */
private:

    /* Function to write specific ObjectRect into YML file */
    void writeItem(cv::FileStorage &fs, ObjectRect* obj);

    /* Function to read specific ObjectRect from YML file */
    ObjectRect* readItem(cv::FileNodeIterator iterator, int ymltype = YMLType::Validator);
};

#endif // YMLREADER_H
