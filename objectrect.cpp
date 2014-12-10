#include "objectrect.h"

ObjectRect::ObjectRect()
{
    // Append default points
    this->points.append(QPointF(0.0, 0.0));
    this->points.append(QPointF(0.0, 0.0));
    this->points.append(QPointF(0.0, 0.0));
    this->points.append(QPointF(0.0, 0.0));
    this->polygon = QPolygonF( this->points );

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

    // Default contour setup
    QPen contour_pen(QColor(160, 32, 240, 255), 2);
    this->contour = new QGraphicsPolygonItem( this );
    this->contour->setPen( contour_pen );
    this->contour->setBrush( Qt::NoBrush );

    // Default resize rect setup
    this->resize_rect = new QGraphicsPolygonItem( this );
    this->resize_rect->setPen( *this->pen );
    this->resize_rect->setBrush( Qt::NoBrush );

    this->render();
}

void ObjectRect::setPoint1(QPointF point)
{
    this->points[0] = point;

    // Cal rendering procedure
    this->render();
}

void ObjectRect::setPoint2(QPointF point)
{
    this->points[1] = point;

    // Cal rendering procedure
    this->render();
}

void ObjectRect::setPoint3(QPointF point)
{
    this->points[2] = point;

    // Cal rendering procedure
    this->render();
}

void ObjectRect::setPoint3_Rigid(QPointF point)
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

void ObjectRect::setPoint3_Rigid(QPointF point, QPointF offset)
{

    QPointF centered (
                    point.x() - offset.x(),
                    point.y() - offset.y()
                );

    this->setPoint3_Rigid(centered);
}

void ObjectRect::setPoint4(QPointF point)
{
    this->points[3] = point;

    // Cal rendering procedure
    this->render();
}

void ObjectRect::setPoints(QPointF p1, QPointF p2, QPointF p3, QPointF p4)
{
    this->setPoint1( p1 );
    this->setPoint2( p2 );
    this->setPoint3( p3 );
    this->setPoint4( p4 );

    // Cal rendering procedure
    this->render();
}

