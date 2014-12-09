#ifndef YMLREADER_H
#define YMLREADER_H

#include <opencv2/core/core.hpp>
#include "detectedobject.h"
#include <QString>
#include <QList>

class YMLParser
{
public:
    YMLParser();

    QList<DetectedObject> loadYML(QString path);
    void writeYML(QList<DetectedObject> objects, QString path);

    //DetectedObject ObjectRectToDetectedObject(ObjectRect* obj);

 private:
    DetectedObject toDetectedObject(cv::FileNodeIterator iterator);
    void writeItem(cv::FileStorage &fs, DetectedObject obj);
};

#endif // YMLREADER_H
