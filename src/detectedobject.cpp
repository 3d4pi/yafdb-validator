#include "detectedobject.h"

DetectedObject::DetectedObject()
{
    this->autoStatus = "None";
    this->manualStatus = "None";

    this->area.p1 = QPointF(0.0, 0.0);
    this->area.p2 = QPointF(0.0, 0.0);
    this->area.system = 0;

    this->gnomonic.width = 0;
    this->gnomonic.aperture_x = 0.0;
    this->gnomonic.aperture_y = 0.0;

    this->params.azimuth = 0.0;
    this->params.elevation = 0.0;
    this->params.aperture = 0.0;
}
