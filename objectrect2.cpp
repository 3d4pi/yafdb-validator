#include "objectrect2.h"

ObjectRect2::ObjectRect2()
{
    // Append default points
    this->points.append(QPointF(0.0, 0.0));
    this->points.append(QPointF(0.0, 0.0));
    this->points.append(QPointF(0.0, 0.0));
    this->points.append(QPointF(0.0, 0.0));

    // Default pen setup
    this->pen = new QPen(QColor(0, 255, 255, 255), 4);
    this->brush = new QBrush(QColor(0, 255, 0, 50), Qt::SolidPattern);

    this->setPen( * this->pen );
    this->setBrush( * this->brush );

    // Cal rendering procedure
    this->render();
}

void ObjectRect2::setPoint1(QPointF point)
{
    this->points[0] = point;
    this->polygon = QPolygonF( this->points );
    this->setPolygon( this->polygon );

    // Cal rendering procedure
    this->render();
}

void ObjectRect2::setPoint2(QPointF point)
{
    this->points[1] = point;
    this->polygon = QPolygonF( this->points );
    this->setPolygon( this->polygon );

    // Cal rendering procedure
    this->render();
}

void ObjectRect2::setPoint3(QPointF point)
{
    this->points[2] = point;
    this->polygon = QPolygonF( this->points );
    this->setPolygon( this->polygon );

    // Cal rendering procedure
    this->render();
}

void ObjectRect2::setPoint4(QPointF point)
{
    this->points[3] = point;
    this->polygon = QPolygonF( this->points );
    this->setPolygon( this->polygon );

    // Cal rendering procedure
    this->render();
}

QPointF ObjectRect2::getPoint1()
{
    return this->points[0];
}

QPointF ObjectRect2::getPoint2()
{
    return this->points[1];
}

QPointF ObjectRect2::getPoint3()
{
    return this->points[2];
}

QPointF ObjectRect2::getPoint4()
{
    return this->points[3];
}

QVector<QPointF> ObjectRect2::getPoints()
{
    return this->points;
}


void ObjectRect2::setPoints(QPointF p1, QPointF p2, QPointF p3, QPointF p4)
{
    this->setPoint1( p1 );
    this->setPoint2( p2 );
    this->setPoint3( p3 );
    this->setPoint4( p4 );

    // Cal rendering procedure
    this->render();
}

void ObjectRect2::setSize(QSizeF size)
{
    QPointF new_p1 = this->points[0];
    QPointF new_p2 = this->points[1];
    QPointF new_p3 = this->points[2];
    QPointF new_p4 = this->points[3];

    // Width
    new_p2.setX( new_p1.x() + size.width() );
    new_p3.setX( new_p1.x() + size.width() );

    // Height
    new_p4.setY( new_p1.y() + size.height() );
    new_p3.setY( new_p1.y() + size.height() );

    // Update positions
    this->setPoints( new_p1, new_p2, new_p3, new_p4 );

    // Cal rendering procedure
    this->render();
}

QSizeF ObjectRect2::getSize()
{
    QPointF p1 = this->points[0];
    QPointF p2 = this->points[1];
    QPointF p4 = this->points[3];

    QSizeF size;

    size.setWidth( p2.x() -  p1.x());
    size.setHeight( p4.y() -  p1.y());

    return size;
}

void ObjectRect2::setObjectRectType(int type)
{
    switch(type)
    {
        case ObjectRectType::Manual:
            this->pen->setColor( QColor(0, 255, 255, 255) );
            break;
        case ObjectRectType::Valid:
            this->pen->setColor( QColor(0, 255, 0, 255) );
            break;
        case ObjectRectType::Invalid:
            this->pen->setColor( QColor(255, 0, 0, 255) );
            break;
    }

    // Update pen
    this->setPen( * this->pen );
}

void ObjectRect2::setObjectRectState(int state)
{
    switch(state)
    {
        case ObjectRectState::Valid:
            this->brush->setColor( QColor(0, 255, 0, 50) );
            break;
        case ObjectRectState::Invalid:
            this->brush->setColor( QColor(255, 0, 0, 50) );
            break;
    }

    // Update brush
    this->setBrush( * this->brush );
}

void ObjectRect2::render()
{
    qDebug() << "Render";
}
