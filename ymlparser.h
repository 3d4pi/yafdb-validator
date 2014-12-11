#ifndef YMLREADER_H
#define YMLREADER_H

#include <opencv2/core/core.hpp>
#include "detectedobject.h"
#include "objectrect.h"
#include <QString>
#include <QList>

class YMLParser
{
public:
    YMLParser();

    void writeYML(QList<ObjectRect*> objects, QString path);
    QList<ObjectRect*> loadYML(QString path);

private:
    void writeItem(cv::FileStorage &fs, ObjectRect* obj);
    ObjectRect* readItem(cv::FileNodeIterator iterator);
};

#endif // YMLREADER_H
