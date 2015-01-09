#ifndef YMLREADER_H
#define YMLREADER_H

#include <opencv2/core/core.hpp>
#include "detectedobject.h"
#include "objectrect.h"
#include <QString>
#include <QList>

struct YMLType
{
    enum Type
    {
        Detector = 1, Validator = 2
    };
};

class YMLParser
{
public:
    YMLParser();

    void writeYML(QList<ObjectRect*> objects, QString path);
    QList<ObjectRect*> loadYML(QString path, int ymltype = YMLType::Validator);

private:
    void writeItem(cv::FileStorage &fs, ObjectRect* obj);
    ObjectRect* readItem(cv::FileNodeIterator iterator, int ymltype = YMLType::Validator, int parent_count = 0);
};

#endif // YMLREADER_H
