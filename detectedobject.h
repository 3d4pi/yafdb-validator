#ifndef DETECTEDOBJECT_H
#define DETECTEDOBJECT_H

#include <QString>
#include <QPixmap>

class DetectedObject
{
public:
    DetectedObject();

    QString algorithm;

    struct gnomonic_struct {
      int width;
      int aperture_x;
      int aperture_y;
    } gnomonic;

    struct params_struct {
      float azimuth;
      float elevation;
      float aperture;
    } params;

    QString source;

    QString className;

    struct area_struct {
      int system;
      QPointF p1;
      QPointF p2;
    } area;

    QString falsePositive;

    QString autoStatus;
    QString manualStatus;

    QList<DetectedObject> childrens;
};

#endif // DETECTEDOBJECT_H
