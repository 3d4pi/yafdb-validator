#include "objectrect.h"

/* Constructor */
ObjectRect::ObjectRect()
{
    /* Append default points */
    this->points.append(QPointF(0.0, 0.0));
    this->points.append(QPointF(0.0, 0.0));
    this->points.append(QPointF(0.0, 0.0));
    this->points.append(QPointF(0.0, 0.0));
    this->polygon = QPolygonF( this->points );

    /* Default id */
    this->id = 0;

    /* Default pen setup */
    this->pen = new QPen(QColor(0, 255, 255, 255), 2);
    this->brush = new QBrush(QColor(0, 255, 0, 50), Qt::SolidPattern);

    /* Default projection parameters */
    this->projection_parameters.azimuth = 0.0;
    this->projection_parameters.elevation = 0.0;
    this->projection_parameters.aperture = 0.0;
    this->projection_parameters.width = 0.0;
    this->projection_parameters.height = 0.0;

    /* Default projection points */
    this->projection_parameters.points.append(QPointF(0.0, 0.0));
    this->projection_parameters.points.append(QPointF(0.0, 0.0));
    this->projection_parameters.points.append(QPointF(0.0, 0.0));
    this->projection_parameters.points.append(QPointF(0.0, 0.0));

    /* Default informations */
    this->info.automatic_status = "None";
    this->info.manual_status = "None";
    this->info.blurred = false;
    this->info.validated = false;
    this->info.type = ObjectType::None;
    this->info.sub_type = ObjectSubType::None;

    /* Default contour setup (First) */
    this->contour_pen = new QPen(QColor(255, 255, 255, 255), 2);
    this->contour = new QGraphicsPolygonItem( this );
    this->contour->setPen( *this->contour_pen );
    this->contour->setBrush( Qt::NoBrush );

    /* Default contour setup (Second) */
    this->contour2_pen = new QPen(QColor(0, 0, 0, 255), 2);
    this->contour2 = new QGraphicsPolygonItem( this );
    this->contour2->setPen( *this->contour2_pen );
    this->contour2->setBrush( Qt::NoBrush );

    /* Default resize rect setup */
    this->resize_rect = new QGraphicsPolygonItem( this );
    this->resize_rect->setPen( *this->pen );
    this->resize_rect->setBrush( Qt::NoBrush );
    this->resizeEnabled = true;

    /* Render object */
    this->render();
}

/* Function to set point 1 */
void ObjectRect::setPoint1(QPointF point)
{
    /* Assign point 1 value */
    this->points[0] = point;

    /* Render object */
    this->render();
}

/* Function to set point 2 */
void ObjectRect::setPoint2(QPointF point)
{
    /* Assign point 2 value */
    this->points[1] = point;

    /* Render object */
    this->render();
}

/* Function to set point 3 */
void ObjectRect::setPoint3(QPointF point)
{
    /* Assign point 3 value */
    this->points[2] = point;

    /* Render object */
    this->render();
}

/* Function to set point 3 in rigid structure, moving other points at the same time, ex: resize edge */
void ObjectRect::setPoint3_Rigid(QPointF point)
{
    /* Backup points */
    QPointF new_p1 = this->points[0];
    QPointF new_p2 = this->points[1];
    QPointF new_p3 = point;
    QPointF new_p4 = this->points[3];

    /* Compute new points */
    new_p2.setY( new_p3.y() );
    new_p4.setX( new_p3.x() );

    /* Clamp x axis */
    if ( (new_p3.x() - new_p2.x()) < 1 )
    {
        new_p3.setX( new_p2.x() + 1 );
        new_p4.setX( new_p3.x() );
    }

    /* Clamp y axis */
    if ( (new_p2.y() - new_p1.y()) < 1 )
    {
        new_p2.setY( new_p1.y() + 1 );
        new_p3.setY( new_p2.y() );
    }

    /* Update positions */
    this->setPoints( new_p1, new_p2, new_p3, new_p4 );

    /* Render object */
    this->render();
}

