#include "detectedobject.h"

DetectedObject::DetectedObject()
{
}

DetectedObject::DetectedObject(QString objectType, QString autoStatus, QString manualStatus,
                               bool blurred, bool valid, QPointF point_1, QPointF point_2,
                               float azimuth, float elevation, float aperture
)
{
    this->objectType = objectType;
    this->autoStatus = autoStatus;
    this->manualStatus = manualStatus;

    this->blurred = blurred;
    this->valid = valid;

    this->point_1 = point_1;
    this->point_2 = point_2;

    this->azimuth = azimuth;
    this->elevation = elevation;
    this->aperture = aperture;
}
