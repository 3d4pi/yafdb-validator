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

/* Includes */
#include "ymlparser.h"

/* Constructor */
YMLParser::YMLParser()
{
}

/* Function to write ObjectRect list to YML file on disk */
void YMLParser::writeYML(QList<ObjectRect*> objects, QString path)
{
    /* Open storage for writing */
    cv::FileStorage fs(path.toStdString(), cv::FileStorage::WRITE);

    /* Write source file path */
    fs << "source_image" << objects.first()->getSourceImagePath().toStdString();

    /* Write objects */
    fs << "objects" << "[";

    /* Iterate over objects */
    foreach (ObjectRect* obj, objects) {

        /* Open array element */
        fs << "{";

        /* Write object */
        this->writeItem(fs, obj);

        /* Write childrens if present */
        if(obj->childrens.length() > 0)
        {
            fs << "childrens" << "[";
            foreach (ObjectRect* child, obj->childrens) {
                fs << "{";
                    this->writeItem(fs, child);
                fs << "}";
            }
            fs << "]";
        }

        /* Close array element */
        fs << "}";
    }

    /* Close array */
    fs << "]";
}

/* Function load ObjectRect list from YML file on disk */
QList<ObjectRect*> YMLParser::loadYML(QString path, int ymltype)
{
    /* Init output list */
    QList<ObjectRect*> out_list;

    /* Read YML file */
    cv::FileStorage fs(path.toStdString(), cv::FileStorage::READ);

    /* Retrieve objects node */
    cv::FileNode objectsNode = fs["objects"];

    /* Retrieve invalid objects node */
    cv::FileNode invalidObjectsNode = fs["invalidObjects"];

    /* Iterate over objects */
    for (cv::FileNodeIterator it = objectsNode.begin(); it != objectsNode.end(); ++it) {

        /* Initialize detected object */
        ObjectRect* object = this->readItem(it, ymltype);

        /* Assign source image path */
        std::string source_image;
        fs["source_image"] >> source_image;
        object->setSourceImagePath( QString(source_image.c_str()) );

        /* Append to list */
        out_list.append(object);
    }

    /* Iterate over objects */
    for (cv::FileNodeIterator it = invalidObjectsNode.begin(); it != invalidObjectsNode.end(); ++it) {

        /* Initialize detected object */
        ObjectRect* object = this->readItem(it, ymltype);

        /* Assign object automatic state / status */
        object->setObjectAutomaticState( ObjectAutomaticState::Invalid );
        object->setAutomaticStatus( (object->getAutomaticStatus().length() <= 0 || object->getAutomaticStatus() == "None") ? "MissingOption" : object->getAutomaticStatus() );

        /* Assign source image path */
        std::string source_image;
        fs["source_image"] >> source_image;
        object->setSourceImagePath( QString(source_image.c_str()) );

        /* Append to list */
        out_list.append(object);
    }

    /* Return results */
    return out_list;
}

/* Function to write specific ObjectRect into YML file */
void YMLParser::writeItem(cv::FileStorage &fs, ObjectRect* obj)
{
    /* Write type */
    switch(obj->getObjectType())
    {
    case ObjectType::Face:
        fs << "className" << "Face";
        break;
    case ObjectType::NumberPlate:
        fs << "className" << "NumberPlate";
        break;
    case ObjectType::ToBlur:
        fs << "className" << "ToBlur";
        break;
    case ObjectType::None:
        fs << "className" << "None";
        break;
    }

    /* Write sub-type */
    switch(obj->getObjectSubType())
    {
    case ObjectSubType::None:
        fs << "subClassName" << "None";
        break;
    case ObjectSubType::Front:
        fs << "subClassName" << "Front";
        break;
    case ObjectSubType::Profile:
        fs << "subClassName" << "Profile";
        break;
    case ObjectSubType::Back:
        fs << "subClassName" << "Back";
        break;
    case ObjectSubType::Top:
        fs << "subClassName" << "Top";
        break;
    case ObjectSubType::Eyes:
        fs << "subClassName" << "Eyes";
        break;
    }

    /* Write square area coodinates */
    fs << "area" << "{";
        fs << "p1" << cv::Point2d(obj->proj_point_1().x(), obj->proj_point_1().y());
        fs << "p2" << cv::Point2d(obj->proj_point_2().x(), obj->proj_point_2().y());
        fs << "p3" << cv::Point2d(obj->proj_point_3().x(), obj->proj_point_3().y());
        fs << "p4" << cv::Point2d(obj->proj_point_4().x(), obj->proj_point_4().y());
    fs << "}";

    /* Write projection parameters */
    fs << "params" << "{";
        fs << "azimuth" << obj->proj_azimuth();
        fs << "elevation" << obj->proj_elevation();
        fs << "aperture" << obj->proj_aperture();
        fs << "width" << obj->proj_width();
        fs << "height" << obj->proj_height();
    fs << "}";

    /* Write status tags */
    fs << "autoStatus" << obj->getAutomaticStatus().toStdString();
    fs << "manualStatus" << obj->getManualStatus().toStdString();
    fs << "blurObject" << (obj->isBlurred() ? "Yes" : "No");
}

