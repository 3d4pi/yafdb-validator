#include "ymlparser.h"
#include <QDebug>

YMLParser::YMLParser()
{
}

DetectedObject YMLParser::toDetectedObject(cv::FileNodeIterator iterator)
{
    // Initialize detected object
    DetectedObject object;

    // Parse class name
    std::string clasName;
    (*iterator)["className"] >> clasName;
    object.className = clasName.c_str();

    // Parse area points
    cv::FileNode areaNode = (*iterator)["area"];
    cv::Point2d pt_1;
    cv::Point2d pt_2;
    areaNode["p1"] >> pt_1;
    areaNode["p2"] >> pt_2;
    areaNode["system"] >> object.area.system;
    object.area.p1 = QPointF(pt_1.x, pt_1.y);
    object.area.p2 = QPointF(pt_2.x, pt_2.y);

    // Parse false positive status
    std::string falsePositive;
    (*iterator)["falsePositive"] >> falsePositive;
    object.falsePositive = QString(falsePositive.c_str());

    // Parse auto status
    std::string autoStatus;
    (*iterator)["autoStatus"] >> autoStatus;
    object.autoStatus = QString(autoStatus.c_str());

    // Parse manual status
    std::string manualStatus;
    (*iterator)["manualStatus"] >> manualStatus;
    object.manualStatus = QString(manualStatus.c_str());

    // Return object
    return object;
}

QList<DetectedObject> YMLParser::loadYML(QString path)
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
        DetectedObject object = this->toDetectedObject(it);

        // Parse algorithm
        std::string algorithm;
        fs["algorithm"] >> algorithm;
        object.algorithm = algorithm.c_str();

        // Parse gnomonic parameters
        fs["gnomonic"]["width"] >> object.gnomonic.width;
        fs["gnomonic"]["aperture_x"] >> object.gnomonic.aperture_x;
        fs["gnomonic"]["aperture_y"] >> object.gnomonic.aperture_y;

        // Parse source file name
        std::string source;
        fs["source"] >> source;
        object.source = source.c_str();

        // Parse childrens
        cv::FileNode childNode = (*it)["children"];
        for (cv::FileNodeIterator child = childNode.begin(); child != childNode.end(); ++child) {
            object.childrens.append(this->toDetectedObject(child));
        }

        // Append to list
        out_list.append(object);

    }

    // Return results
    return out_list;
}

void YMLParser::writeItem(cv::FileStorage &fs, DetectedObject obj)
{
    // Write clann name
    fs << "className" << obj.className.toStdString();

    // Write square area coodinates
    fs << "area" << "{";
        fs << "system" << obj.area.system;
        fs << "p1" << cv::Point2d(obj.area.p1.x(), obj.area.p1.y());
        fs << "p2" << cv::Point2d(obj.area.p2.x(), obj.area.p2.y());
    fs << "}";

    // Write status tags
    fs << "falsePositive" << obj.falsePositive.toStdString();
    fs << "manualStatus" << obj.manualStatus.toStdString();
    fs << "autoStatus" << obj.autoStatus.toStdString();
}

void YMLParser::writeYML(QList<DetectedObject> objects, QString path)
{
    // Open storage for writing
    cv::FileStorage fs(path.toStdString(), cv::FileStorage::WRITE);

    // Write algorithm
    fs << "algorithm" << objects.first().algorithm.toStdString();

    // Write gnomonic parameters
    fs << "gnomonic" << "{";
        fs << "width" << objects.first().gnomonic.width;
        fs << "aperture_x" << objects.first().gnomonic.aperture_x;
        fs << "width" << objects.first().gnomonic.aperture_y;
    fs << "}";

    // Write source file path
    fs << "source" << objects.first().source.toStdString();

    // Write objects
    fs << "objects" << "[";

        // Iterate over objects
        foreach (DetectedObject obj, objects) {

            // Open array element
            fs << "{";

                // Object writer method
                this->writeItem(fs, obj);

                // Write childrens if present
                if(obj.childrens.length() > 0)
                {
                    fs << "children" << "[";
                        foreach (DetectedObject child, obj.childrens) {
                            fs << "{";
                                this->writeItem(fs, child);
                            fs << "}";
                        }
                    fs << "]";
                }

            // Close array element
            fs << "}";
        }

    // Close array
    fs << "]";
}
