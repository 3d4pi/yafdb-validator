#ifndef ObjectRect_H
#define ObjectRect_H

#include <QGraphicsPolygonItem>
#include <QPen>
#include <QPaintEvent>
#include <QDebug>

#include "types.h"
#include "etg_point.h"
#include "g2g_point.h"

class ObjectRect : public QGraphicsPolygonItem
{

public:
    ObjectRect();

    // Points setters
    void setPoint1(QPointF point);
    void setPoint2(QPointF point);
    void setPoint3(QPointF point);
    void setPoint3_Rigid(QPointF point);
    void setPoint3_Rigid(QPointF point, QPointF offset);
    void setPoint4(QPointF point);
    void setPoints(QPointF point1, QPointF point2, QPointF point3, QPointF point4);

    // Functions
    void moveObject(QPointF pos,
                    QPointF offset_1,
                    QPointF offset_2,
                    QPointF offset_3,
                    QPointF offset_4);

    QVector<QPointF> simulate_moveObject(QPointF pos,
                                         QPointF offset_1,
                                         QPointF offset_2,
                                         QPointF offset_3,
                                         QPointF offset_4);

    // Points getters
    QPointF getPoint1();
    QPointF getPoint2();
    QPointF getPoint3();
    QPointF getPoint4();
    QVector<QPointF> getPoints();

    // Size setters
    void setSize(QSizeF size);

    // Object parameters
    void setObjectRectType(int type);
    void setObjectRectState(int state);
    void setObjectType(int type);
    int getObjectRectType();
    int getObjectRectState();
    int getObjectType();

    // Size getters
    QSizeF getSize();
    float  getBorderWidth();

    // Id setter
    void setId(int id);

    // Id getter
    int getId();

    // Projection paramaters get/set
    void setProjectionParametters(float width,
                                  float height,
                                  float azimuth,
                                  float elevation,
                                  float aperture);

    void setSourceImagePath(QString path);
    QString getSourceImagePath();

    void setProjectionPoints();
    void setProjectionPoints(QPointF p1,
                             QPointF p2,
                             QPointF p3,
                             QPointF p4);

    // Copy function
    ObjectRect* copy();
    void mergeWith(ObjectRect* rect);

    // Point map function
    void mapTo(float width,
               float height,
               float azimuth,
               float elevation,
               float aperture);

    void mapFromSpherical(float source_width,
                          float source_height,
                          float dest_width,
                          float dest_height,
                          float dest_azimuth,
                          float dest_elevation,
                          float dest_aperture,
                          float dest_zoom_min,
                          float dest_zoom_max);

    float proj_azimuth();
    float proj_elevation();
    float proj_aperture();

    QPointF proj_point_1();
    QPointF proj_point_2();
    QPointF proj_point_3();
    QPointF proj_point_4();

    float proj_width();
    float proj_height();

    // Informations get/set
    int getType();
    int getSubType();
    void setType(int value);
    void setSubType(int value);

    bool isBlurred();
    bool isValidated();
    void setBlurred(bool value);

    QString getManualStatus();
    QString getAutomaticStatus();
    void setManualStatus(QString value);
    void setAutomaticStatus(QString value);

    void setResizeEnabled(bool value);
    bool isResizeEnabled();

    QList<ObjectRect*> childrens;

private:

    // Contour object
    QGraphicsPolygonItem* contour;
    QGraphicsPolygonItem* contour2;

    // Resize rect object
    QGraphicsPolygonItem* resize_rect;

    // Points container
    QVector<QPointF> points;

    // Polygon container
    QPolygonF polygon;
    QPolygonF resize_rect_polygon;

    // Pen container
    QPen* pen;
    QPen* contour_pen;
    QPen* contour2_pen;

    // Brush container
    QBrush* brush;

    // Private functions
    void render();

    // Id container
    int id;

    // Type containers
    int rect_type;
    int type;

    // State container
    int rect_state;

    // Projection parameters structure
    struct {
        float azimuth;
        float elevation;
        float aperture;
        float width;
        float height;
        QVector<QPointF> points;
        QString source_image;
    } projection_parameters;

    // Projection parameters structure
    struct {
        int type;
        int sub_type;
        bool blurred;
        bool validated;
        QString manual_status;
        QString automatic_status;
    } info;

    bool resizeEnabled;

};

#endif // ObjectRect_H
