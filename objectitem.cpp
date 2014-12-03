#include "objectitem.h"
#include "ui_objectitem.h"
#include <QDebug>

ObjectItem::ObjectItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectItem)
{
    ui->setupUi(this);

    this->ui->imageLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    this->setSize(QSize(128, 128));
    this->setSelected(false);

    this->manualStatus = "None";
    this->autoStatus = "None";
}

void ObjectItem::setId(int id)
{
    this->id = id;
}

bool ObjectItem::setImage(QImage image)
{
    this->image = image;

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

    this->ui->validFrame->resize(pixmap.width(), pixmap.height());
    float pos_x = (this->width() - pixmap.width()) / 2;
    float pos_y = (this->height() - pixmap.height()) / 2;
    this->ui->validFrame->move(pos_x, pos_y);

    return true;
}

bool ObjectItem::LoadImage(QString path)
{
    this->image = QImage(path);

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

    this->ui->validFrame->resize(pixmap.width(), pixmap.height());
    float pos_x = (this->width() - pixmap.width()) / 2;
    float pos_y = (this->height() - pixmap.height()) / 2;
    this->ui->validFrame->move(pos_x, pos_y);

    this->ui->imageLabel->setPixmap(pixmap);

    return true;
}

void ObjectItem::setSize(QSize size)
{
    this->setMinimumSize(size);
    this->setMaximumSize(size);

    this->ui->imageLabel->setMinimumSize(size);
    this->ui->imageLabel->setMaximumSize(size);

    this->ui->selectFrame->setMinimumSize(size);
    this->ui->selectFrame->setMaximumSize(size);

    int label_w = size.width() / 8;
    int label_h = size.width() / 8;

    this->ui->blurLabel->setMinimumSize(label_w, label_h);
    this->ui->blurLabel->setMaximumSize(label_w, label_h);
    this->ui->blurLabel->resize(label_w, label_h);

    this->ui->typeLabel->setMinimumSize(label_w, label_h);
    this->ui->typeLabel->setMaximumSize(label_w, label_h);
    this->ui->typeLabel->resize(label_w, label_h);

    this->ui->typeLabel->move(0, 0);
    this->ui->blurLabel->move((size.width() - this->ui->blurLabel->width()), (size.height() - this->ui->blurLabel->height()));

    this->setImage(this->image);
}

void ObjectItem::setType(int type)
{
    this->type = type;

    switch(type)
    {
        case ObjectType::Face:
            this->ui->typeLabel->setPixmap( QPixmap(":/resources/icons/Face.png") );
            break;
        case ObjectType::NumberPlate:
            this->ui->typeLabel->setPixmap( QPixmap(":/resources/icons/Plate.png") );
            break;
        case ObjectType::BlurOnly:
            this->ui->typeLabel->setPixmap( QPixmap(":/resources/icons/Blur.png") );
            break;
    }
}

void ObjectItem::setRectType(int type)
{
    this->recttype = type;

    switch(type)
    {
        case ObjectItemRectType::Valid:
            this->setStyleSheet(
                "QLabel#imageLabel{"
                "border: 4px solid rgb(0, 255, 0);"
                "}"
            );
            break;
        case ObjectItemRectType::Invalid:
            this->setStyleSheet(
                "QLabel#imageLabel{"
                "border: 4px solid rgb(255, 0, 0);"
                "}"
            );
            break;
        case ObjectItemRectType::Manual:
            this->setStyleSheet(
                "QLabel#imageLabel{"
                "border: 4px solid rgb(0, 255, 255);"
                "}"
            );
            break;
    }
}

void ObjectItem::setBlurred(bool value)
{
    this->blurred = value;

    if(value)
    {
        this->ui->blurLabel->setPixmap( QPixmap(":/resources/icons/Blur.png") );
    } else {
        this->ui->blurLabel->setPixmap( QPixmap() );
    }
}

void ObjectItem::setValidState(int state)
{
    this->validstate = state;

    switch(state)
    {
        case ObjectValidState::Valid:
            this->ui->validFrame->setStyleSheet("background-color: rgba(0, 255, 0, 50);");
            this->valid = true;
            break;
        case ObjectValidState::Invalid:
            this->ui->validFrame->setStyleSheet("background-color: rgba(255, 0, 0, 50);");
            this->valid = false;
            break;
        case ObjectValidState::None:
            this->ui->validFrame->setStyleSheet("background-color: rgba(0, 0, 0, 50);");
            this->valid = false;
            break;
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

void ObjectItem::setAutomaticStatus(QString value)
{
    this->autoStatus = value;
}

void ObjectItem::setManualStatus(QString value)
{
    this->manualStatus = value;
}

void ObjectItem::mousePressEvent(QMouseEvent* ){
    this->setSelected(!this->selected);
}

ObjectItem::~ObjectItem()
{
    delete ui;
}
