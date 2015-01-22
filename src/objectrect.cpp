/*
 * yafdb-validate - Yafdb validation tool
 *
 * Copyright (c) 2014-2015 FOXEL SA - http://foxel.ch
 * Please read <http://foxel.ch/license> for more information.
 *
 *
 * Author(s):
 *
 *      Kevin Velickovic <k.velickovic@foxel.ch>
 *
 *
 * This file is part of the FOXEL project <http://foxel.ch>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Additional Terms:
 *
 *      You are required to preserve legal notices and author attributions in
 *      that material or in the Appropriate Legal Notices displayed by works
 *      containing it.
 *
 *      You are required to attribute the work as explained in the "Usage and
 *      Attribution" section of <http://foxel.ch/license>.
 */

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
    /* Assign value */
    this->manual_state = state;

    /* Set proper color depending on state specified */
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

    /* Render contour */
    this->setBrush( * this->brush );
}

/* Function to get object manual state */
int ObjectRect::getObjectManualState()
{
    /* Return result */
    return this->manual_state;
}

/* Function to (re)render object */
void ObjectRect::render()
{
    /* Create polygon from points */
    this->polygon = QPolygonF( this->points );

    /* Update polygon */
    this->setPolygon( this->polygon );

    /* Draw first contour (automatic status) */
    QVector<QPointF> contour_points;
    float pen_width = this->contour_pen->width();
    contour_points.append( QPointF(this->points[0].x() - pen_width, this->points[0].y() - pen_width) );
    contour_points.append( QPointF(this->points[1].x() - pen_width, this->points[1].y() + pen_width) );
    contour_points.append( QPointF(this->points[2].x() + pen_width, this->points[2].y() + pen_width) );
    contour_points.append( QPointF(this->points[3].x() + pen_width, this->points[3].y() - pen_width) );
    QPolygonF contour_polygon( contour_points );
    this->contour->setPolygon( contour_polygon );

    /* Draw shape (manual status) */
    QVector<QPointF> contour2_points;
    float pen2_width = this->contour2_pen->width();
    contour2_points.append( QPointF(contour_points[0].x() - pen2_width, contour_points[0].y() - pen2_width) );
    contour2_points.append( QPointF(contour_points[1].x() - pen2_width, contour_points[1].y() + pen2_width) );
    contour2_points.append( QPointF(contour_points[2].x() + pen2_width, contour_points[2].y() + pen2_width) );
    contour2_points.append( QPointF(contour_points[3].x() + pen2_width, contour_points[3].y() - pen2_width) );
    QPolygonF contour2_polygon( contour2_points );
    this->contour2->setPolygon( contour2_polygon );

    /* Draw resizing rect */
    QVector<QPointF> resize_rect_points;
    resize_rect_points.append( QPointF(this->points[2].x() + 10, this->points[2].y() + 10) );
    resize_rect_points.append( QPointF(this->points[2].x() + 10, this->points[2].y()) );
    resize_rect_points.append( QPointF(this->points[2].x(),  this->points[2].y()) );
    resize_rect_points.append( QPointF(this->points[2].x(),  this->points[2].y() + 10) );
    this->resize_rect_polygon = QPolygonF( resize_rect_points );
    this->resize_rect->setPolygon( this->resize_rect_polygon );

    /* Contour size condition */
    if(this->getSizeCurrent().width() < 70 || this->getSizeCurrent().height() < 70 )
    {
        /* Update contour sizes */
        this->pen->setWidth( 1 );
        this->contour_pen->setWidth( 1 );
        this->contour2_pen->setWidth( 1 );
    } else {

        /* Update contour sizes */
        this->pen->setWidth( 2 );
        this->contour_pen->setWidth( 2 );
        this->contour2_pen->setWidth( 2 );
    }

    /* Update pens */
    this->setPen( *this->pen );
    this->resize_rect->setPen( *this->pen );
    this->contour->setPen( *this->contour_pen );
    this->contour2->setPen( *this->contour2_pen );
}

/* Function to set/update initial projection parameters */
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

/* Function to set/update initial projection points based on current points */
void ObjectRect::setProjectionPoints()
{
    /* Assign value */
    this->projection_parameters.points = this->points;
}

