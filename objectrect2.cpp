#include "objectrect2.h"

ObjectRect2::ObjectRect2()
{
    // Append default points
    this->points.append(QPointF(0.0, 0.0));
    this->points.append(QPointF(0.0, 0.0));
    this->points.append(QPointF(0.0, 0.0));
    this->points.append(QPointF(0.0, 0.0));
    this->polygon = QPolygonF( this->points );
    this->setPolygon( this->polygon );

    // Default id
    this->id = 0;

    // Default pen setup
    this->pen = new QPen(QColor(0, 255, 255, 255), 2);
    this->brush = new QBrush(QColor(0, 255, 0, 50), Qt::SolidPattern);

    // Default projection parameters
    this->projection_parameters.azimuth = 0.0;
    this->projection_parameters.elevation = 0.0;
    this->projection_parameters.aperture = 0.0;

    this->projection_parameters.points.append(QPointF(0.0, 0.0));
    this->projection_parameters.points.append(QPointF(0.0, 0.0));
    this->projection_parameters.points.append(QPointF(0.0, 0.0));
    this->projection_parameters.points.append(QPointF(0.0, 0.0));

    this->projection_parameters.width = 0.0;
    this->projection_parameters.height = 0.0;

    // Default informations
    this->info.automatic_status = "None";
    this->info.manual_status = "None";
    this->info.blurred = false;
    this->info.validated = false;
    this->info.type = ObjectType::None;
}

void ObjectRect2::setPoint1(QPointF point)
{
    this->points[0] = point;

    // Cal rendering procedure
    this->render();
}

void ObjectRect2::setPoint2(QPointF point)
{
    this->points[1] = point;

    // Cal rendering procedure
    this->render();
}

void ObjectRect2::setPoint3(QPointF point)
{
    this->points[2] = point;

    // Cal rendering procedure
    this->render();
}

void ObjectRect2::setPoint3_Rigid(QPointF point)
{
    QPointF new_p1 = this->points[0];
    QPointF new_p2 = this->points[1];
    QPointF new_p3 = point;
    QPointF new_p4 = this->points[3];

    new_p2.setY( new_p3.y() );
    new_p4.setX( new_p3.x() );

    // Update positions
    this->setPoints( new_p1, new_p2, new_p3, new_p4 );

    // Cal rendering procedure
    this->render();
}

void ObjectRect2::setPoint3_Rigid(QPointF point, QPointF offset)
{

    QPointF centered (
                    point.x() - offset.x(),
                    point.y() - offset.y()
                );

    this->setPoint3_Rigid(centered);
}

void ObjectRect2::setPoint4(QPointF point)
{
    this->points[3] = point;

    // Cal rendering procedure
    this->render();
}

void ObjectRect2::setPoints(QPointF p1, QPointF p2, QPointF p3, QPointF p4)
{
    this->setPoint1( p1 );
    this->setPoint2( p2 );
    this->setPoint3( p3 );
    this->setPoint4( p4 );

    // Cal rendering procedure
    this->render();
}

void ObjectRect2::moveObject(QPointF pos,
                             QPointF offset_1,
                             QPointF offset_2,
                             QPointF offset_3,
                             QPointF offset_4)
{

    // Center point by clicking offset
    QPointF centered_point_1 (
                    pos.x() - offset_1.x(),
                    pos.y() - offset_1.y()
                );

    QPointF centered_point_2 (
                    pos.x() - offset_2.x(),
                    pos.y() - offset_2.y()
                );

    QPointF centered_point_3 (
                    pos.x() - offset_3.x(),
                    pos.y() - offset_3.y()
                );

    QPointF centered_point_4 (
                    pos.x() - offset_4.x(),
                    pos.y() - offset_4.y()
                );

    // Move object point 1
    this->setPoint1(centered_point_1);
    this->setPoint2(centered_point_2);
    this->setPoint3(centered_point_3);
    this->setPoint4(centered_point_4);

    this->render();
}

QPointF ObjectRect2::getPoint1()
{
    return this->points[0];
}

QPointF ObjectRect2::getPoint2()
{
    return this->points[1];
}

QPointF ObjectRect2::getPoint3()
{
    return this->points[2];
}

QPointF ObjectRect2::getPoint4()
{
    return this->points[3];
}

QVector<QPointF> ObjectRect2::getPoints()
{
    return this->points;
}

