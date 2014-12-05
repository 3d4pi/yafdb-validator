#ifndef OBJECTRECT2_H
#define OBJECTRECT2_H

#include <QGraphicsPolygonItem>
#include <QPen>
#include <QPaintEvent>
#include <QDebug>
#include "types.h"

class ObjectRect2 : public QGraphicsPolygonItem
{

public:
    ObjectRect2();

    // Points setters
    void setPoint1(QPointF point);
    void setPoint2(QPointF point);
    void setPoint3(QPointF point);
    void setPoint4(QPointF point);
    void setPoints(QPointF point1, QPointF point2, QPointF point3, QPointF point4);

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

};

#endif // OBJECTRECT2_H
