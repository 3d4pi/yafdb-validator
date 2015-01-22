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

/* Includes */
#include "panoramaviewer.h"
#include "editview.h"

/* Constructor */
PanoramaViewer::PanoramaViewer(QWidget *parent, bool connectSlots) :
    QGraphicsView(parent)
{

    /* Remove side bars on the panorama viewer */
    this->verticalScrollBar()->blockSignals(true);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->horizontalScrollBar()->blockSignals(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    /* Initialize object list index */
    this->rect_list_id_index = 1;

    /* Initialize position container */
    this->position.start_x = 0;
    this->position.start_y = 0;
    this->position.start_azimuth = 0.0;
    this->position.start_elevation = 0.0;
    this->position.azimuth = 0.0;
    this->position.elevation = 0.0;
    this->position.old_azimuth = 0.0;
    this->position.old_elevation = 0.0;
    this->position.old_width = 0;
    this->position.old_height = 0;

    /* Intialize image info container */
    this->image_info.channels = 0;
    this->image_info.height = 0;
    this->image_info.image = NULL;

    /* Initialize default mode */
    this->mode = PanoramaViewerMode::None;

    /* Configure default settings */
    this->scale_factor = 1.0;
    this->zoom_min = 20.0;
    this->zoom_max = 120.0;
    this->position.aperture_delta = 100.0;
    this->position.aperture = ( this->position.aperture_delta * ( LG_PI / 180.0 ) );;
    this->position.old_aperture = this->position.aperture;
    this->threads_count = 1;
    this->vis_group = PanoramaViewerVisGroups::All;
    this->moveEnabled = true;
    this->zoomEnabled = true;
    this->createEnabled = true;
    this->editEnabled = true;

    /* Initialize pixmap state container */
    this->pixmap_initialized = false;

    /* Initialize in rect containers */
    this->increation_rect.rect = NULL;
    this->selected_rect = NULL;

    /* Initialize pressed keys container */
    this->pressed_keys.CTRL = false;

    /* Create default scene */
    this->scene = new QGraphicsScene();
    this->setScene(this->scene);

    /* Create sight */
    QPen sight_pen;

    /* Set sight color */
    sight_pen.setColor( Qt::red );

    /* Set sight contour width */
    sight_pen.setWidth( 2 );

    /* Set sight width */
    this->sight_width = 800;

    /* Add sight to scene */
    this->sight = this->scene->addRect( this->dest_image_map.width() / 2,
                                        this->dest_image_map.height() / 2,
                                        this->sight_width,
                                        this->sight_width,
                                        sight_pen);

    /* Connect signal for labels refresh */
    if( connectSlots )
    {
        connect(this, SIGNAL(refreshLabels()), parent, SLOT(refreshLabels()));
        connect(this, SIGNAL(updateScaleSlider(int)), parent, SLOT(updateScaleSlider(int)));
    }
}

/* Main setup function */
void PanoramaViewer::setup(int width,
                           int height,
                           float scale_factor,
                           float zoom_min,
                           float zoom_max,
                           float zoom_def,
                           int threads)
{
    /* Assign values */
    this->scale_factor = scale_factor;
    this->zoom_min = zoom_min;
    this->zoom_max = zoom_max;
    this->position.aperture_delta = zoom_def;
    this->position.aperture = (zoom_def * (LG_PI / 180.0) );
    this->threads_count = threads;

    /* Resize PanoramaViewer */
    this->resize( width, height );
}

/* Function to load specified image */
void PanoramaViewer::loadImage(QString path)
{
    /* Save image path */
    this->image_path = path;

    /* Create a temporary image */
    IplImage * temp_image = NULL;

    /* Load image */
    temp_image = cvLoadImage( path.toStdString().c_str(), CV_LOAD_IMAGE_UNCHANGED );

    /* Save image details */
    this->image_info.channels = (temp_image->nChannels + 1);
    this->image_info.width = temp_image->width;
    this->image_info.height = temp_image->height;

    /* Convert it to QImage */
    this->image_info.image = IplImage2QImage( temp_image );

    /* Release temporary image */
    cvReleaseImage( &temp_image );

    /* Render PanoramaViewer */
    this->render();
}

/* Function to update scene (viewer) */
void PanoramaViewer::updateScene(float azimuth,
                                 float elevation,
                                 float zoom)
{
    /* Exif if input image is not loaded */
    if( this->image_info.image == NULL )
        return;

    /* Compute destination image size */
    int dest_width = this->width() * scale_factor;
    int dest_height = this->height() * scale_factor;

    /* Clamp azimuth/elevation */
    float clamped_azimuth = clampRad(azimuth, -360.0, 360.0);
    float clamped_elevation = clamp(elevation, -90.0, 90.0);

    /* Save old size */
    this->position.old_width = this->dest_image.width();
    this->position.old_height = this->dest_image.height();

    /* Allocate destination image */
    this->dest_image = QImage (dest_width, dest_height, QImage::Format_RGB32);

    /* Project gnomonic image */
    lg_etg_apperturep(

        ( inter_C8_t * ) this->image_info.image->bits(),
        this->image_info.width,
        this->image_info.height,
        this->image_info.channels,
        ( inter_C8_t * ) dest_image.bits(),
        dest_width,
        dest_height,
        this->image_info.channels,
        clamped_azimuth,
        clamped_elevation,
        0.0,
        zoom,
        li_bilinearf,
        this->threads_count
    );

    /* Convert projected image to pixmap */
    this->dest_image_map = QPixmap::fromImage(this->dest_image);

    /* Check if pixmap has been already initialized */
    if(this->pixmap_initialized)
    {
        /* Remove old pixmap */
        this->scene->removeItem(this->last_pixmap);
        delete this->last_pixmap;
    } else {
        /* Update value to true */
        this->pixmap_initialized = true;
    }

    /* Add pixmap to scene (panorama gnomonic */
    this->last_pixmap = this->scene->addPixmap(this->dest_image_map);

    /* Update z value (to see other objects */
    this->last_pixmap->setZValue(-1);

    /* Set scene boundaries */
    this->scene->setSceneRect(this->dest_image_map.rect());

    /* Fit image in scene */
    this->fitInView(this->dest_image_map.rect());

    /* Update sight position */
    this->sight->setPos( QPointF( (dest_width / 2) - ((this->sight_width / 2) * (this->scale_factor / this->position.aperture)),
                                  (dest_height / 2) - ((this->sight_width / 2) * (this->scale_factor / this->position.aperture)) ) );

    /* Update sight scale */
    this->sight->setScale( this->scale_factor / this->position.aperture );
}

/* Function to render panorama and all objects */
void PanoramaViewer::render()
{
    /* Call scene update procedure */
    this->updateScene(
        this->position.azimuth,
        this->position.elevation,
        this->position.aperture
    );

    /* Iterate over objects */
    foreach(ObjectRect* rect, this->rect_list)
    {
        /* Object size filtering check */
        if( rect->getSize().width() < 1 ||
               rect->getSize().height() < 1 )
        {
            /* Remove rect from list */
            this->rect_list.removeOne( rect );
            delete rect;

            /* Refresh main window labels */
            emit refreshLabels();
        } else {

            /* Check if object is manual */
            if(rect->getAutomaticStatus() == "None")
            {
                /* Check if current parameter are the same as object's projection parameters */
                if( rect->proj_azimuth() != this->position.azimuth ||
                        rect->proj_elevation() != this->position.elevation ||
                        rect->proj_aperture() != this->position.aperture )
                {
                    /* Disable resizing */
                    rect->setResizeEnabled( false );
                } else {

                    /* Enable resizing */
                    rect->setResizeEnabled( true );
                }
            } else {

                /* Disable resizing */
                rect->setResizeEnabled( false );
            }

            /* Map object to current projection parameters */
            rect->mapTo(this->dest_image_map.width(),
                        this->dest_image_map.height(),
                        this->position.azimuth,
                        this->position.elevation,
                        this->position.aperture);
        }
    }

    /* Apply visibility groups */
    this->applyVisGroup();
}

/* Function to update zoom of current scene */
void PanoramaViewer::setZoom(float zoom_level)
{
    /* Backup current positions */
    this->backupPosition();

    /* Convert zoom value to radians */
    this->position.aperture = ( zoom_level * ( LG_PI / 180.0 ) );
    this->position.aperture_delta = zoom_level;

    /* Render scene */
    this->render();
}

void PanoramaViewer::setView(float azimuth,
                             float elevation)
{
    /* Update view angles */
    this->position.azimuth = azimuth;
    this->position.elevation = elevation;

    /* Render scene */
    this->render();
}

/* Mouse wheel event */
void PanoramaViewer::wheelEvent(QWheelEvent* event)
{
    /* Determine delta */
    int delta = (event->delta() / 120);

    /* Check if CTRL ke is pressed */
    if( this->pressed_keys.CTRL )
    {
        /* Update scale factor */
        this->scale_factor = (this->scale_factor + (delta / 50.0));
        this->scale_factor = clamp(this->scale_factor, 0.1, 1.0);

        /* Update scale slider */
        emit updateScaleSlider( this->scale_factor * 10 );

        /* Render scene */
        this->render();
    } else {

        /* Check if zoom is enabled */
        if(!this->zoomEnabled)
            return;

        /* Update current zoom level */
        float old_zoom = this->position.aperture_delta;
        this->position.aperture_delta -= (delta * 1.5);
        this->position.aperture_delta = clamp(this->position.aperture_delta, this->zoom_min, this->zoom_max);

        /* Apply zoom level */
        if(this->position.aperture_delta != old_zoom)
            this->setZoom(this->position.aperture_delta);

    }
}

/* Mouse press event */
void PanoramaViewer::mousePressEvent(QMouseEvent* event)
{
    /* Store normalized mouse coords */
    QPointF mouse_scene = this->mapToScene(event->pos());

    /* Check presence of left click */
    if(event->buttons() & Qt::LeftButton)
    {
        /* Exit function if move is disabled */
        if(!this->moveEnabled)
            return;

        /* Enable mouse tracking */
        this->setMouseTracking(true);

        /* Switch to mooving mode */
        this->mode = PanoramaViewerMode::Move;

        /* Store base positions (Used to determine offset to move in panorama later) */
        this->position.start_x = mouse_scene.x();
        this->position.start_y = mouse_scene.y();

        /* Store base directions (Used to determine offset to move in panorama later) */
        this->position.start_azimuth = this->position.azimuth / (LG_PI / 180.0);
        this->position.start_elevation = this->position.elevation / (LG_PI / 180.0);

    }

    /* Check presence of right click */
    else if (event->buttons() & Qt::RightButton)
    {
        /* Get item at mouse position */
        QGraphicsPolygonItem* clicked_poly = qgraphicsitem_cast<QGraphicsPolygonItem*>(this->itemAt(event->x(), event->y()));

        /* Check if item is valid */
        if (clicked_poly != NULL)
        {
            /* Get clicked rect */
            ObjectRect* clicked_rect = qgraphicsitem_cast<ObjectRect*>(clicked_poly->parentItem());

            /* Verify that clicked object is a widget and is not null */
            if (clicked_rect != NULL)
            {
                /* Assign selected rect */
                this->selected_rect = clicked_rect;
            }
        }

        /* If  selected rct is valid */
        if(this->selected_rect)
        {

            /* Exit if resize is disabled */
            if(!this->selected_rect->isResizeEnabled())
                return;

            /* Store base positions (Used to determine offset to move in panorama later) */
            this->position.start_x = mouse_scene.x();
            this->position.start_y = mouse_scene.y();

            /* Determine mouse click pointer offset 1 */
            this->position.offset_1 = QPointF(
                                          mouse_scene.x() - this->selected_rect->getPoint1().x(),
                                          mouse_scene.y() - this->selected_rect->getPoint1().y()
                                      );

            /* Determine mouse click pointer offset 2 */
            this->position.offset_2 = QPointF(
                                          mouse_scene.x() - this->selected_rect->getPoint2().x(),
                                          mouse_scene.y() - this->selected_rect->getPoint2().y()
                                      );

            /* Determine mouse click pointer offset 3 */
            this->position.offset_3 = QPointF(
                                          mouse_scene.x() - this->selected_rect->getPoint3().x(),
                                          mouse_scene.y() - this->selected_rect->getPoint3().y()
                                      );

            /* Determine mouse click pointer offset 4 */
            this->position.offset_4 = QPointF(
                                          mouse_scene.x() - this->selected_rect->getPoint4().x(),
                                          mouse_scene.y() - this->selected_rect->getPoint4().y()
                                      );

            /* Check if mouse is inside rect */
            if( (this->position.offset_3.x() > 0)
                    && (this->position.offset_3.y() > 0) )
            {
                /* Switch mode to resize */
                this->mode = PanoramaViewerMode::ObjectResize;
            } else {
                /* Switch mode to move */
                this->mode = PanoramaViewerMode::ObjectMove;
            }

            /* Enable mouse tracking */
            this->setMouseTracking(true);

        } else {

            /* Exit if creation is disabled */
            if(!this->createEnabled)
                return;

            /* Check if mouse is in sight */
            if(!this->isInSight( mouse_scene ))
                return;

            /* Switch mode to create */
            this->mode = PanoramaViewerMode::ObjectCreate;

            /* Enable mouse tracking */
            this->setMouseTracking(true);

            /* Store base positions (Used to determine size and position of object later) */
            this->increation_rect.start_x = mouse_scene.x();
            this->increation_rect.start_y = mouse_scene.y();

        }
    }
}

/* Mouse double click event */
void PanoramaViewer::mouseDoubleClickEvent(QMouseEvent *event)
{
    /* Exit if edition is disabled */
    if(!this->editEnabled)
        return;

    /* Get item at mouse position */
    QGraphicsPolygonItem* clicked_poly = qgraphicsitem_cast<QGraphicsPolygonItem*>(this->itemAt(event->x(), event->y()));

    /* Check if item is valid */
    if(clicked_poly != NULL)
    {
        /* Get rect */
        ObjectRect* clicked_rect = qgraphicsitem_cast<ObjectRect*>(clicked_poly->parentItem());

        /* Verify that clicked object valid */
        if (clicked_rect != NULL)
        {
            /* If right double click */
            if( (event->buttons() == Qt::RightButton) )
            {
                /* Update view to rect's projection parameters */
                this->position.azimuth = clicked_rect->proj_azimuth();
                this->position.elevation = clicked_rect->proj_elevation();
                this->position.aperture = clicked_rect->proj_aperture();
                this->position.aperture_delta = (this->position.aperture / (LG_PI / 180.0));

                /* Render scene */
                this->render();
            }

            /* If left double click */
            else if ( event->buttons() == Qt::LeftButton )
            {
                /* Create and show a new edition window */
                EditView* w = new EditView(this, clicked_rect, this->image_info, NULL, EditMode::Scene);
                w->setAttribute( Qt::WA_DeleteOnClose );
                w->show();
            }
        }
    }
}

// Mouse buttons release handler
void PanoramaViewer::mouseReleaseEvent(QMouseEvent *)
{
    // Reset mode
    this->mode = PanoramaViewerMode::None;

    // Reset temporary object
    if( this->increation_rect.rect )
    {
        this->increation_rect.rect->setProjectionPoints();
        this->increation_rect.rect = NULL;
    }
    this->selected_rect = NULL;

    // Disable mouse tracking
    this->setMouseTracking(false);
}

/* Mouse move event */
void PanoramaViewer::mouseMoveEvent(QMouseEvent* event)
{
    /* Store normalized mouse coords */
    QPointF mouse_scene = this->mapToScene(event->pos());

    /* Store mouse coords */
    int MouseX = mouse_scene.x();
    int MouseY = mouse_scene.y();

    /* Moving mouse section (move in panorama) */
    if(this->mode == PanoramaViewerMode::Move)
    {

        /* Determine the displacement delta */
        int delta_x = (MouseX - this->position.start_x);
        int delta_y = (MouseY - this->position.start_y);

        /* Apply delta to azimuth and elevation */
        float azimuth   = (this->position.start_azimuth   - ( (delta_x * this->position.aperture) * 0.1 ) / (this->scale_factor * 2.0) );
        float elevation = (this->position.start_elevation + ( (delta_y * this->position.aperture) * 0.1 ) / (this->scale_factor * 2.0) );

        /* Save current view */
        this->backupPosition();

        /* Clamp azimuth and elevation */
        this->position.azimuth   = clampRad(azimuth, -360.0, 360.0) * (LG_PI / 180.0);
        this->position.elevation = clamp(elevation, -90.0, 90.0) * (LG_PI / 180.0);

        /* Render scene */
        this->render();

    }

    /* Creation section (create a new object) */
    else if (this->mode == PanoramaViewerMode::ObjectCreate)
    {

        /* Check if mouse in in sight */
        if(!this->isInSight( mouse_scene ))
            return;

        /* Check if temporary object is created */
        if(this->increation_rect.rect == NULL)
        {
            /* Create new object */
            this->increation_rect.rect = new ObjectRect();

            /* Configure object */
            this->increation_rect.rect->setObjectAutomaticState( ObjectAutomaticState::Manual );
            this->increation_rect.rect->setObjectManualState( ObjectManualState::Valid );
            this->increation_rect.rect->setObjectType( ObjectType::None );
            this->increation_rect.rect->setManualStatus( "Valid" );
            this->increation_rect.rect->setBlurred( true );

            /* Assign id to object */
            this->increation_rect.rect->setId( this->rect_list_id_index++ );

            /* Configure object projection parameters */
            this->increation_rect.rect->setProjectionParametters(this->position.azimuth,
                    this->position.elevation,
                    this->position.aperture,
                    this->dest_image.width(),
                    this->dest_image.height());

            /* Set object source image path */
            this->increation_rect.rect->setSourceImagePath( this->image_path );

            /* Move object to mouse coords */
            this->increation_rect.rect->setPoints(
                mouse_scene,
                mouse_scene,
                mouse_scene,
                mouse_scene
            );

            /* Add object to scene */
            this->rect_list.append( this->increation_rect.rect );
            this->scene->addItem( this->increation_rect.rect );

            /* Refresh vis groups */
            this->applyVisGroup();

            /* Refresh main window labels */
            emit refreshLabels();

        } else  {

            /* Clamp mouse X */
            float mouse_x = clamp(mouse_scene.x(),
                                  this->increation_rect.rect->getPoint1().x() + this->increation_rect.rect->getBorderWidth(),
                                  this->width());

            /* Clamp mouse Y */
            float mouse_y = clamp(mouse_scene.y(),
                                  this->increation_rect.rect->getPoint1().y() + this->increation_rect.rect->getBorderWidth(),
                                  this->height());

            /* Move/resize selection object to mouse coords */
            this->increation_rect.rect->setPoint3_Rigid(
                QPointF(mouse_x, mouse_y)
            );

            /* Display size tooltip */
            QToolTip::showText(event->globalPos(),
                               QString::number( (int) (this->increation_rect.rect->getSizeCurrent().width() / this->scale_factor) ) + "x" +
                               QString::number( (int) (this->increation_rect.rect->getSizeCurrent().height() / this->scale_factor )),
                               this, rect() );

        }
    }

     /* Mooving section (move an existing object) */
    else if(this->mode == PanoramaViewerMode::ObjectMove)
    {
        /* Exit function if current projection parameters are not the same as object */
        if( this->selected_rect->proj_azimuth() != this->position.azimuth ||
                this->selected_rect->proj_elevation() != this->position.elevation ||
                this->selected_rect->proj_aperture() != this->position.aperture)
        {
            return;
        }

        /* Simulate a mouse move to ged coordinates */
        QVector<QPointF> points = this->selected_rect->simulate_moveObject(mouse_scene,
                                  this->position.offset_1,
                                  this->position.offset_2,
                                  this->position.offset_3,
                                  this->position.offset_4);

        /* Check if object is in sight */
        if(!this->isObjectInSight( points[0], points[1], points[2], points[3] ))
            return;

        /* Move object */
        this->selected_rect->moveObject(mouse_scene,
                                        this->position.offset_1,
                                        this->position.offset_2,
                                        this->position.offset_3,
                                        this->position.offset_4);

        /* Update projection paramaters */
        this->selected_rect->setProjectionParametters(this->position.azimuth,
                this->position.elevation,
                this->position.aperture,
                this->dest_image.width(),
                this->dest_image.height());

        /* Set projection points */
        this->selected_rect->setProjectionPoints();
    }

    /* Resize section (resize an existing object) */
    else if(this->mode == PanoramaViewerMode::ObjectResize)
    {
        /* Check if mouse and rect are in in sight */
        if(!this->isObjectInSight( this->selected_rect->getPoint1(),
                                   this->selected_rect->getPoint2(),
                                   this->selected_rect->getPoint3(),
                                   this->selected_rect->getPoint4()) && !this->isInSight( mouse_scene ))
            return;

        /* Check if scene has moved */
        if( this->selected_rect->proj_azimuth() != this->position.azimuth ||
                this->selected_rect->proj_elevation() != this->position.elevation ||
                this->selected_rect->proj_aperture() != this->position.aperture)
        {
            return;
        }

        /* Clamp mouse X */
        float mouse_x = clamp(mouse_scene.x(),
                              this->selected_rect->getPoint1().x() + this->selected_rect->getBorderWidth(),
                              this->width());

        /* Clamp mouse Y */
        float mouse_y = clamp(mouse_scene.y(),
                              this->selected_rect->getPoint1().y() + this->selected_rect->getBorderWidth(),
                              this->height());

        /* Move selection object to mouse coords */
        this->selected_rect->setPoint3_Rigid(
            QPointF(mouse_x, mouse_y),
            this->position.offset_3
        );

        /* Update projection paramaters */
        this->selected_rect->setProjectionParametters(this->position.azimuth,
                this->position.elevation,
                this->position.aperture,
                this->dest_image.width(),
                this->dest_image.height());

        /* Set projection points */
        this->selected_rect->setProjectionPoints();

        /* Display size tooltip */
        QToolTip::showText(event->globalPos(),
                           QString::number( (int) (this->selected_rect->getSize().width() / this->scale_factor) ) + "x" +
                           QString::number( (int) (this->selected_rect->getSize().height() / this->scale_factor )),
                           this, rect() );
    }
}

/* Main window resize event */
void PanoramaViewer::resizeEvent(QResizeEvent *)
{

    /* Check if dimensions has changed */
    if(this->width() != this->previous_width
            && this->height() != this->previous_height)
    {

        /* Store new dimensions */
        this->previous_width = this->width();
        this->previous_height = this->height();

        /* Render scene */
        this->render();
    }

}

/* Key press event */
void PanoramaViewer::keyPressEvent(QKeyEvent *event)
{
    /* Check if CTRL key is pressed */
    if(event->key() == Qt::Key_Control)
        this->pressed_keys.CTRL = true;
}

/* Key release event */
void PanoramaViewer::keyReleaseEvent(QKeyEvent *)
{
    /* Mark CTRL key as not pressed */
    this->pressed_keys.CTRL = false;
}

/* Function to crop an image from object */
QImage PanoramaViewer::cropObject(ObjectRect* rect)
{
    /* Copy rect */
    ObjectRect* rect_mapped = rect->copy();

    /* Map object to current projection parameters */
    rect_mapped->mapTo(this->width(),
                       this->height(),
                       rect->proj_azimuth(),
                       rect->proj_elevation(),
                       rect->proj_aperture());

    /* Set object's projection points */
    rect_mapped->setProjectionPoints();

    /* Get selection from object's points */
    QRect rect_sel(
        QPoint(rect_mapped->proj_point_1().x() + rect_mapped->getBorderWidth(), rect_mapped->proj_point_1().y() + rect_mapped->getBorderWidth()),
        QPoint(rect_mapped->proj_point_3().x() - rect_mapped->getBorderWidth(), rect_mapped->proj_point_3().y() - rect_mapped->getBorderWidth())
    );

    /* Delete temp rect */
    delete rect_mapped;

    /* Create temporary destination image */
    QImage temp_dest(this->width(), this->height(), QImage::Format_RGB32);

    /* Project gnomonic image */
    lg_etg_apperturep(

        ( inter_C8_t * ) this->image_info.image->bits(),
        this->image_info.width,
        this->image_info.height,
        this->image_info.channels,
        ( inter_C8_t * ) temp_dest.bits(),
        this->width(),
        this->height(),
        this->image_info.channels,
        rect->proj_azimuth(),
        rect->proj_elevation(),
        0.0,
        rect->proj_aperture(),
        li_bilinearf,
        this->threads_count
    );

    /* Crop and return image */
    return temp_dest.copy(rect_sel);

}

/* Function to backup current postion (used for projection) */
void PanoramaViewer::backupPosition()
{
    /* Save angles */
    this->position.old_azimuth = this->position.azimuth;
    this->position.old_elevation = this->position.elevation;
    this->position.old_aperture = this->position.aperture;
}

/* Function to determine if an object is visible or not */
bool PanoramaViewer::isObjectVisible(ObjectRect *rect)
{

    /* Check if point 1 is visible */
    int state = g2g_point(rect->proj_width(),
                          rect->proj_height(),
                          rect->proj_azimuth(),
                          rect->proj_elevation(),
                          rect->proj_aperture(),
                          rect->proj_point_1().x(),
                          rect->proj_point_1().y(),

                          this->dest_image_map.width(),
                          this->dest_image_map.height(),
                          this->position.azimuth,
                          this->position.elevation,
                          this->position.aperture);

    /* Check if point 2 is visible */
    int state2 = g2g_point(rect->proj_width(),
                           rect->proj_height(),
                           rect->proj_azimuth(),
                           rect->proj_elevation(),
                           rect->proj_aperture(),
                           rect->proj_point_2().x(),
                           rect->proj_point_2().y(),

                           this->dest_image_map.width(),
                           this->dest_image_map.height(),
                           this->position.azimuth,
                           this->position.elevation,
                           this->position.aperture);

    /* Check if point 3 is visible */
    int state3 = g2g_point(rect->proj_width(),
                           rect->proj_height(),
                           rect->proj_azimuth(),
                           rect->proj_elevation(),
                           rect->proj_aperture(),
                           rect->proj_point_3().x(),
                           rect->proj_point_3().y(),

                           this->dest_image_map.width(),
                           this->dest_image_map.height(),
                           this->position.azimuth,
                           this->position.elevation,
                           this->position.aperture);

    /* Check if point 4 is visible */
    int state4 = g2g_point(rect->proj_width(),
                           rect->proj_height(),
                           rect->proj_azimuth(),
                           rect->proj_elevation(),
                           rect->proj_aperture(),
                           rect->proj_point_4().x(),
                           rect->proj_point_4().y(),

                           this->dest_image_map.width(),
                           this->dest_image_map.height(),
                           this->position.azimuth,
                           this->position.elevation,
                           this->position.aperture);

    /* Return proper result */
    if( state != 1 || state2 != 1 || state3 != 1 || state4 != 1) {
        return false;
    } else {
        return true;
    }
}

/* Function to set the visibility group */
void PanoramaViewer::setVisGroup(int visgroup)
{
    this->vis_group = visgroup;
    this->applyVisGroup();
}

/* Function to apply visibility groups */
void PanoramaViewer::applyVisGroup()
{
    /* Main visibility group switch */
    switch (this->vis_group) {

    case PanoramaViewerVisGroups::All:

        /* Iterate over objects */
        foreach(ObjectRect* obj, this->rect_list)
        {
            /* Check if object is visible */
            if( this->isObjectVisible( obj ) ){
                obj->setVisible( true );
            } else {
                obj->setVisible( false );
            }
        }
        break;
    case PanoramaViewerVisGroups::Automatic:

        /* Iterate over objects */
        foreach(ObjectRect* obj, this->rect_list)
        {
            /* Check if object is visible */
            if( this->isObjectVisible( obj ) )
            {
                /* Check object is automatic */
                if( obj->getAutomaticStatus().toLower() != "none" )
                {
                    obj->setVisible( true );
                } else {
                    obj->setVisible( false );
                }
            } else {
                obj->setVisible( false );
            }
        }
        break;
    case PanoramaViewerVisGroups::Manual:

        /* Iterate over objects */
            foreach(ObjectRect* obj, this->rect_list)
            {
                /* Check if object is visible */
                if( this->isObjectVisible( obj ) )
                {
                    /* Check if object is manual */
                    if( obj->getAutomaticStatus().toLower() == "none" )
                    {
                        obj->setVisible( true );
                    } else {
                        obj->setVisible( false );
                    }
                } else {
                    obj->setVisible( false );
                }
            }
        break;
    case PanoramaViewerVisGroups::InCreation:

        /* Iterate over objects */
        foreach(ObjectRect* obj, this->rect_list)
        {
             obj->setVisible( false );
        }

        /* Check if in creation rect is valid */
        if( this->increation_rect.rect != NULL )
        {
            /* Check if object is visible */
            if( this->isObjectVisible( this->increation_rect.rect ) )
            {
                this->increation_rect.rect->setVisible( true );
            } else {
                this->increation_rect.rect->setVisible( false );
            }
        } else {

            /* Check if scene have objects defined */
            if( this->rect_list.length() > 0 )
            {
                /* Check if last object is visible */
                if( this->isObjectVisible( this->rect_list.last() ) )
                {
                    this->rect_list.last()->setVisible( true );
                } else {
                    this->rect_list.last()->setVisible( false );
                }
            }
        }
        break;
    }
}

/* Function to determine if a point is in sight */
bool PanoramaViewer::isInSight(QPointF pos,
                               float tolerance)
{
    /* Get sight rect */
    QSizeF sight_rect_size = (this->sight->boundingRect().size() * this->sight->scale());

    /* Check if point is in creation zone */
    if (!((pos.x() + tolerance) >= this->sight->pos().x()
            &&(pos.x() + tolerance) < (this->sight->pos().x() + sight_rect_size.width())
            && (pos.y() + tolerance) >= this->sight->pos().y()
            && (pos.y() + tolerance) < (this->sight->pos().y() + sight_rect_size.height())))
    {
        return false;
    } else {
        return true;
    }
}

/* Function to determine if a polygon is in sight */
bool PanoramaViewer::isObjectInSight(QPointF p1,
                                     QPointF p2,
                                     QPointF p3,
                                     QPointF p4)
{
    /* Check if rect is in creation zone */
    if(!(this->isInSight( p1 )
            && this->isInSight( p2 )
            && this->isInSight( p3 )
            && this->isInSight( p4 )))
    {
        return false;
    } else {
        return true;
    }
}

/* Function to toggle mooving */
void PanoramaViewer::setMoveEnabled(bool value)
{
    /* Assign value */
    this->moveEnabled = value;
}

/* Function to toggle zooming */
void PanoramaViewer::setZoomEnabled(bool value)
{
    /* Assign value */
    this->zoomEnabled = value;
}

/* Function to toggle object creation */
void PanoramaViewer::setCreateEnabled(bool value)
{
    /* Assign value */
    this->createEnabled = value;
}

/* Function to toggle object edition */
void PanoramaViewer::setEditEnabled(bool value)
{
    /* Assign value */
    this->editEnabled = value;
}

/* Slot for main window labels refresh */
void PanoramaViewer::refreshLabels_slot()
{
    /* Emit refresh signal */
    emit refreshLabels();
}

/* Slot for main window scale slider update */
void PanoramaViewer::updateScaleSlider_slot(int value)
{
    /* Emit refresh signal */
    emit updateScaleSlider(value);
}

/* Function to get current scene */
QGraphicsScene* PanoramaViewer::getScene()
{
    /* Return value */
    return this->scene;
}

/* Function to get minimal zoom value */
float PanoramaViewer::minZoom()
{
    /* Return value */
    return this->zoom_min;
}

/* Function to get maximal zoom value */
float PanoramaViewer::maxZoom()
{
    /* Return value */
    return this->zoom_max;
}

/* Function to get number of threads */
int PanoramaViewer::threads()
{
    /* Return value */
    return this->threads_count;
}

/* Function to get current image scale factor */
float PanoramaViewer::getScaleFactor()
{
    /* Return value */
    return this->scale_factor;
}

/* Function to set image scale factor */
void PanoramaViewer::setScaleFactor(float value)
{
    /* Assign value */
    this->scale_factor = value;
}

/* Function to get current azimuth */
float PanoramaViewer::azimuth()
{
    /* Return value */
    return this->position.azimuth;
}

/* Function to get current elevation */
float PanoramaViewer::elevation()
{
    /* Return value */
    return this->position.elevation;
}

/* Function to get current aperture */
float PanoramaViewer::aperture()
{
    /* Return value */
    return this->position.aperture;
}
