#ifndef PANORAMAVIEWER_H
#define PANORAMAVIEWER_H

#include <inter-all.h>
#include <gnomonic-all.h>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QScrollBar>
#include <QImage>
#include <QPixmap>

#include <QWheelEvent>
#include <QMouseEvent>
#include <QList>
#include <QGraphicsPixmapItem>
#include <QRubberBand>
#include "objectrect.h"
#include "util.h"

struct position_container {
    int start_x;
    int start_y;

    QPointF offset;

    int x;
    int y;

    float start_azimuth;
    float start_elevation;

    float azimuth;
    float elevation;
};

struct create_container {
    int start_x;
    int start_y;
    ObjectRect * rect;
};

struct Mode
{
    enum Type
    {
        None = 0, Move = 1, Create = 2, MoveCreate = 3
    };
};

class PanoramaViewer : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PanoramaViewer(QWidget *parent = 0);

    QGraphicsScene* scene;

    QImage src_image;
    QPixmap src_image_map;

    QImage dest_image;
    QPixmap dest_image_map;

    int mode;

    int   threads_count;
    float current_zoom;
    float current_zoom_rad;

    float scale_factor;
    float zoom_min;
    float zoom_max;

    QGraphicsPixmapItem* last_pixmap;
    bool pixmap_initialized;

    position_container position;
    create_container   create_position;
    QList<ObjectRect*> rect_list;
    ObjectRect * selected_rect;

    void setup(float scale_factor, float zoom_min, float zoom_max, float zoom_def, int threads);
    void loadImage(QString path);
    void updateScene(float azimuth, float elevation, float zoom);
    void render();

    void setZoom(float zoom);
    void setView(float azimuth, float elevation);

    QImage cropObject(ObjectRect* rect);

signals:

public slots:

private:

    void wheelEvent(QWheelEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *releaseEvent);
    void resizeEvent(QResizeEvent *);

    int previous_height;
    int previous_width;

};

#endif // PANORAMAVIEWER_H
