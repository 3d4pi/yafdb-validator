#ifndef OBJECTRECT_H
#define OBJECTRECT_H

#include <QWidget>

#include "types.h"

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

    ~ObjectRect();

    projection_parameters_container projection_parameters;

    void setRectType(int type);
    void setObjectType(int type);

    void setBlurred(bool blur);
    void setValidState(int state);

    void setPos(QPointF p1, QPointF p2, int type = 1);

    void moveRect(QPointF point, QPointF offset);

    void setAutomaticStatus(QString value);
    void setManualStatus(QString value);

    QPointF point_1;
    QPointF point_2;

    QPointF point_3;
    QPointF point_4;
    QSize  size;

    int id;


    QString manualStatus;
    QString autoStatus;

    int recttype;
    int objecttype;
    bool blurred;
    bool valid;
    int validstate;

private:

    //void setPosPoint1(QPoint loc, float scale_factor);
    //void setPosPoint2(QPoint loc, float scale_factor);

    Ui::ObjectRect *ui;

    void setPosPoint1(QPointF point);
    void setPosPoint2(QPointF point);

};

#endif // OBJECTRECT_H
