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
#include "utils.h"

class PanoramaViewer : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PanoramaViewer(QWidget *parent = 0);

    float scale_factor;
    int rect_list_index;

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
    void setCreateEnabled(bool value);
    void setEditEnabled(bool value);

    bool isObjectVisible(ObjectRect* rect);

    QList<ObjectRect*> rect_list;

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

    struct image_info_struct{
        QImage* image;
        int width;
        int height;
        int channels;
    } image_info;

public slots:
    void refreshLabels_slot();
    void updateScaleSlider_slot(int value);

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

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    bool isInSight(QPointF pos, float tolerance = 0.0);
    bool isObjectInSight(QPointF p1, QPointF p2, QPointF p3, QPointF p4);

    int mode;
    int resizePoint;
    bool moveEnabled;
    bool zoomEnabled;
    bool createEnabled;
    bool editEnabled;

    int previous_height;
    int previous_width;
    bool pixmap_initialized;

    int sight_width;

    QPixmap src_image_map;

    QGraphicsPixmapItem* last_pixmap;
    ObjectRect * selected_rect;
    QGraphicsRectItem* sight;

    struct {
        int start_x;
        int start_y;
        ObjectRect * rect;
    } increation_rect;

    struct {
        bool CTRL;
    } pressed_keys;

signals:
    void refreshLabels();
    void updateScaleSlider(int value);

};

#endif // PANORAMAVIEWER_H
