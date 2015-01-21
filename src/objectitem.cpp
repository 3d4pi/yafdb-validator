#include "objectitem.h"
#include "ui_objectitem.h"
#include "editview.h"

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

    this->needs_removal = false;
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

    this->needs_removal = false;
}

/* Destructor */
ObjectItem::~ObjectItem()
{
    /* Delete copied rect */
    delete this->rect;

    /* Delete main UI */
    delete ui;
}

void ObjectItem::remove(bool value)
{
    this->needs_removal = value;
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

void ObjectItem::setItemType(int type)
{
    this->item_type = type;

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

    this->rect->setObjectType( type );
}

void ObjectItem::setItemSubType(int sub_type)
{
    this->item_sub_type = sub_type;
    this->rect->setObjectSubType( sub_type );
}

void ObjectItem::setItemAutomaticState(int state)
{
    this->automatic_state = state;

    switch(state)
    {
    case ObjectAutomaticState::Valid:
        this->setStyleSheet(
            "QLabel#imageLabel{"
            "border: " + QString::number(this->border_size) + "px solid rgb(0, 255, 0);"
            "}"
        );
        break;
    case ObjectAutomaticState::Invalid:
        this->setStyleSheet(
            "QLabel#imageLabel{"
            "border: " + QString::number(this->border_size) + "px solid rgb(255, 0, 0);"
            "}"
        );
        break;
    case ObjectAutomaticState::Manual:
        this->setStyleSheet(
            "QLabel#imageLabel{"
            "border: " + QString::number(this->border_size) + "px solid rgb(0, 255, 255);"
            "}"
        );
        break;
    }
}

void ObjectItem::setItemManualState(int state)
{
    this->manual_state = state;

    switch(state)
    {
    case ObjectManualState::Valid:
        this->ui->validFrame->setStyleSheet("background-color: rgba(0, 255, 0, 50);");
        this->valid = true;
        break;
    case ObjectManualState::Invalid:
        this->ui->validFrame->setStyleSheet("background-color: rgba(255, 0, 0, 50);");
        this->valid = false;
        break;
    case ObjectManualState::None:
        this->ui->validFrame->setStyleSheet("background-color: rgba(0, 0, 0, 50);");
        this->valid = false;
        break;
    case ObjectManualState::ToBlur:
        this->ui->validFrame->setStyleSheet("background-color: rgba(255, 255, 0, 50);");
        this->valid = false;
        break;
    }

    this->rect->setObjectManualState( state );
}

void ObjectItem::setAutomaticStatus(QString value)
{
    this->autoStatus = value;
    this->rect->setAutomaticStatus( value );
}

void ObjectItem::setManualStatus(QString value)
{
    this->manualStatus = value;
    this->rect->setManualStatus( value );
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
    this->setItemType(src_rect->getObjectType());
    this->setItemSubType( src_rect->getObjectSubType() );
    this->setBlurred(src_rect->isBlurred());
    this->setItemManualState(src_rect->getObjectManualState());
    this->setManualStatus(src_rect->getManualStatus());
    this->setAutomaticStatus(src_rect->getAutomaticStatus());

    if(src_rect->getAutomaticStatus() != "None")
    {
        if(src_rect->getAutomaticStatus() == "Valid")
        {
            this->setItemAutomaticState(ObjectAutomaticState::Valid);
        } else {
            this->setItemAutomaticState(ObjectAutomaticState::Invalid);
        }
    } else {
        this->setItemAutomaticState(ObjectAutomaticState::Manual);
    }
}

void ObjectItem::setPano(PanoramaViewer *pano)
{
    this->parent_pano = pano;
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
        EditView* w = new EditView(this->parent_pano, this->rect, this->parent_pano->image_info, this, EditMode::Single);
        w->setAttribute( Qt::WA_DeleteOnClose );
        w->show();
    }
}

bool ObjectItem::isValid()
{
    return this->valid;
}

bool ObjectItem::isBlurred()
{
    return this->blurred;
}

bool ObjectItem::isSelected()
{
    return this->selected;
}

bool ObjectItem::toBeRemoved()
{
    return this->needs_removal;
}

int ObjectItem::getId()
{
    return this->id;
}

int ObjectItem::getItemType()
{
    return this->item_type;
}

int ObjectItem::getItemSubType()
{
    return this->item_sub_type;
}

QString ObjectItem::getItemManualStatus()
{
    return this->manualStatus;
}

QString ObjectItem::getItemAutomaticStatus()
{
    return this->autoStatus;
}

ObjectRect* ObjectItem::getParentRect()
{
    return this->rect;
}
