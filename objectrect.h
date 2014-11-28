#ifndef OBJECTRECT_H
#define OBJECTRECT_H

#include <QWidget>
#include "util.h"

struct RectType
{
    enum Type
    {
        Valid = 1, Invalid = 2, Manual = 3
    };
};

struct RectMove
{
    enum Type
    {
        All = 1, Only_Point1 = 2, Only_Point2 = 3
    };
};

struct projection_parameters_container {
    float azimuth;
    float elevation;
    float aperture;
    float scale_factor;
};

namespace Ui {
class ObjectRect;
}

class ObjectRect : public QWidget
{
    Q_OBJECT

public:
    explicit ObjectRect(QWidget *parent = 0);
    explicit ObjectRect(QWidget *parent, int type);

    ~ObjectRect();

    projection_parameters_container projection_parameters;

    void setType(int type);
    void setValid(bool value);
    void setPos(QPointF p1, QPointF p2, normalization_struct norm_params, int type = 1);
    void update(normalization_struct norm_params);

    void moveRect(QPointF point, normalization_struct norm_params, QPointF offset);

    QPointF point_1;
    QPointF point_2;
    QSize  size;

private:

    //void setPosPoint1(QPoint loc, float scale_factor);
    //void setPosPoint2(QPoint loc, float scale_factor);

    Ui::ObjectRect *ui;

    bool valid;

    void setPosPoint1(QPointF point, normalization_struct norm_params);
    void setPosPoint2(QPointF point, normalization_struct norm_params);

};

#endif // OBJECTRECT_H
