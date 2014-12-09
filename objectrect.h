#ifndef ObjectRect_H
#define ObjectRect_H

#include <QGraphicsPolygonItem>
#include <QPen>
#include <QPaintEvent>
#include <QDebug>
#include "types.h"

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

    // Size getters
    QSizeF getSize();

    // Id setter
    void setId(int id);

    // Id getter
    int getId();

    // Projection paramaters get/set
    void setProjectionParametters(float azimuth,
                             float elevation,
                             float aperture,
                             float width,
                             float height);

    void setProjectionPoints();

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
    void setType(int value);

    bool isBlurred();
    bool isValidated();
    void setBlurred(bool value);
    void setValidated(bool value);

    QString getManualStatus();
    QString getAutomaticStatus();
    void setManualStatus(QString value);
    void setAutomaticStatus(QString value);

private:

    // Points container
    QVector<QPointF> points;

    // Polygon container
    QPolygonF polygon;

    // Pen container
    QPen* pen;

    // Brush container
    QBrush* brush;

    // Private functions
    void render();

    // Id container
    int id;

    // Projection parameters structure
    struct {
        float azimuth;
        float elevation;
        float aperture;
        float width;
        float height;
        QVector<QPointF> points;
    } projection_parameters;

    // Projection parameters structure
    struct {
        int type;
        bool blurred;
        bool validated;
        QString manual_status;
        QString automatic_status;
    } info;

};

#endif // ObjectRect_H
