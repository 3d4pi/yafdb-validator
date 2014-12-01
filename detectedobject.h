#ifndef DETECTEDOBJECT_H
#define DETECTEDOBJECT_H

#include <QString>
#include <QPixmap>

class DetectedObject
{
public:
    DetectedObject();
    DetectedObject(QString objectType, QString autoStatus, QString manualStatus,
                   bool blurred, bool valid, QPointF point_1, QPointF point_2,
                   float azimuth, float elevation, float aperture);

    QString objectType;
    QString autoStatus;
    QString manualStatus;

    bool blurred;
    bool valid;

    QPointF point_1;
    QPointF point_2;

    float azimuth;
    float elevation;
    float aperture;

    QPixmap tile;

};

#endif // DETECTEDOBJECT_H
