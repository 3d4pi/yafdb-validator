#include "detectedobject.h"

DetectedObject::DetectedObject()
{
}

DetectedObject::DetectedObject(QString objectType, QString autoStatus, QString manualStatus,
                               bool blurred, bool valid, float point_1[2], float point_2[2],
                               float azimuth, float elevation, float aperture
)
{
    this->objectType = objectType;
    this->autoStatus = autoStatus;
    this->manualStatus = manualStatus;

    this->blurred = blurred;
    this->valid = valid;

    this->point_1[0] = point_1[0];
    this->point_1[1] = point_1[1];
    this->point_2[0] = point_2[0];
    this->point_2[1] = point_2[1];

    this->azimuth = azimuth;
    this->elevation = elevation;
    this->aperture = aperture;
}
