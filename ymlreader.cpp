#include "ymlreader.h"
#include <QDebug>

YMLReader::YMLReader()
{
}

QList<DetectedObject> YMLReader::loadYML(QString path)
{

    // Init output list
    QList<DetectedObject> out_list;

    // Read YML file
    cv::FileStorage fs(path.toStdString(), cv::FileStorage::READ);

    // Retrieve objects node
    cv::FileNode objectsNode = fs["objects"];

    // Iterate over objects
    for (cv::FileNodeIterator it = objectsNode.begin(); it != objectsNode.end(); ++it) {

        // Initialize detected object
        DetectedObject object;

        // Parse class name
        std::string clasName;
        (*it)["className"] >> clasName;
        object.className = clasName.c_str();

        // Parse area points
        cv::FileNode areaNode = (*it)["area"];
        cv::Point2d pt_1;
        cv::Point2d pt_2;
        areaNode["p1"] >> pt_1;
        areaNode["p2"] >> pt_2;
        object.point_1 = QPointF(pt_1.x, pt_1.y);
        object.point_2 = QPointF(pt_2.x, pt_2.y);

        // Parse false positive status
        std::string falsePositive;
        (*it)["falsePositive"] >> falsePositive;
        object.falsePositive = QString(falsePositive.c_str());

        // Parse auto status
        std::string autoStatus;
        (*it)["autoStatus"] >> autoStatus;
        object.autoStatus = QString(autoStatus.c_str());

        // Parse manual status
        std::string manualStatus;
        (*it)["manualStatus"] >> manualStatus;
        object.manualStatus = QString(manualStatus.c_str());

        // Append to list
        out_list.append(object);

    }

    // Return results
    return out_list;
}