void ObjectRect::moveObject(QPointF pos,
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

QPointF ObjectRect::getPoint1()
{
    return this->points[0];
}

QPointF ObjectRect::getPoint2()
{
    return this->points[1];
}

QPointF ObjectRect::getPoint3()
{
    return this->points[2];
}

QPointF ObjectRect::getPoint4()
{
    return this->points[3];
}

QVector<QPointF> ObjectRect::getPoints()
{
    return this->points;
}

void ObjectRect::setSize(QSizeF size)
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

QSizeF ObjectRect::getSize()
{
    QPointF p1 = this->points[0];
    QPointF p2 = this->points[1];
    QPointF p4 = this->points[3];

    QSizeF size;

    size.setWidth( p4.x() -  p1.x());
    size.setHeight( p2.y() -  p1.y());

    return size;
}

void ObjectRect::setId(int id)
{
    this->id = id;
}

int ObjectRect::getId()
{
    return this->id;
}

void ObjectRect::setObjectRectType(int type)
{
    this->rect_type = type;

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

    // Refresh pen
    this->setPen( * this->pen );
}

int ObjectRect::getObjectRectType()
{
    return this->rect_type;
}

void ObjectRect::setObjectRectState(int state)
{
    this->rect_state = state;

    switch(state)
    {
        case ObjectRectState::None:
            this->brush->setColor( QColor(0, 0, 0, 0) );
            break;
        case ObjectRectState::Valid:
            this->brush->setColor( QColor(0, 255, 0, 50) );
            break;
        case ObjectRectState::Invalid:
            this->brush->setColor( QColor(255, 0, 0, 50) );
            break;
    }

    // Render brush
    this->setBrush( * this->brush );
}

void ObjectRect::setObjectType(int type)
{
    this->type = type;
}

int ObjectRect::getObjectType()
{
    return this->type;
}

int ObjectRect::getObjectRectState()
{
    return this->rect_state;
}

void ObjectRect::render()
{
    this->polygon = QPolygonF( this->points );

    // Draw contour
    QVector<QPointF> contour_points;
    contour_points.append( QPointF(this->points[0].x() - 2, this->points[0].y() - 2) );
    contour_points.append( QPointF(this->points[1].x() - 2, this->points[1].y() + 2) );
    contour_points.append( QPointF(this->points[2].x() + 2, this->points[2].y() + 2) );
    contour_points.append( QPointF(this->points[3].x() + 2, this->points[3].y() - 2) );
    QPolygonF contour_polygon( contour_points );
    this->contour->setPolygon( contour_polygon );

    // Draw resize rect
    QVector<QPointF> resize_rect_points;
    resize_rect_points.append( QPointF(this->points[2].x() + 10, this->points[2].y() + 10) );
    resize_rect_points.append( QPointF(this->points[2].x() + 10, this->points[2].y()) );
    resize_rect_points.append( QPointF(this->points[2].x(),  this->points[2].y()) );
    resize_rect_points.append( QPointF(this->points[2].x(),  this->points[2].y() + 10) );
    this->resize_rect_polygon = QPolygonF( resize_rect_points );
    this->resize_rect->setPolygon( this->resize_rect_polygon );

    this->setPolygon( this->polygon );


}

void ObjectRect::setProjectionParametters(float azimuth,
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

void ObjectRect::setProjectionPoints()
{
    this->projection_parameters.points = this->points;
}

void ObjectRect::setProjectionPoints(QPointF p1, QPointF p2, QPointF p3, QPointF p4)
{
    this->projection_parameters.points[0] = p1;
    this->projection_parameters.points[1] = p2;
    this->projection_parameters.points[2] = p3;
    this->projection_parameters.points[3] = p4;
}

float ObjectRect::proj_azimuth()
{
    return this->projection_parameters.azimuth;
}

float ObjectRect::proj_elevation()
{
    return this->projection_parameters.elevation;
}

float ObjectRect::proj_aperture()
{
    return this->projection_parameters.aperture;
}

QPointF ObjectRect::proj_point_1()
{
    return this->projection_parameters.points[0];
}

QPointF ObjectRect::proj_point_2()
{
    return this->projection_parameters.points[1];
}

QPointF ObjectRect::proj_point_3()
{
    return this->projection_parameters.points[2];
}

QPointF ObjectRect::proj_point_4()
{
    return this->projection_parameters.points[3];
}

float ObjectRect::proj_width()
{
    return this->projection_parameters.width;
}

float ObjectRect::proj_height()
{
    return this->projection_parameters.height;
}

int ObjectRect::getType()
{
    return this->info.type;
}

void ObjectRect::setType(int value)
{
    this->info.type = value;
}

bool ObjectRect::isBlurred()
{
    return this->info.blurred;
}

bool ObjectRect::isValidated()
{
    return (this->rect_state == ObjectRectState::Valid);
}

void ObjectRect::setBlurred(bool value)
{
    this->info.blurred = value;
}

QString ObjectRect::getManualStatus()
{
    return this->info.manual_status;
}

QString ObjectRect::getAutomaticStatus()
{
    return this->info.automatic_status;
}

void ObjectRect::setManualStatus(QString value)
{
    this->info.manual_status = value;
}

void ObjectRect::setAutomaticStatus(QString value)
{
    this->info.automatic_status = value;
}

ObjectRect* ObjectRect::copy()
{
    ObjectRect* rect_copy = new ObjectRect();
    rect_copy->setObjectRectType( this->getObjectRectType() );
    rect_copy->setObjectRectState( this->getObjectRectState() );
    rect_copy->setManualStatus( this->getManualStatus() );
    rect_copy->setBlurred( this->isBlurred() );
    rect_copy->setId( this->getId() );

    rect_copy->setProjectionParametters(this->proj_azimuth(),
                                        this->proj_elevation(),
                                        this->proj_aperture(),
                                        this->proj_width(),
                                        this->proj_height());

    rect_copy->setPoints(this->proj_point_1(),
                         this->proj_point_2(),
                         this->proj_point_3(),
                         this->proj_point_4());

    rect_copy->setProjectionPoints();

    return rect_copy;
}

void ObjectRect::mapTo(float width, float height, float azimuth, float elevation, float aperture)
{
    double p1_x = 0.0;
    double p1_y = 0.0;
    double p2_x = 0.0;
    double p2_y = 0.0;
    double p3_x = 0.0;
    double p3_y = 0.0;
    double p4_x = 0.0;
    double p4_y = 0.0;

    g2g_point(this->proj_width(),
              this->proj_height(),
              this->proj_azimuth(),
              this->proj_elevation(),
              this->proj_aperture(),
              this->proj_point_1().x(),
              this->proj_point_1().y(),

              width,
              height,
              azimuth,
              elevation,
              aperture,
              &p1_x,
              &p1_y);

      g2g_point(this->proj_width(),
                this->proj_height(),
                this->proj_azimuth(),
                this->proj_elevation(),
                this->proj_aperture(),
                this->proj_point_2().x(),
                this->proj_point_2().y(),

                width,
                height,
                azimuth,
                elevation,
                aperture,
                &p2_x,
                &p2_y);

      g2g_point(this->proj_width(),
                this->proj_height(),
                this->proj_azimuth(),
                this->proj_elevation(),
                this->proj_aperture(),
                this->proj_point_3().x(),
                this->proj_point_3().y(),

                width,
                height,
                azimuth,
                elevation,
                aperture,
                &p3_x,
                &p3_y);

      g2g_point(this->proj_width(),
                this->proj_height(),
                this->proj_azimuth(),
                this->proj_elevation(),
                this->proj_aperture(),
                this->proj_point_4().x(),
                this->proj_point_4().y(),

                width,
                height,
                azimuth,
                elevation,
                aperture,
                &p4_x,
                &p4_y);

      this->setPoints(QPointF( p1_x, p1_y ),
                      QPointF( p2_x, p2_y ),
                      QPointF( p3_x, p3_y ),
                      QPointF( p4_x, p4_y ));

}

void ObjectRect::setResizeEnabled(bool value)
{
    this->resize_rect->setVisible( value );
}
