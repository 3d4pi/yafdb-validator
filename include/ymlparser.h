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
