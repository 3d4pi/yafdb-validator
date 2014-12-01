#ifndef YMLREADER_H
#define YMLREADER_H

#include <opencv2/core/core.hpp>
#include "detectedobject.h"
#include <QString>
#include <QList>

class YMLReader
{
public:
    YMLReader();

    QList<DetectedObject> loadYML(QString path);
};

#endif // YMLREADER_H