/* Function to set point 3 in rigid structure with an offset, moving other points at the same time, ex: resize edge */
void ObjectRect::setPoint3_Rigid(QPointF point, QPointF offset)
{

    /* Apply offset */
    QPointF centered (
        point.x() - offset.x(),
        point.y() - offset.y()
    );

    /* Apply modification to point 3 */
    this->setPoint3_Rigid(centered);
}

/* Function to set point 4 */
void ObjectRect::setPoint4(QPointF point)
{
     /* Assign point 4 value */
    this->points[3] = point;

    /* Render object */
    this->render();
}

/* Function to set all the points at the same time */
void ObjectRect::setPoints(QPointF p1, QPointF p2, QPointF p3, QPointF p4)
{

    /* Try to compute point 1 if not specified */
    if( p1.x() == 0 || p1.y() == 0 )
    {
        this->setPoint1( QPointF( p2.x(), p4.y() ) );
    } else {
        this->setPoint1( p1 );
    }

    /* Try to compute point 2 if not specified */
    if( p2.x() == 0 || p2.y() == 0 )
    {
        this->setPoint2( QPointF( p1.x(), p3.y() ) );
    } else {
        this->setPoint2( p2 );
    }

    /* Try to compute point 3 if not specified */
    if( p3.x() == 0 || p3.y() == 0 )
    {
        this->setPoint3( QPointF( p4.x(), p2.y() ) );
    } else {
        this->setPoint3( p3 );
    }

    /* Try to compute point 4 if not specified */
    if( p4.x() == 0 || p4.y() == 0 )
    {
        this->setPoint4( QPointF( p3.x(), p1.y() ) );
    } else {
        this->setPoint4( p4 );
    }

    /* Render object */
    this->render();
}

/* Function to move object at specified coordinates, with an offset */
void ObjectRect::moveObject(QPointF pos,
                            QPointF offset_1,
                            QPointF offset_2,
                            QPointF offset_3,
                            QPointF offset_4)
{

    /* Center point 1 with offset */
    QPointF centered_point_1 (
        pos.x() - offset_1.x(),
        pos.y() - offset_1.y()
    );

    /* Center point 2 with offset */
    QPointF centered_point_2 (
        pos.x() - offset_2.x(),
        pos.y() - offset_2.y()
    );

    /* Center point 3 with offset */
    QPointF centered_point_3 (
        pos.x() - offset_3.x(),
        pos.y() - offset_3.y()
    );

    /* Center point 4 with offset */
    QPointF centered_point_4 (
        pos.x() - offset_4.x(),
        pos.y() - offset_4.y()
    );

    /* Move object to new points */
    this->setPoint1(centered_point_1);
    this->setPoint2(centered_point_2);
    this->setPoint3(centered_point_3);
    this->setPoint4(centered_point_4);


    /* Render object */
    this->render();
}

/* Function acting same as moveObject but just return the new points without moving object */
QVector<QPointF> ObjectRect::simulate_moveObject(QPointF pos, QPointF offset_1, QPointF offset_2, QPointF offset_3, QPointF offset_4)
{
    /* Output points variable */
    QVector<QPointF> output;

    /* Center point 1 with offset */
    QPointF centered_point_1 (
        pos.x() - offset_1.x(),
        pos.y() - offset_1.y()
    );

    /* Center point 2 with offset */
    QPointF centered_point_2 (
        pos.x() - offset_2.x(),
        pos.y() - offset_2.y()
    );

    /* Center point 3 with offset */
    QPointF centered_point_3 (
        pos.x() - offset_3.x(),
        pos.y() - offset_3.y()
    );

    /* Center point 4 with offset */
    QPointF centered_point_4 (
        pos.x() - offset_4.x(),
        pos.y() - offset_4.y()
    );

    /* Append results */
    output.append( centered_point_1 );
    output.append( centered_point_2 );
    output.append( centered_point_3 );
    output.append( centered_point_4 );

    /* Result results */
    return output;
}

