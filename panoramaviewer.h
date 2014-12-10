#ifndef PANORAMAVIEWER_H
#define PANORAMAVIEWER_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QScrollBar>
#include <QImage>
#include <QPixmap>
#include <QWheelEvent>
#include <QMouseEvent>

#include <inter-all.h>
#include <gnomonic-all.h>
#include "g2g_point.h"
#include "objectrect.h"
#include "detectedobject.h"

class PanoramaViewer : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PanoramaViewer(QWidget *parent = 0);

    float scale_factor;

    QGraphicsScene* scene;

    QImage dest_image;
    QPixmap dest_image_map;

    void setup(int width, int height, float scale_factor, float zoom_min, float zoom_max, float zoom_def, int threads);
    void loadImage(QString path);
    void loadImage(QImage image);
    void updateScene(float azimuth, float elevation, float zoom);
    void render();
    void setZoom(float zoom);
    void setView(float azimuth, float elevation);
    QImage cropObject(ObjectRect* rect);

    void updateLabels();
    void backupPosition();

    void setMoveEnabled(bool value);
    void setZoomEnabled(bool value);

    QList<ObjectRect*> rect_list;

    QImage src_image;
    QString image_path;

    int threads_count;
    float zoom_min;
    float zoom_max;

    struct {
        int start_x;
        int start_y;

        QPointF offset_1;
        QPointF offset_2;
        QPointF offset_3;
        QPointF offset_4;

        float start_azimuth;
        float start_elevation;
        float azimuth;
        float elevation;
        float aperture;
        float aperture_delta;

        float old_aperture;
        float old_azimuth;
        float old_elevation;
        float old_width;
        float old_height;
    } position;

public slots:
    void refreshLabels_slot();

private:

    struct Mode
    {
        enum Type
        {
            None = 0, Move = 1, Create = 2, MoveCreate = 3, MoveResize = 4
        };
    };

    struct Point
    {
        enum Type
        {
            None = 0, Point1 = 1, Point2 = 2, Point3 = 3, Point4 = 4
        };
    };

    void wheelEvent(QWheelEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *releaseEvent);
    void resizeEvent(QResizeEvent *);
    void mouseDoubleClickEvent ( QMouseEvent * event );

    bool isObjectVisible(ObjectRect* rect);

    int mode;
    int rect_list_index;
    int resizePoint;
    bool moveEnabled;
    bool zoomEnabled;

    int previous_height;
    int previous_width;
    bool pixmap_initialized;

    QPixmap src_image_map;

    QGraphicsPixmapItem* last_pixmap;
    ObjectRect * selected_rect;
    QGraphicsEllipseItem* sight;

    struct {
        int start_x;
        int start_y;
        ObjectRect * rect;
    } increation_rect;

signals:
    void refreshLabels();

};

#endif // PANORAMAVIEWER_H
