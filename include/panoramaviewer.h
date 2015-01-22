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

#ifndef PANORAMAVIEWER_H
#define PANORAMAVIEWER_H

/* Includes */
#include <QApplication>
#include <QToolTip>
#include <QScrollBar>
#include <QImage>
#include <QPixmap>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QWheelEvent>
#include <QMouseEvent>

#include <inter-all.h>
#include <gnomonic-all.h>

#include "g2g_point.h"
#include "objectrect.h"
#include "utils.h"

/* Visibility groups struct */
struct PanoramaViewerVisGroups
{
    enum Type
    {
        All = 1, Automatic = 2, Manual = 3, InCreation = 4
    };
};

/* Main class */
class PanoramaViewer : public QGraphicsView
{
    Q_OBJECT

/* Public functions / variables */
public:

    /* Constructor */
    explicit PanoramaViewer(QWidget *parent = 0, bool connectSlots = true);

    /* Variable to store all image informations */
    image_info_struct image_info;

    /* Main ObjectRect list */
    QList<ObjectRect*> rect_list;

    /* Main ObjectRect id indexes */
    int rect_list_id_index;

    /* Main setup function */
    void setup(int width,
               int height,
               float scale_factor,
               float zoom_min,
               float zoom_max,
               float zoom_def,
               int threads);

    /* Function to load specified image */
    void loadImage(QString path);

    /* Function to set zoom level */
    void setZoom(float zoom);

    /* Function to set panorama view orientation */
    void setView(float azimuth,
                 float elevation);


    /* Function to update main window labels */
    void updateLabels();

    /* Function to backup current postion (used for projection) */
    void backupPosition();

    /* Function to toggle mooving */
    void setMoveEnabled(bool value);

    /* Function to toggle zooming */
    void setZoomEnabled(bool value);

    /* Function to toggle object creation */
    void setCreateEnabled(bool value);

    /* Function to toggle object edition */
    void setEditEnabled(bool value);

    /* Function to determine if an object is visible or not */
    bool isObjectVisible(ObjectRect* rect);

    /* Function to set the visibility group */
    void setVisGroup( int visgroup );

    /* Function to update scene (viewer) */
    void updateScene(float azimuth,
                     float elevation,
                     float zoom);

    /* Function to render panorama and all objects */
    void render();

    /* Function to crop an object and return its tile */
    QImage cropObject(ObjectRect* rect);

    /* Function to get current scene */
    QGraphicsScene* getScene();

    /* Function to get minimal zoom value */
    float minZoom();

    /* Function to get maximal zoom value */
    float maxZoom();

    /* Function to get number of threads */
    int threads();

    /* Function to get current image scale factor */
    float getScaleFactor();

    /* Function to set image scale factor */
    void setScaleFactor(float value);

    /* Function to get current azimuth */
    float azimuth();

    /* Function to get current elevation */
    float elevation();

    /* Function to get current aperture */
    float aperture();

/* Public slots */
public slots:

    /* Slot for main window labels refresh */
    void refreshLabels_slot();

    /* Slot for main window scale slider update */
    void updateScaleSlider_slot(int value);

/* Private functions / variables */
private:

    /* Panorama viewer working mode struct */
    struct PanoramaViewerMode
    {
        enum Type
        {
            None = 0, Move = 1, ObjectCreate = 2, ObjectMove = 3, ObjectResize = 4
        };
    };

    /* Main PanoramaViewer scene */
    QGraphicsScene* scene;

    /* Destination image to be shown */
    QImage dest_image;

    /* Destination image to be shown in pixmap format */
    QPixmap dest_image_map;

    /* Main image path */
    QString image_path;

    /* Main threads count */
    int threads_count;

    /* Zoom settings */
    float zoom_min;
    float zoom_max;

    /* Scale factor setting */
    float scale_factor;

    /* PanoramaViewer working mode */
    int mode;

    /* PanoramaViewer properties */
    bool moveEnabled;
    bool zoomEnabled;
    bool createEnabled;
    bool editEnabled;

    /* Main pixmap initialized value storage */
    bool pixmap_initialized;

    /* Variables to store previous sizes on window resize */
    int previous_height;
    int previous_width;

    /* Sight width */
    int sight_width;

    /* Current visibility group */
    int vis_group;

    /* Last rendered pixmap */
    QGraphicsPixmapItem* last_pixmap;

    /* Main sight container */
    QGraphicsRectItem* sight;

    /* Current selected rect container */
    ObjectRect * selected_rect;

    /* Struture to store all movement values */
    struct position_struct{

        /* Start position storage for mooving */
        int start_x;
        int start_y;

        /* Start angles storage for mooving */
        float start_azimuth;
        float start_elevation;

        /* Previous angles storage */
        float old_aperture;
        float old_azimuth;
        float old_elevation;

        /* Previous image sizes storage */
        float old_width;
        float old_height;

        /* Clicking offsets for object mooving */
        QPointF offset_1;
        QPointF offset_2;
        QPointF offset_3;
        QPointF offset_4;

        /* Angles storage */
        float azimuth;
        float elevation;

        /* Aperture (zoom) storage */
        float aperture;

        /* Aperture delta (mouse scroll value) storage */
        float aperture_delta;

    } position;

    /* In creation rect parameters */
    struct increation_rect_struct{
        int start_x;
        int start_y;
        ObjectRect * rect;
    } increation_rect;

    /* Pressed keys container */
    struct pressed_keys_struct{
        bool CTRL;
    } pressed_keys;

    /* Function to determine if a point is in sight */
    bool isInSight(QPointF pos,
                   float tolerance = 0.0);

    /* Function to determine if a polygon is in sight */
    bool isObjectInSight(QPointF p1,
                         QPointF p2,
                         QPointF p3,
                         QPointF p4);

    /* Function to apply visibility groups */
    void applyVisGroup();

/* Signals */
signals:

    /* Function to refresh main window labels */
    void refreshLabels();

    /* Function to update main window scale slider */
    void updateScaleSlider(int value);

/* Protected functions / variables */
protected:

    /* Mouse wheel event */
    void wheelEvent(QWheelEvent* event);

    /* Mouse move event */
    void mouseMoveEvent(QMouseEvent* event);

    /* Mouse press event */
    void mousePressEvent(QMouseEvent* event);

    /* Mouse release event */
    void mouseReleaseEvent(QMouseEvent *releaseEvent);

    /* Mouse double click event */
    void mouseDoubleClickEvent ( QMouseEvent * event );

    /* Key press event */
    void keyPressEvent(QKeyEvent *event);

    /* Key release event */
    void keyReleaseEvent(QKeyEvent *event);

    /* Main window resize event */
    void resizeEvent(QResizeEvent *);

};

#endif // PANORAMAVIEWER_H
