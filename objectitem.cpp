#include "objectitem.h"
#include "ui_objectitem.h"

ObjectItem::ObjectItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectItem)
{
    ui->setupUi(this);

    this->ui->imageLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

bool ObjectItem::setImage(QImage image)
{
    QPixmap pixmap = QPixmap::fromImage(image);

    if (pixmap.isNull()) return false;

    int source_w = pixmap.width();
    int source_h = pixmap.height();

    if(source_w < this->maximumWidth())
        source_w = this->maximumWidth();

    if(source_h < this->maximumHeight())
        source_h = this->maximumHeight();

    int w = std::min(source_h, this->maximumWidth());
    int h = std::min(source_w, this->maximumHeight());

    pixmap = pixmap.scaled(QSize(w, h), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->ui->imageLabel->setPixmap(pixmap);

    return true;
}

bool ObjectItem::LoadImage(QString path)
{
    QPixmap pixmap(path);
    if (pixmap.isNull()) return false;

    int source_w = pixmap.width();
    int source_h = pixmap.height();

    if(source_w < this->maximumWidth())
        source_w = this->maximumWidth();

    if(source_h < this->maximumHeight())
        source_h = this->maximumHeight();

    int w = std::min(source_h, this->maximumWidth());
    int h = std::min(source_w, this->maximumHeight());

    pixmap = pixmap.scaled(QSize(w, h), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->ui->imageLabel->setPixmap(pixmap);

    return true;
}

void ObjectItem::setType(int type)
{
    switch(type)
    {
        case ObjectType::Face:
            this->ui->typeLabel->setPixmap( QPixmap(":/resources/icons/Face.png") );
            break;
    }
}

void ObjectItem::setBlurred(bool value)
{
    if(value)
    {
        this->ui->blurLabel->setPixmap( QPixmap(":/resources/icons/Blur.png") );
    } else {
        this->ui->blurLabel->setPixmap( QPixmap() );
    }
}

void ObjectItem::setSelected(bool value)
{
    if(value)
    {
        this->ui->selectFrame->setStyleSheet("QFrame#selectFrame {background-color :  rgba(255, 255, 0, 50);}");
        this->selected = true;
    } else {
        this->ui->selectFrame->setStyleSheet("QFrame#selectFrame {background-color :  rgba(0, 0, 0, 0);}");
        this->selected = false;
    }
}

void ObjectItem::mousePressEvent(QMouseEvent* ){
    this->setSelected(!this->selected);
}

ObjectItem::~ObjectItem()
{
    delete ui;
}