/* Function to set/update initial projection points */
void ObjectRect::setProjectionPoints(QPointF p1, QPointF p2, QPointF p3, QPointF p4)
{
    this->projection_parameters.points[0] = p1;
    this->projection_parameters.points[1] = p2;
    this->projection_parameters.points[2] = p3;
    this->projection_parameters.points[3] = p4;
}

/* Function to set source image path */
void ObjectRect::setSourceImagePath(QString path)
{
    /* Assign value */
    this->projection_parameters.source_image = path;
}

/* Function to get source image path */
QString ObjectRect::getSourceImagePath()
{
    /* Return result */
    return this->projection_parameters.source_image;
}

/* Function to get projection azimuth */
float ObjectRect::proj_azimuth()
{
    /* Return result */
    return this->projection_parameters.azimuth;
}

/* Function to get projection elevation */
float ObjectRect::proj_elevation()
{
    /* Return result */
    return this->projection_parameters.elevation;
}

/* Function to get projection aperture */
float ObjectRect::proj_aperture()
{
    /* Return result */
    return this->projection_parameters.aperture;
}

/* Function to get projection projection point 1 */
QPointF ObjectRect::proj_point_1()
{
    /* Return result */
    return this->projection_parameters.points[0];
}

/* Function to get projection projection point 2 */
QPointF ObjectRect::proj_point_2()
{
    /* Return result */
    return this->projection_parameters.points[1];
}

/* Function to get projection projection point 3 */
QPointF ObjectRect::proj_point_3()
{
    /* Return result */
    return this->projection_parameters.points[2];
}

/* Function to get projection projection point 4 */
QPointF ObjectRect::proj_point_4()
{
    /* Return result */
    return this->projection_parameters.points[3];
}

/* Function to get projection width */
float ObjectRect::proj_width()
{
    /* Return result */
    return this->projection_parameters.width;
}

/* Function to get projection height */
float ObjectRect::proj_height()
{
    /* Return result */
    return this->projection_parameters.height;
}

/* Function to get object type (See ObjectType struct) */
int ObjectRect::getObjectType()
{
    /* Return result */
    return this->info.type;
}

/* Function to set object type (See ObjectType struct) */
void ObjectRect::setObjectType(int value)
{
    /* Assign value */
    this->info.type = value;

    /* Apply special types colors */
    switch(value)
    {
    case ObjectType::ToBlur:
        this->setObjectManualState( ObjectManualState::ToBlur );
        break;
    }
}

/* Function to get object sub-type (See ObjectSubType) */
int ObjectRect::getObjectSubType()
{
    /* Return result */
    return this->info.sub_type;
}

/* Function to set object sub-type (See ObjectSubType) */
void ObjectRect::setObjectSubType(int value)
{
    /* Assign value */
    this->info.sub_type = value;
}

/* Function to determine if object is marked for bluring */
bool ObjectRect::isBlurred()
{
    /* Return result */
    return this->info.blurred;
}

/* Function to mark object for blurring or not */
void ObjectRect::setBlurred(bool value)
{
    /* Assign value */
    this->info.blurred = value;
}

/* Function to determine if object is validated */
bool ObjectRect::isValidated()
{
    /* Return result */
    return (this->manual_state == ObjectManualState::Valid);
}

/* Function to get manual status */
QString ObjectRect::getManualStatus()
{
    /* Return result */
    return this->info.manual_status;
}

/* Function to set manual status */
void ObjectRect::setManualStatus(QString value)
{
    /* Assign value */
    this->info.manual_status = value;
}

/* Function to get automatic status */
QString ObjectRect::getAutomaticStatus()
{
    /* Return result */
    return this->info.automatic_status;
}

/* Function to set automatic status */
void ObjectRect::setAutomaticStatus(QString value)
{
    /* Assign value */
    this->info.automatic_status = value;

    /* If object is automatic disable resizing */
    if(value != "None")
        this->setResizeEnabled( false );
}

/* Function to toggle resizing */
void ObjectRect::setResizeEnabled(bool value)
{
    /* Assign value */
    this->resizeEnabled = value;

    /* Show/hide resize rect */
    this->resize_rect->setVisible( value );
}

/* Function to determine if resizing is enabled */
bool ObjectRect::isResizeEnabled()
{
    /* Return result */
    return this->resizeEnabled;
}