/* Function to get point 1 */
QPointF ObjectRect::getPoint1()
{
    /* Return value */
    return this->points[0];
}

/* Function to get point 2 */
QPointF ObjectRect::getPoint2()
{
    /* Return value */
    return this->points[1];
}

/* Function to get point 3 */
QPointF ObjectRect::getPoint3()
{
    /* Return value */
    return this->points[2];
}

/* Function to get point 4 */
QPointF ObjectRect::getPoint4()
{
    /* Return value */
    return this->points[3];
}

/* Function to get all points */
QVector<QPointF> ObjectRect::getPoints()
{
    /* Return values */
    return this->points;
}

/* Function to resize object */
void ObjectRect::setSize(QSizeF size)
{
    /* Backup points */
    QPointF new_p1 = this->points[0];
    QPointF new_p2 = this->points[1];
    QPointF new_p3 = this->points[2];
    QPointF new_p4 = this->points[3];

    /* Compute width */
    new_p2.setX( new_p1.x() + size.width() );
    new_p3.setX( new_p1.x() + size.width() );

    /* Compute height */
    new_p4.setY( new_p1.y() + size.height() );
    new_p3.setY( new_p1.y() + size.height() );

    /* Update positions */
    this->setPoints( new_p1, new_p2, new_p3, new_p4 );

    /* Render object */
    this->render();
}

/* Function to get object size (initial projection) */
QSizeF ObjectRect::getSize()
{
    /* Baclup values */
    QPointF p1 = this->proj_point_1();
    QPointF p2 = this->proj_point_2();
    QPointF p4 = this->proj_point_4();

    /* Output vriable */
    QSizeF size;

    /* Compute width */
    size.setWidth( p4.x() -  p1.x());

    /* Compute height */
    size.setHeight( p2.y() -  p1.y());

    /* Return value */
    return size;
}

/* Function to get object size (current projection) */
QSizeF ObjectRect::getSizeCurrent()
{
    /* Baclup values */
    QPointF p1 = this->points[0];
    QPointF p2 = this->points[1];
    QPointF p4 = this->points[3];

    /* Output vriable */
    QSizeF size;

    /* Compute width */
    size.setWidth( p4.x() -  p1.x());

    /* Compute height */
    size.setHeight( p2.y() -  p1.y());

    /* Return value */
    return size;
}

/* Function to get border width */
float ObjectRect::getBorderWidth()
{
    /* Return value */
    return this->pen->width();
}

/* Function to set object's ID */
void ObjectRect::setId(int id)
{
    /* Assign value */
    this->id = id;
}

/* Function to get object's ID */
int ObjectRect::getId()
{
    /* Return value */
    return this->id;
}

/* Function to set object rect type (Contour color & status ) */
void ObjectRect::setObjectAutomaticState(int state)
{
    /* Assign value */
    this->automatic_state = state;

    /* Set proper color depending on rect type specified */
    switch(state)
    {
    case ObjectAutomaticState::Manual:
        this->pen->setColor( QColor(0, 255, 255, 255) );
        break;
    case ObjectAutomaticState::Valid:
        this->pen->setColor( QColor(0, 255, 0, 255) );
        break;
    case ObjectAutomaticState::Invalid:
        this->pen->setColor( QColor(255, 0, 0, 255) );
        break;
    }

    /* Refresh main contour */
    this->setPen( * this->pen );
    this->resize_rect->setPen( *this->pen );
}

/* Function to get object rect type (Contour color & status ) */
int ObjectRect::getObjectAutomaticState()
{
    /* Return result */
    return this->automatic_state;
}

