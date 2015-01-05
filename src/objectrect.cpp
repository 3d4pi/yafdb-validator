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
    this->info.sub_type = ObjectSubType::None;

    // Default contour setup
    this->contour_pen = new QPen(QColor(255, 255, 255, 255), 2);
    this->contour = new QGraphicsPolygonItem( this );
    this->contour->setPen( *this->contour_pen );
    this->contour->setBrush( Qt::NoBrush );

    this->contour2_pen = new QPen(QColor(0, 0, 0, 255), 2);
    this->contour2 = new QGraphicsPolygonItem( this );
    this->contour2->setPen( *this->contour2_pen );
    this->contour2->setBrush( Qt::NoBrush );

    // Default resize rect setup
    this->resize_rect = new QGraphicsPolygonItem( this );
    this->resize_rect->setPen( *this->pen );
    this->resize_rect->setBrush( Qt::NoBrush );

    this->resizeEnabled = true;

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

    if ( (new_p3.x() - new_p2.x()) < 1 )
    {
        new_p3.setX( new_p2.x() + 1 );
        new_p4.setX( new_p3.x() );
    }

    if ( (new_p2.y() - new_p1.y()) < 1 )
    {
        new_p2.setY( new_p1.y() + 1 );
        new_p3.setY( new_p2.y() );
    }

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

    if( p1.x() == 0 || p1.y() == 0 )
    {
        this->setPoint1( QPointF( p2.x(), p4.y() ) );
    } else {
        this->setPoint1( p1 );
    }

    if( p2.x() == 0 || p2.y() == 0 )
    {
        this->setPoint2( QPointF( p1.x(), p3.y() ) );
    } else {
        this->setPoint2( p2 );
    }

    if( p3.x() == 0 || p3.y() == 0 )
    {
        this->setPoint3( QPointF( p4.x(), p2.y() ) );
    } else {
        this->setPoint3( p3 );
    }

    if( p4.x() == 0 || p4.y() == 0 )
    {
        this->setPoint4( QPointF( p3.x(), p1.y() ) );
    } else {
        this->setPoint4( p4 );
    }

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

QVector<QPointF> ObjectRect::simulate_moveObject(QPointF pos, QPointF offset_1, QPointF offset_2, QPointF offset_3, QPointF offset_4)
{
    QVector<QPointF> output;

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

    output.append( centered_point_1 );
    output.append( centered_point_2 );
    output.append( centered_point_3 );
    output.append( centered_point_4 );

    return output;
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

float ObjectRect::getBorderWidth()
{
    return this->pen->width();
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
    this->resize_rect->setPen( *this->pen );
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
    case ObjectRectState::ToBlur:
        this->brush->setColor( QColor(255, 255, 0, 50) );
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
    float pen_width = this->contour_pen->width();
    contour_points.append( QPointF(this->points[0].x() - pen_width, this->points[0].y() - pen_width) );
    contour_points.append( QPointF(this->points[1].x() - pen_width, this->points[1].y() + pen_width) );
    contour_points.append( QPointF(this->points[2].x() + pen_width, this->points[2].y() + pen_width) );
    contour_points.append( QPointF(this->points[3].x() + pen_width, this->points[3].y() - pen_width) );
    QPolygonF contour_polygon( contour_points );
    this->contour->setPolygon( contour_polygon );

    QVector<QPointF> contour2_points;
    float pen2_width = this->contour2_pen->width();
    contour2_points.append( QPointF(contour_points[0].x() - pen2_width, contour_points[0].y() - pen2_width) );
    contour2_points.append( QPointF(contour_points[1].x() - pen2_width, contour_points[1].y() + pen2_width) );
    contour2_points.append( QPointF(contour_points[2].x() + pen2_width, contour_points[2].y() + pen2_width) );
    contour2_points.append( QPointF(contour_points[3].x() + pen2_width, contour_points[3].y() - pen2_width) );
    QPolygonF contour2_polygon( contour2_points );
    this->contour2->setPolygon( contour2_polygon );

    // Draw resize rect
    QVector<QPointF> resize_rect_points;
    resize_rect_points.append( QPointF(this->points[2].x() + 10, this->points[2].y() + 10) );
    resize_rect_points.append( QPointF(this->points[2].x() + 10, this->points[2].y()) );
    resize_rect_points.append( QPointF(this->points[2].x(),  this->points[2].y()) );
    resize_rect_points.append( QPointF(this->points[2].x(),  this->points[2].y() + 10) );
    this->resize_rect_polygon = QPolygonF( resize_rect_points );
    this->resize_rect->setPolygon( this->resize_rect_polygon );


    if(this->getSize().width() < 70 || this->getSize().height() < 70 )
    {
        this->pen->setWidth( 1 );
        this->contour_pen->setWidth( 1 );
        this->contour2_pen->setWidth( 1 );
    } else {
        this->pen->setWidth( 2 );
        this->contour_pen->setWidth( 2 );
        this->contour2_pen->setWidth( 2 );
    }

    this->setPen( *this->pen );
    this->resize_rect->setPen( *this->pen );
    this->contour->setPen( *this->contour_pen );
    this->contour2->setPen( *this->contour2_pen );

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

void ObjectRect::setSourceImagePath(QString path)
{
    this->projection_parameters.source_image = path;
}

QString ObjectRect::getSourceImagePath()
{
    return this->projection_parameters.source_image;
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

int ObjectRect::getSubType()
{
    return this->info.sub_type;
}

void ObjectRect::setType(int value)
{
    this->info.type = value;

    switch(value)
    {
    case ObjectType::ToBlur:
        this->setObjectRectState( ObjectRectState::ToBlur );
        break;
    }
}

void ObjectRect::setSubType(int value)
{
    this->info.sub_type = value;
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

    if(value != "None")
        this->setResizeEnabled( false );
}

ObjectRect* ObjectRect::copy()
{
    ObjectRect* rect_copy = new ObjectRect;
    rect_copy->setObjectRectType( this->getObjectRectType() );
    rect_copy->setType( this->getType() );
    rect_copy->setObjectRectState( this->getObjectRectState() );
    rect_copy->setManualStatus( this->getManualStatus() );
    rect_copy->setAutomaticStatus( this->getAutomaticStatus() );
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

void ObjectRect::mergeWith(ObjectRect *rect)
{
    rect->mapTo(this->proj_width(),
                           this->proj_height(),
                           this->proj_azimuth(),
                           this->proj_elevation(),
                           this->proj_aperture()
                          );

    this->setProjectionPoints(rect->getPoint1(),
                              rect->getPoint2(),
                              rect->getPoint3(),
                              rect->getPoint4());

    this->setType( rect->getType() );
    this->setSubType( rect->getSubType() );
    this->setObjectRectState( rect->getObjectRectState() );

    this->setManualStatus( rect->getManualStatus() );
    this->setAutomaticStatus( rect->getAutomaticStatus() );

    this->setBlurred( rect->isBlurred() );
}

void ObjectRect::mapTo(float width, float height, float azimuth, float elevation, float aperture)
{
    QPointF p1, p2, p3, p4;

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
              &p1.rx(),
              &p1.ry());

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
              &p2.rx(),
              &p2.ry());

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
              &p3.rx(),
              &p3.ry());

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
              &p4.rx(),
              &p4.ry());

    this->setPoints( p1, p2, p3, p4 );
}

void ObjectRect::setResizeEnabled(bool value)
{
    this->resizeEnabled = value;
    this->resize_rect->setVisible( value );
}

bool ObjectRect::isResizeEnabled()
{
    return this->resizeEnabled;
}