/* Function to copy object */
ObjectRect* ObjectRect::copy()
{
    /* Create a new ObjectRect instance */
    ObjectRect* rect_copy = new ObjectRect;

    /* Restore tags/values */
    rect_copy->setObjectAutomaticState( this->getObjectAutomaticState() );
    rect_copy->setObjectManualState( this->getObjectManualState() );
    rect_copy->setObjectType( this->getObjectType() );
    rect_copy->setManualStatus( this->getManualStatus() );
    rect_copy->setAutomaticStatus( this->getAutomaticStatus() );
    rect_copy->setBlurred( this->isBlurred() );
    rect_copy->setId( this->getId() );

    /* Restore projection parameters */
    rect_copy->setProjectionParametters(this->proj_azimuth(),
                                        this->proj_elevation(),
                                        this->proj_aperture(),
                                        this->proj_width(),
                                        this->proj_height());

    /* Restore points */
    rect_copy->setPoints(this->proj_point_1(),
                         this->proj_point_2(),
                         this->proj_point_3(),
                         this->proj_point_4());

    /* Set projection points */
    rect_copy->setProjectionPoints();

    /* Return result */
    return rect_copy;
}

/* Function to merge object with another */
void ObjectRect::mergeWith(ObjectRect *rect)
{
    /* Map source object to current scene projection parameters */
    rect->mapTo(this->proj_width(),
                this->proj_height(),
                this->proj_azimuth(),
                this->proj_elevation(),
                this->proj_aperture());

    /* Update projection points */
    this->setProjectionPoints(rect->getPoint1(),
                              rect->getPoint2(),
                              rect->getPoint3(),
                              rect->getPoint4());

    /* Update tags/values */
    this->setObjectType( rect->getObjectType() );
    this->setObjectSubType( rect->getObjectSubType() );
    this->setObjectManualState( rect->getObjectManualState() );
    this->setManualStatus( rect->getManualStatus() );
    this->setAutomaticStatus( rect->getAutomaticStatus() );
    this->setBlurred( rect->isBlurred() );
}

/* Function to map current object to specific projection parameters */
void ObjectRect::mapTo(float width, float height, float azimuth, float elevation, float aperture)
{
    /* Destination points containers */
    QPointF p1, p2, p3, p4;

    /* Map point 1 */
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

    /* Map point 2 */
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

    /* Map point 3 */
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

    /* Map point 4 */
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

    /* Update current object points */
    this->setPoints( p1, p2, p3, p4 );
}

/* Function to convert spherical coordinates system to gnomonic */
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
    /* Declare point containers */
    QPointF p1, p3;

    /* Denormalize point 1 */
    double p1_d_x = ((this->getPoint1().x() / LG_PI2) * source_width);
    double p1_d_y = (((this->getPoint1().y()) + ( LG_PI / 2.0 )) / LG_PI ) * source_height;

    /* Denormalize point 2 */
    double p3_d_x = ((this->getPoint3().x() / LG_PI2) * source_width);
    double p3_d_y = (((this->getPoint3().y()) + ( LG_PI / 2.0 )) / LG_PI ) * source_height;

    /* Determine width/height */
    double width  = ( p3_d_x + p1_d_x );
    double height = ( p3_d_y + p1_d_y );

    /* Determine best aperture */
    float aperture = ( ( ( p3_d_x - p1_d_x ) / source_width ) * 30.0 );

    /* Clamp aperture */
    aperture = aperture < dest_zoom_min ? dest_zoom_min : aperture;
    aperture = aperture > dest_zoom_max ? dest_zoom_max : aperture;

    /* Determine center */
    double center_x = ( width / 2.0 );
    double center_y = ( height / 2.0 );

    /* Determine azimuth/elevation */
    float azimuth = ( ( center_x / source_width ) * LG_PI2 );
    float elevation = ( ( - ( center_y / source_height ) + 0.5 ) * LG_PI );

    /* Convert point 1 */
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

    /* Convert point 2 */
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

    /* Update points */
    this->setPoints(p1,
                    QPointF(0.0, 0.0),
                    p3,
                    QPointF(0.0, 0.0));

    /* Update projection points */
    this->setProjectionPoints();

    /* Update projection parameters */
    this->setProjectionParametters(azimuth,
            elevation,
            aperture,
            dest_width,
            dest_height);

    /* Map points */
    this->mapTo(dest_width,
                dest_height,
                dest_azimuth,
                dest_elevation,
                dest_aperture);

}