void ObjectRect::setObjectManualState(int state)
{
    this->manual_state = state;

    switch(state)
    {
    case ObjectManualState::None:
        this->brush->setColor( QColor(0, 0, 0, 0) );
        break;
    case ObjectManualState::Valid:
        this->brush->setColor( QColor(0, 255, 0, 50) );
        break;
    case ObjectManualState::Invalid:
        this->brush->setColor( QColor(255, 0, 0, 50) );
        break;
    case ObjectManualState::ToBlur:
        this->brush->setColor( QColor(255, 255, 0, 50) );
        break;
    }

    // Render brush
    this->setBrush( * this->brush );
}

int ObjectRect::getObjectManualState()
{
    return this->manual_state;
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


    if(this->getSizeCurrent().width() < 70 || this->getSizeCurrent().height() < 70 )
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

int ObjectRect::getObjectType()
{
    return this->info.type;
}

void ObjectRect::setObjectType(int value)
{
    this->info.type = value;

    switch(value)
    {
    case ObjectType::ToBlur:
        this->setObjectManualState( ObjectManualState::ToBlur );
        break;
    }
}

int ObjectRect::getSubType()
{
    return this->info.sub_type;
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
    return (this->manual_state == ObjectManualState::Valid);
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
    rect_copy->setObjectAutomaticState( this->getObjectAutomaticState() );
    rect_copy->setObjectManualState( this->getObjectManualState() );
    rect_copy->setObjectType( this->getObjectType() );
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

    this->setObjectType( rect->getObjectType() );
    this->setSubType( rect->getSubType() );
    this->setObjectManualState( rect->getObjectManualState() );

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

inline float clamp(float x, float a, float b)
{
    return x < a ? a : (x > b ? b : x);
}


void ObjectRect::mapFromSpherical(float source_width,
                                  float source_height,
                                  float dest_width,
                                  float dest_height,
                                  float dest_azimuth,
                                  float dest_elevation,
                                  float dest_aperture,
                                  float dest_zoom_min,
                                  float dest_zoom_max)
{
    QPointF p1, p3;

    double p1_d_x = ((this->getPoint1().x() / LG_PI2) * source_width);
    double p1_d_y = (((this->getPoint1().y()) + ( LG_PI / 2.0 )) / LG_PI ) * source_height;

    double p3_d_x = ((this->getPoint3().x() / LG_PI2) * source_width);
    double p3_d_y = (((this->getPoint3().y()) + ( LG_PI / 2.0 )) / LG_PI ) * source_height;

    double width  = ( p3_d_x + p1_d_x );
    double height = ( p3_d_y + p1_d_y );

    float aperture = ( ( ( p3_d_x - p1_d_x ) / source_width ) * 30.0 );

    aperture = aperture < dest_zoom_min ? dest_zoom_min : aperture;
    aperture = aperture > dest_zoom_max ? dest_zoom_max : aperture;

    double center_x = ( width / 2.0 );
    double center_y = ( height / 2.0 );

    float azimuth = ( ( center_x / source_width ) * LG_PI2 );
    float elevation = ( ( - ( center_y / source_height ) + 0.5 ) * LG_PI );

    etg_point(source_width,
              source_height,
              p1_d_x,
              p1_d_y,
              dest_width,
              dest_height,
              azimuth,
              elevation,
              aperture,
              &p1.rx(),
              &p1.ry());

    etg_point(source_width,
              source_height,
              p3_d_x,
              p3_d_y,
              dest_width,
              dest_height,
              azimuth,
              elevation,
              aperture,
              &p3.rx(),
              &p3.ry());

    this->setPoints(p1,
                    QPointF(0.0, 0.0),
                    p3,
                    QPointF(0.0, 0.0));

    this->setProjectionPoints();

    this->setProjectionParametters(azimuth,
            elevation,
            aperture,
            dest_width,
            dest_height);

    this->mapTo(dest_width,
                dest_height,
                dest_azimuth,
                dest_elevation,
                dest_aperture);

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