void ObjectRect2::setSize(QSizeF size)
{
    QPointF new_p1 = this->points[0];
    QPointF new_p2 = this->points[1];
    QPointF new_p3 = this->points[2];
    QPointF new_p4 = this->points[3];

    // Width
    new_p2.setX( new_p1.x() + size.width() );
    new_p3.setX( new_p1.x() + size.width() );

    // Height
    new_p4.setY( new_p1.y() + size.height() );
    new_p3.setY( new_p1.y() + size.height() );

    // Update positions
    this->setPoints( new_p1, new_p2, new_p3, new_p4 );

    // Cal rendering procedure
    this->render();
}

QSizeF ObjectRect2::getSize()
{
    QPointF p1 = this->points[0];
    QPointF p2 = this->points[1];
    QPointF p4 = this->points[3];

    QSizeF size;

    size.setWidth( p4.x() -  p1.x());
    size.setHeight( p2.y() -  p1.y());

    return size;
}

void ObjectRect2::setId(int id)
{
    this->id = id;
}

int ObjectRect2::getId()
{
    return this->id;
}

void ObjectRect2::setObjectRectType(int type)
{
    switch(type)
    {
        case ObjectRectType::Manual:
            this->pen->setColor( QColor(0, 255, 255, 255) );
            break;
        case ObjectRectType::Valid:
            this->pen->setColor( QColor(0, 255, 0, 255) );
            break;
        case ObjectRectType::Invalid:
            this->pen->setColor( QColor(255, 0, 0, 255) );
            break;
    }

    // Cal rendering procedure
    this->render();
}

void ObjectRect2::setObjectRectState(int state)
{
    switch(state)
    {
        case ObjectRectState::Valid:
            this->brush->setColor( QColor(0, 255, 0, 50) );
            break;
        case ObjectRectState::Invalid:
            this->brush->setColor( QColor(255, 0, 0, 50) );
            break;
    }

    // Cal rendering procedure
    this->render();
}

void ObjectRect2::render()
{
    this->polygon = QPolygonF( this->points );

    this->setPen( * this->pen );
    this->setBrush( * this->brush );
    this->setPolygon( this->polygon );
}

void ObjectRect2::setProjectionParametters(float azimuth,
                                           float elevation,
                                           float aperture,
                                           float width,
                                           float height)
{
    this->projection_parameters.azimuth = azimuth;
    this->projection_parameters.elevation = elevation;
    this->projection_parameters.aperture = aperture;
    this->projection_parameters.width = width;
    this->projection_parameters.height = height;
}

void ObjectRect2::setProjectionPoints()
{
    this->projection_parameters.points = this->points;
}

float ObjectRect2::proj_azimuth()
{
    return this->projection_parameters.azimuth;
}

float ObjectRect2::proj_elevation()
{
    return this->projection_parameters.elevation;
}

float ObjectRect2::proj_aperture()
{
    return this->projection_parameters.aperture;
}

QPointF ObjectRect2::proj_point_1()
{
    return this->projection_parameters.points[0];
}

QPointF ObjectRect2::proj_point_2()
{
    return this->projection_parameters.points[1];
}

QPointF ObjectRect2::proj_point_3()
{
    return this->projection_parameters.points[2];
}

QPointF ObjectRect2::proj_point_4()
{
    return this->projection_parameters.points[3];
}

float ObjectRect2::proj_width()
{
    return this->projection_parameters.width;
}

float ObjectRect2::proj_height()
{
    return this->projection_parameters.height;
}

int ObjectRect2::getType()
{
    return this->info.type;
}

void ObjectRect2::setType(int value)
{
    this->info.type = value;
}

bool ObjectRect2::isBlurred()
{
    return this->info.blurred;
}

bool ObjectRect2::isValidated()
{
    return this->info.validated;
}

void ObjectRect2::setBlurred(bool value)
{
    this->info.blurred = value;
}

void ObjectRect2::setValidated(bool value)
{
    this->info.validated = value;
}

QString ObjectRect2::getManualStatus()
{
    return this->info.manual_status;
}

QString ObjectRect2::getAutomaticStatus()
{
    return this->info.automatic_status;
}

void ObjectRect2::setManualStatus(QString value)
{
    this->info.manual_status = value;
}

void ObjectRect2::setAutomaticStatus(QString value)
{
    this->info.automatic_status = value;
}
