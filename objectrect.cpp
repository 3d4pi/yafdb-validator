#include "objectrect.h"
#include "ui_objectrect.h"

#include <QDebug>

ObjectRect::ObjectRect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectRect)
{
    ui->setupUi(this);

    this->setRectType(RectType::Manual);
    this->setObjectType(ObjectType::None);
    this->setValidState(ObjectValidState::None);

    this->autoStatus = "None";
    this->manualStatus = "None";
}

void ObjectRect::setRectType(int type)
{
    this->recttype = type;

    switch(type)
    {
        case RectType::Manual:
            this->setStyleSheet(
                "QWidget#ObjectRect{"
                "background-color: rgba(0, 0, 0, 0);}"

                "QWidget#frame{"
                "border: 2px solid rgb(255, 255, 0);"
                "}"

                "QWidget#frame_2{"
                "border: 2px solid rgb(0, 255, 255);"
                "}"
            );
            break;
        case RectType::Auto:
            this->setStyleSheet(
                "QWidget#ObjectRect{"
                "background-color: rgba(0, 0, 0, 0);}"

                "QWidget#frame{"
                "border: 2px solid rgb(255, 255, 0);"
                "}"

                "QWidget#frame_2{"
                "border: 2px solid rgb(0, 255, 0);"
                "}"
            );
            break;
    }
}

void ObjectRect::setObjectType(int type)
{
    this->objecttype = type;
}

void ObjectRect::setBlurred(bool blur)
{
    this->blurred = blur;
}

void ObjectRect::setAutomaticStatus(QString value)
{
    this->autoStatus = value;
}

void ObjectRect::setManualStatus(QString value)
{
    this->manualStatus = value;
}

void ObjectRect::setValidState(int state)
{

    this->validstate = state;

    return;


    switch(state)
    {
        case ObjectValidState::Valid:
           this->ui->frame_2->setStyleSheet("QFrame#frame_2 {background-color :  rgba(0, 255, 0, 50);}");
            this->valid = true;
            break;
        case ObjectValidState::Invalid:
            this->ui->frame_2->setStyleSheet("QFrame#frame_2 {background-color :  rgba(255, 0, 0, 50);}");
            this->valid = false;
            break;
        case ObjectValidState::None:
            this->ui->frame_2->setStyleSheet("QFrame#frame_2 {background-color :  rgba(0, 0, 0, 0);}");
            this->valid = false;
            break;
    }
}

void ObjectRect::setPosPoint1(QPointF point)
{
    this->move(point.x(), point.y());

    this->point_1 = point;

    this->point_3 = (QPointF(this->point_2.x(), this->point_1.y()));
    this->point_4 = (QPointF(this->point_1.x(), this->point_2.y()));
}

void ObjectRect::setPosPoint2(QPointF point)
{
    int calc_w = (point.x() - this->point_1.x());
    int calc_h = (point.y() - this->point_1.y());

    this->resize(calc_w, calc_h);

    this->point_2 = point;
    this->point_3 = (QPointF(this->point_2.x(), this->point_1.y()));
    this->point_4 = (QPointF(this->point_1.x(), this->point_2.y()));
}

void ObjectRect::moveRect(QPointF point, QPointF offset)
{

    // Center point by clicking offset
    QPointF centered_point(
                    point.x() - offset.x(),
                    point.y() - offset.y()
                );

    // Move object point 1
    this->setPosPoint1(centered_point);

    // Compute destination point 2 location
    QPointF size(
                this->point_1.x() + (this->width()),
                this->point_1.y() + (this->height())
            );

    // Update position of point 2
    this->point_2 = size;

}

void ObjectRect::setPos(QPointF p1, QPointF p2, int mode)
{
    switch(mode)
    {
        case RectMoveType::All:
            this->setPosPoint1(p1);
            this->setPosPoint2(p2);
            break;
        case RectMoveType::Only_Point1:
            this->setPosPoint1(p1);
            break;
        case RectMoveType::Only_Point2:
            this->setPosPoint2(p2);
            break;
    }
}

void ObjectRect::setPos(QPointF p1, QPointF p2, QPointF p3, QPointF p4, int mode)
{
    switch(mode)
    {
        case RectMoveType::All:
            this->setPosPoint1(p1);
            this->setPosPoint2(p2);
            break;
        case RectMoveType::Only_Point1:
            this->setPosPoint1(p1);
            break;
        case RectMoveType::Only_Point2:
            this->setPosPoint2(p2);
            break;
    }

    this->point_3 = p3;
    this->point_4 = p4;
}

void ObjectRect::paintEvent(QPaintEvent *)
{
    /*
    QRect widget_rect = this->rect();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::red);
    painter.setOpacity(100);

    QPainterPath rounded_rect;
    rounded_rect.addRoundRect(1, 1, widget_rect.width() - 2, widget_rect.height() - 2, 12, 12);
    painter.setClipPath(rounded_rect);
    painter.fillPath(rounded_rect,QBrush(Qt::black));*/

    /*
    qreal opacity(0.675);
    int roundness(12);
    QRect widget_rect = this->rect();

    QPainter painter(this);
    painter.save();

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::red);

    // clip
    QPainterPath rounded_rect;
    rounded_rect.addRoundRect(1, 1, widget_rect.width() - 2, widget_rect.height() - 2, roundness, roundness);
    painter.setClipPath(rounded_rect);

    // get clipping region
    QRegion maskregion = painter.clipRegion();

    // mask the widget
    setMask(maskregion);
    painter.setOpacity(opacity);

    // fill path with color
    painter.fillPath(rounded_rect,QBrush(Qt::black));

    // restore painter
    painter.restore();*/
}

ObjectRect::~ObjectRect()
{
    delete ui;
}
