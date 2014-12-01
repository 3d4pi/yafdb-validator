#include "objectrect.h"
#include "ui_objectrect.h"

#include <QDebug>

ObjectRect::ObjectRect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectRect)
{
    ui->setupUi(this);

    this->setState(RectType::Valid);
}

ObjectRect::ObjectRect(QWidget *parent, int state) :
    QWidget(parent),
    ui(new Ui::ObjectRect)
{
    ui->setupUi(this);

    this->setState(state);
    this->resize(1, 1);
}

void ObjectRect::setState(int type)
{
    switch(type)
    {
        case RectType::Invalid:
            this->setStyleSheet(
                "QWidget#ObjectRect{"
                "background-color: rgba(0, 0, 0, 0);}"

                "QWidget#frame{"
                "border: 2px solid rgb(255, 255, 0);"
                "}"

                "QWidget#frame_2{"
                "border: 2px solid rgb(255, 0, 0);"
                "}"
            );
            break;
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
    case RectType::Valid:
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

void ObjectRect::setClass(int classType)
{
    this->classType = classType;
}

void ObjectRect::setBlurred(bool blur)
{
    this->toBlur = blur;
}

void ObjectRect::setValid(bool value)
{
    if(value)
    {
        this->ui->frame_2->setStyleSheet("QFrame#frame_2 {background-color :  rgba(0, 255, 0, 50);}");
        this->valid = true;
    } else {
        this->ui->frame_2->setStyleSheet("QFrame#frame_2 {background-color :  rgba(0, 0, 0, 0);}");
        this->valid = false;
    }
}

void ObjectRect::setPosPoint1(QPointF point, normalization_struct norm_params)
{
    QPointF point_denormalized = util::denormalize(point, norm_params);

    this->move(point_denormalized.x(), point_denormalized.y());

    this->point_1 = point;

    this->point_3 = (QPointF(this->point_2.x(), this->point_1.y()));
    this->point_4 = (QPointF(this->point_1.x(), this->point_2.y()));
}

void ObjectRect::setPosPoint2(QPointF point, normalization_struct norm_params)
{
    QPointF point_denormalized = util::denormalize(point, norm_params);
    QPointF point1_denormalized = util::denormalize(this->point_1, norm_params);

    int calc_w = (point_denormalized.x() - point1_denormalized.x());
    int calc_h = (point_denormalized.y() - point1_denormalized.y());

    this->resize(calc_w, calc_h);

    this->point_2 = point;

    this->point_3 = (QPointF(this->point_2.x(), this->point_1.y()));
    this->point_4 = (QPointF(this->point_1.x(), this->point_2.y()));
}

void ObjectRect::moveRect(QPointF point, normalization_struct norm_params, QPointF offset)
{

    // Center point by clicking offset
    QPointF centered_point(
                    point.x() - offset.x(),
                    point.y() - offset.y()
                );

    // Move object point 1
    this->setPosPoint1(centered_point, norm_params);

    // Denormalize destination point 2
    QPointF point1_denormalized = util::denormalize(this->point_1, norm_params, 1);

    // Compute destination point 2 location
    QPointF size(
                point1_denormalized.x() + (this->width() / norm_params.scale_factor),
                point1_denormalized.y() + (this->height() / norm_params.scale_factor)
            );

    // Re-normalize modified destination point 2
    QPointF size_norm = util::normalize(size, norm_params);

    // Update position of point 2
    this->point_2 = size_norm;

}

void ObjectRect::update(normalization_struct norm_params)
{
    this->setPosPoint1(this->point_1, norm_params);
    this->setPosPoint2(this->point_2, norm_params);
}

void ObjectRect::setPos(QPointF p1, QPointF p2, normalization_struct norm_params, int mode)
{
    switch(mode)
    {
        case RectMoveType::All:
            this->setPosPoint1(p1, norm_params);
            this->setPosPoint2(p2, norm_params);
            break;
        case RectMoveType::Only_Point1:
            this->setPosPoint1(p1, norm_params);
            break;
        case RectMoveType::Only_Point2:
            this->setPosPoint2(p2, norm_params);
            break;
    }
}

ObjectRect::~ObjectRect()
{
    delete ui;
}