/* Function to read specific ObjectRect from YML file */
ObjectRect* YMLParser::readItem(cv::FileNodeIterator iterator, int ymltype)
{
    /* Initialize detected object */
    ObjectRect* object = new ObjectRect;

    /* Parse falsePositive tag */
    std::string falsePositive;
    (*iterator)["falsePositive"] >> falsePositive;
    QString lowerFalsePositive = QString( falsePositive.c_str() ).toLower();

    /* Parse class name */
    std::string className;
    (*iterator)["className"] >> className;

    /* Parse sub class name */
    std::string subClassName;
    (*iterator)["subClassName"] >> subClassName;

    /* Convert values to lower case */
    QString lowerClassName = QString( className.c_str() ).toLower();
    QString lowerSubClassName = QString( subClassName.c_str() ).toLower();

    /* Assign object type */
    if(lowerClassName == "face")
    {
        object->setObjectType( ObjectType::Face );
    } else if (lowerClassName == "front" || lowerClassName == "front:profile"){
        object->setObjectType( ObjectType::Face );
        object->setObjectSubType( ObjectSubType::Front );
    } else if (lowerClassName == "profile"){
        object->setObjectType( ObjectType::Face );
        object->setObjectSubType( ObjectSubType::Profile );
    } else if(lowerClassName == "numberplate") {
        object->setObjectType( ObjectType::NumberPlate );
    } else if(lowerClassName == "toblur") {
        object->setObjectType( ObjectType::ToBlur );
    } else if(lowerClassName == "none") {
        object->setObjectType( ObjectType::None );
    }

    /* Assign object sub-type */
    if(lowerSubClassName == "none")
    {
        object->setObjectSubType( ObjectSubType::None );
    } else if(lowerSubClassName == "front") {
        object->setObjectSubType( ObjectSubType::Front );
    } else if(lowerSubClassName == "profile") {
        object->setObjectSubType( ObjectSubType::Profile );
    } else if(lowerSubClassName == "back") {
        object->setObjectSubType( ObjectSubType::Back );
    } else if(lowerSubClassName == "top") {
        object->setObjectSubType( ObjectSubType::Top );
    } else if(lowerSubClassName == "eyes") {
        object->setObjectSubType( ObjectSubType::Eyes );
    }

    /* Parse area points */
    cv::FileNode areaNode = (*iterator)["area"];
    cv::Point2d pt_1;
    cv::Point2d pt_2;
    cv::Point2d pt_3;
    cv::Point2d pt_4;

    /* Read coordinates */
    switch(ymltype)
    {
    case YMLType::Detector:

        /* Parse square edges points */
        areaNode["p1"] >> pt_1;
        areaNode["p2"] >> pt_3;
        break;
    case YMLType::Validator:

        /* Parse polygon points */
        areaNode["p1"] >> pt_1;
        areaNode["p2"] >> pt_2;
        areaNode["p3"] >> pt_3;
        areaNode["p4"] >> pt_4;
        break;
    }

    /* Set object coordinates */
    object->setPoints(QPointF(pt_1.x, pt_1.y),
                      QPointF(pt_2.x, pt_2.y),
                      QPointF(pt_3.x, pt_3.y),
                      QPointF(pt_4.x, pt_4.y));

    /* Parse gnomonic parameters */
    cv::FileNode paramsNode = (*iterator)["params"];
    float azimuth = 0.0;
    float elevation = 0.0;
    float aperture = 0.0;
    float width = 0.0;
    float height = 0.0;

    paramsNode["azimuth"] >> azimuth;
    paramsNode["elevation"] >> elevation;
    paramsNode["aperture"] >> aperture;
    paramsNode["width"] >> width;
    paramsNode["height"] >> height;

    /* Set object projection parameters */
    object->setProjectionParametters(azimuth,
                                    elevation,
                                    aperture,
                                    width,
                                    height);

    /* Set object projection points */
    object->setProjectionPoints();

    /* Parse auto status */
    std::string autoStatus;
    (*iterator)["autoStatus"] >> autoStatus;

    /* Convert values to lower case */
    QString lowerAutoStatus = QString( autoStatus.c_str() ).toLower();
    lowerAutoStatus = lowerAutoStatus.length() > 0 ? lowerAutoStatus : "none";

    /* Check presence of auto status */
    if(lowerAutoStatus != "none")
    {
        /* Check if auto status is valid */
        if(lowerAutoStatus == "valid")
        {
            /* Set values */
            object->setObjectAutomaticState( ObjectAutomaticState::Valid );
            object->setAutomaticStatus( "Valid" );

            /* Tag object for blurring */
            if( ymltype == YMLType::Detector )
            object->setBlurred( true );

        } else {

            /* Set automatic state as invalid */
            object->setObjectAutomaticState( ObjectAutomaticState::Invalid );

            /* Restore automatic status */
            switch(ymltype)
            {
            case YMLType::Detector:

                /* Assign proper automatic filtering flag */
                if( lowerAutoStatus == "filtered-ratio" )
                {
                   object->setAutomaticStatus( "Ratio" );
                } else if( lowerAutoStatus == "filtered-size" ){
                   object->setAutomaticStatus( "Size" );
                } else if( lowerAutoStatus == "filtered-ratio-size" ){
                    object->setAutomaticStatus( "Ratio-Size" );
                }
                break;
            case YMLType::Validator:

                /* Assign proper automatic filtering flag */
                if( lowerAutoStatus == "ratio" )
                {
                   object->setAutomaticStatus( "Ratio" );
                } else if( lowerAutoStatus == "size" ){
                   object->setAutomaticStatus( "Size" );
                } else if( lowerAutoStatus == "ratio-size" ){
                    object->setAutomaticStatus( "Ratio-Size" );
                } else if( lowerAutoStatus == "missingoption" ){
                    object->setAutomaticStatus( "MissingOption" );
                }
                break;
            }
        }
    } else {
        object->setObjectAutomaticState( ObjectAutomaticState::Manual );
    }

    /* Set automatic status to none if null */
    object->setAutomaticStatus( (object->getAutomaticStatus().length() > 0 ? object->getAutomaticStatus() : "None") );

    /* Parse manual status */
    std::string manualStatus;
    (*iterator)["manualStatus"] >> manualStatus;
    object->setManualStatus( QString(manualStatus.c_str()) );
    object->setManualStatus( (object->getManualStatus().length() > 0 ? object->getManualStatus() : "None") );

    /* Restore manual status tag */
    /* Check if object is tagged as falsePositive */
    if(lowerFalsePositive == "no")
    {
        /* CHeck if object is manual */
        if(lowerAutoStatus == "none")
        {
            object->setManualStatus("Valid");
        }
    } else if( lowerFalsePositive == "yes" )
    {
        object->setManualStatus("Invalid");
    }

    /* Restore manual state */
    /* Check if object is a "ToBlur" element */
    if(object->getObjectType() == ObjectType::ToBlur)
    {
        object->setObjectManualState( ObjectManualState::ToBlur );
    } else {

        /* Check if object is manual */
        if(object->getManualStatus() != "None")
        {
            /* CHeck if object manual state is valid */
            if(object->getManualStatus() == "Valid")
            {
                object->setObjectManualState( ObjectManualState::Valid );
            } else {
                object->setObjectManualState( ObjectManualState::Invalid );
            }
        } else {
            object->setObjectManualState( ObjectManualState::None );
        }

    }

    /* Restore blur tag */
    if( ymltype == YMLType::Validator )
    {
        /* Read blur tag */
        std::string blurObject;
        (*iterator)["blurObject"] >> blurObject;
        QString blurObject_lower = QString( blurObject.c_str() ).toLower();

        /* Set blur tag */
        object->setBlurred( blurObject_lower == "yes" ? true : false );
    }

    /* Load childrens */
    cv::FileNode childNode = (*iterator)["childrens"];
    for (cv::FileNodeIterator child = childNode.begin(); child != childNode.end(); ++child) {
        object->childrens.append( this->readItem( child ) );
    }

    /* Disable object resizing */
    object->setResizeEnabled( false );

    /* Return object */
    return object;
}
