#include "objectitem.h"
#include "ui_objectitem.h"
#include "editview.h"
#include <QDebug>

ObjectItem::ObjectItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectItem)
{
    ui->setupUi(this);

    this->ui->imageLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    this->setSelected(false);

    this->manualStatus = "None";
    this->autoStatus = "None";

    this->border_size = 4;

    this->setSize(QSize(128, 128));

    this->toBeRemoved = false;
}

ObjectItem::ObjectItem(QWidget *parent, PanoramaViewer* pano, ObjectRect* rect) :
    QWidget(parent),
    ui(new Ui::ObjectItem)
{
    ui->setupUi(this);

    this->ui->imageLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    this->setSelected(false);

    this->manualStatus = "None";
    this->autoStatus = "None";

    this->border_size = 4;

    this->setSize(QSize(128, 128));

    this->setPano( pano );
    this->setRect( rect );
    this->parent_elements = parent_elements;

    this->toBeRemoved = false;
}

void ObjectItem::remove(bool value)
{
    this->toBeRemoved = value;
    this->setVisible( !value );
    this->setEnabled( !value );
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

    int corr_pixmap_w = pixmap.width();
    int corr_pixmap_h = pixmap.height();

    if(corr_pixmap_w >= (this->width() - this->border_size))
    {
        corr_pixmap_w = corr_pixmap_w - (this->border_size * 2);
    }

    if(corr_pixmap_h >= (this->height() - this->border_size))
    {
        corr_pixmap_h = corr_pixmap_h - (this->border_size * 2);
    }

    this->ui->validFrame->resize(corr_pixmap_w, corr_pixmap_h);
    float pos_x = (this->width() - corr_pixmap_w) / 2;
    float pos_y = (this->height() - corr_pixmap_h) / 2;
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

    int corr_pixmap_w = pixmap.width();
    int corr_pixmap_h = pixmap.height();

    if(corr_pixmap_w >= (this->width() - this->border_size))
    {
        corr_pixmap_w = corr_pixmap_w - (this->border_size * 2);
    }

    if(corr_pixmap_h >= (this->height() - this->border_size))
    {
        corr_pixmap_h = corr_pixmap_h - (this->border_size * 2);
    }

    this->ui->validFrame->resize(corr_pixmap_w, corr_pixmap_h);
    float pos_x = (this->width() - corr_pixmap_w) / 2;
    float pos_y = (this->height() - corr_pixmap_h) / 2;
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
    this->ui->typeLabel->move(0 + this->border_size, 0 + this->border_size);

    this->ui->blurLabel->move((size.width() - this->ui->blurLabel->width()) - this->border_size, (size.height() - this->ui->blurLabel->height()) - this->border_size);

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
    case ObjectType::ToBlur:
        this->ui->typeLabel->setPixmap( QPixmap(":/resources/icons/Blur.png") );
        break;
    }

    this->rect->setType( type );
}

void ObjectItem::setRectType(int type)
{
    this->recttype = type;

    switch(type)
    {
    case ObjectItemRectType::Valid:
        this->setStyleSheet(
            "QLabel#imageLabel{"
            "border: " + QString::number(this->border_size) + "px solid rgb(0, 255, 0);"
            "}"
        );
        break;
    case ObjectItemRectType::Invalid:
        this->setStyleSheet(
            "QLabel#imageLabel{"
            "border: " + QString::number(this->border_size) + "px solid rgb(255, 0, 0);"
            "}"
        );
        break;
    case ObjectItemRectType::Manual:
        this->setStyleSheet(
            "QLabel#imageLabel{"
            "border: " + QString::number(this->border_size) + "px solid rgb(0, 255, 255);"
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

    this->rect->setBlurred( value );
}

void ObjectItem::setValidState(int state)
{
    this->validstate = state;

    switch(state)
    {
    case ObjectRectState::Valid:
        this->ui->validFrame->setStyleSheet("background-color: rgba(0, 255, 0, 50);");
        this->valid = true;
        break;
    case ObjectRectState::Invalid:
        this->ui->validFrame->setStyleSheet("background-color: rgba(255, 0, 0, 50);");
        this->valid = false;
        break;
    case ObjectRectState::None:
        this->ui->validFrame->setStyleSheet("background-color: rgba(0, 0, 0, 50);");
        this->valid = false;
        break;
    case ObjectRectState::ToBlur:
        this->ui->validFrame->setStyleSheet("background-color: rgba(255, 255, 0, 50);");
        this->valid = false;
        break;
    }

    this->rect->setObjectRectState( state );
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

void ObjectItem::setRect(ObjectRect *src_rect)
{
    this->rect = src_rect->copy();

    this->setId(src_rect->getId());
    this->setImage(this->parent_pano->cropObject(src_rect));
    this->setType(src_rect->getType());
    this->setBlurred(src_rect->isBlurred());
    this->setValidState(src_rect->getObjectRectState());
    this->setManualStatus(src_rect->getManualStatus());
    this->setAutomaticStatus(src_rect->getAutomaticStatus());

    if(src_rect->getAutomaticStatus() != "None")
    {
        if(src_rect->getAutomaticStatus() == "Valid")
        {
            this->setRectType(ObjectItemRectType::Valid);
        } else {
            this->setRectType(ObjectItemRectType::Invalid);
        }
    } else {
        this->setRectType(ObjectItemRectType::Manual);
    }
}

void ObjectItem::setPano(PanoramaViewer *pano)
{
    this->parent_pano = pano;
}

void ObjectItem::setAutomaticStatus(QString value)
{
    this->autoStatus = value;
}

void ObjectItem::setManualStatus(QString value)
{
    this->manualStatus = value;
}


void ObjectItem::mousePressEvent(QMouseEvent* event) {

    // Check presence of left click
    if(event->buttons() & Qt::LeftButton)
    {
        this->setSelected(!this->selected);
    }
}


void ObjectItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    // Check presence of right click
    if(event->buttons() & Qt::RightButton)
    {
        EditView* w = new EditView(this->parent_pano, this->rect, this, EditMode::Single);
        w->setAttribute( Qt::WA_DeleteOnClose );
        w->show();
    }
}

ObjectItem::~ObjectItem()
{
    delete ui;
}
