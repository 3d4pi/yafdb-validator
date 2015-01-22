/*
 * yafdb-validate - Yafdb validation tool
 *
 * Copyright (c) 2014-2015 FOXEL SA - http://foxel.ch
 * Please read <http://foxel.ch/license> for more information.
 *
 *
 * Author(s):
 *
 *      Kevin Velickovic <k.velickovic@foxel.ch>
 *
 *
 * This file is part of the FOXEL project <http://foxel.ch>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Additional Terms:
 *
 *      You are required to preserve legal notices and author attributions in
 *      that material or in the Appropriate Legal Notices displayed by works
 *      containing it.
 *
 *      You are required to attribute the work as explained in the "Usage and
 *      Attribution" section of <http://foxel.ch/license>.
 */

/* Includes */
#include "objectitem.h"
#include "ui_objectitem.h"
#include "editview.h"

/* Constructor 1 */
ObjectItem::ObjectItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectItem)
{
    ui->setupUi(this);

    /* Configure default tile size */
    this->setSize(QSize(128, 128));

    /* Center image alignement */
    this->ui->imageLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    /* Default values initialisation */
    this->manualStatus = "None";
    this->autoStatus = "None";
    this->border_size = 4;
    this->needs_removal = false;

    /* Load blur label image */
    this->ui->blurLabel->setPixmap( QPixmap(":/resources/icons/Blur.png") );

    /* Set as unselected by default */
    this->setSelected(false);
}

/* Constructor 2 */
ObjectItem::ObjectItem(QWidget *parent, PanoramaViewer* pano, ObjectRect* rect) :
    QWidget(parent),
    ui(new Ui::ObjectItem)
{
    ui->setupUi(this);

    /* Configure default tile size */
    this->setSize(QSize(128, 128));

    /* Center image alignement */
    this->ui->imageLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    /* Default values initialisation */
    this->manualStatus = "None";
    this->autoStatus = "None";
    this->border_size = 4;
    this->needs_removal = false;

    /* Set as unselected by default */
    this->setSelected(false);

    /* Load blur label image */
    this->ui->blurLabel->setPixmap( QPixmap(":/resources/icons/Blur.png") );

    /* Assign parent PanoramaViewer */
    this->setPano( pano );

    /* Assign parent ObjectRect */
    this->setParentRect( rect );
}

/* Destructor */
ObjectItem::~ObjectItem()
{
    /* Delete copied rect */
    delete this->parent_rect_copy;

    /* Delete main UI */
    delete ui;
}

/* Set source image */
bool ObjectItem::setImage(QImage image)
{
    /* Assign value */
    this->image = image;

    /* Convert source QImage to QPixmap */
    QPixmap pixmap = QPixmap::fromImage(image);

    /* Exit if pixmap is null */
    if (pixmap.isNull()) return false;

    /* Retrieve width/height of source image */
    int source_w = pixmap.width();
    int source_h = pixmap.height();

    /* Clamp to maximum width */
    if(source_w < this->maximumWidth())
        source_w = this->maximumWidth();

    /* Clamp to maximum height */
    if(source_h < this->maximumHeight())
        source_h = this->maximumHeight();

    /* Get the smallest possible sizes */
    int min_w = std::min(source_w, this->maximumWidth());
    int min_h = std::min(source_h, this->maximumHeight());

    /* Scale pixmap to size */
    pixmap = pixmap.scaled(QSize(min_w, min_h), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    /* Display tile */
    this->ui->imageLabel->setPixmap(pixmap);

    /* Variables for contour/frame sizes correction */
    int corr_pixmap_w = pixmap.width();
    int corr_pixmap_h = pixmap.height();

    /* Clamp border width to fit perfectly */
    if(corr_pixmap_w >= (this->width() - this->border_size))
    {
        corr_pixmap_w = corr_pixmap_w - (this->border_size * 2);
    }

    /* Clamp border height to fit perfectly */
    if(corr_pixmap_h >= (this->height() - this->border_size))
    {
        corr_pixmap_h = corr_pixmap_h - (this->border_size * 2);
    }

    /* Resize valid frame */
    this->ui->validFrame->resize(corr_pixmap_w, corr_pixmap_h);

    /* Move valid frame */
    float pos_x = (this->width() - corr_pixmap_w) / 2;
    float pos_y = (this->height() - corr_pixmap_h) / 2;
    this->ui->validFrame->move(pos_x, pos_y);

    /* Success */
    return true;
}

/* ID setter */
void ObjectItem::setId(int id)
{
    /* Assign value */
    this->id = id;
}

/* ID getter */
int ObjectItem::getId()
{
    /* Return value */
    return this->id;
}

/* Function to resize tile */
void ObjectItem::setSize(QSize size)
{
    /* Assign minimal/maximal sizes to ObjectItem */
    this->setMinimumSize(size);
    this->setMaximumSize(size);

    /* Assign minimal/maximal sizes to image */
    this->ui->imageLabel->setMinimumSize(size);
    this->ui->imageLabel->setMaximumSize(size);

    /* Assign minimal/maximal sizes to selection frame */
    this->ui->selectFrame->setMinimumSize(size);
    this->ui->selectFrame->setMaximumSize(size);

    /* Label size corrections */
    int label_w = size.width() / 8;
    int label_h = size.width() / 8;

    /* Assign minimal/maximal sizes to blur label */
    this->ui->blurLabel->setMinimumSize(label_w, label_h);
    this->ui->blurLabel->setMaximumSize(label_w, label_h);

    /* Resize blur label */
    this->ui->blurLabel->resize(label_w, label_h);

    /* Assign minimal/maximal sizes to type label */
    this->ui->typeLabel->setMinimumSize(label_w, label_h);
    this->ui->typeLabel->setMaximumSize(label_w, label_h);

    /* Resize type label */
    this->ui->typeLabel->resize(label_w, label_h);

    /* Move type label */
    this->ui->typeLabel->move(0 + this->border_size, 0 + this->border_size);

    /* Move blur label */
    this->ui->blurLabel->move((size.width() - this->ui->blurLabel->width()) - this->border_size,
                              (size.height() - this->ui->blurLabel->height()) - this->border_size);

    /* Update tile image */
    this->setImage(this->image);
}

/* Function to set item type */
void ObjectItem::setItemType(int type)
{
    /* Assign value */
    this->item_type = type;

    /* Assign proper type label image */
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

    /* Update parent rect type */
    this->parent_rect_copy->setObjectType( type );
}

/* Item type getter */
int ObjectItem::getItemType()
{
    /* Return value */
    return this->item_type;
}

/* Function to set item sub-type */
void ObjectItem::setItemSubType(int sub_type)
{
    /* Assign value */
    this->item_sub_type = sub_type;

    /* Update parent rect sub-type */
    this->parent_rect_copy->setObjectSubType( sub_type );
}

/* Item sub-type getter */
int ObjectItem::getItemSubType()
{
    /* Return value */
    return this->item_sub_type;
}

/* Function to mark object as blurred or not */
void ObjectItem::setBlurred(bool value)
{
    /* Assign value */
    this->blurred = value;

    /* Check input value */
    if(value)
    {
        /* Show blur label */
        this->ui->blurLabel->show();
    } else {

        /* Hide blur label */
        this->ui->blurLabel->hide();
    }

    /* Update parent rect blurred flag */
    this->parent_rect_copy->setBlurred( value );
}

/* Blurred flag getter */
bool ObjectItem::isBlurred()
{
    /* Return value */
    return this->blurred;
}

void ObjectItem::setSelected(bool value)
{
    /* Assign value */
    this->selected = value;

    /* Check input value */
    if(value)
    {
        /* Apply selected color (yellow) */
        this->ui->selectFrame->setStyleSheet("QFrame#selectFrame {background-color :  rgba(255, 255, 0, 50);}");
    } else {

        /* Apply no colors */
        this->ui->selectFrame->setStyleSheet("QFrame#selectFrame {background-color :  rgba(0, 0, 0, 0);}");
    }
}

/* Selected flag getter */
bool ObjectItem::isSelected()
{
    /* Return value */
    return this->selected;
}

/* Function to assign a parent ObjectRect to item */
void ObjectItem::setParentRect(ObjectRect *src_rect)
{
    /* Copy input ObjectRect to local parent_rect_copy variable */
    this->parent_rect_copy = src_rect->copy();

    /* Update item using parent rect values */
    this->setId( src_rect->getId() );
    this->setImage( this->parent_pano->cropObject( src_rect ) );
    this->setItemType( src_rect->getObjectType() );
    this->setItemSubType( src_rect->getObjectSubType() );
    this->setBlurred( src_rect->isBlurred() );
    this->setItemManualState( src_rect->getObjectManualState() );
    this->setManualStatus( src_rect->getManualStatus() );
    this->setAutomaticStatus( src_rect->getAutomaticStatus() );

    /* If object has an automatic status */
    if(src_rect->getAutomaticStatus() != "None")
    {
        /* If automatic status is valid */
        if(src_rect->getAutomaticStatus() == "Valid")
        {
            /* Set automatic state to valid */
            this->setItemAutomaticState(ObjectAutomaticState::Valid);
        } else {

            /* Set automatic state to invalid */
            this->setItemAutomaticState(ObjectAutomaticState::Invalid);
        }
    } else {

        /* Set automatic state to manual */
        this->setItemAutomaticState(ObjectAutomaticState::Manual);
    }
}

/* Function to get parent ObjectRect */
ObjectRect* ObjectItem::getParentRect()
{
    /* Return value */
    return this->parent_rect_copy;
}

/* Function to assign parent PanoramaViewer */
void ObjectItem::setPano(PanoramaViewer *pano)
{
    /* Assign value */
    this->parent_pano = pano;
}

/* Function to remove item */
void ObjectItem::remove(bool value)
{
    /* Assign flag */
    this->needs_removal = value;

    /* Hide item */
    this->setVisible( !value );

    /* Disable item */
    this->setEnabled( !value );
}

/* Function to set item manual state */
void ObjectItem::setItemManualState(int state)
{
    /* Assign value */
    this->manual_state = state;

    /* Assign proper validation frame color based on input state */
    switch(state)
    {
    case ObjectManualState::Valid:

        /* Set validation frame color to green */
        this->ui->validFrame->setStyleSheet("background-color: rgba(0, 255, 0, 50);");

        /* Mark object as valid */
        this->valid = true;
        break;
    case ObjectManualState::Invalid:

        /* Set validation frame color to red */
        this->ui->validFrame->setStyleSheet("background-color: rgba(255, 0, 0, 50);");

        /* Mark object as invalid */
        this->valid = false;
        break;
    case ObjectManualState::None:

        /* Set validation frame color to none */
        this->ui->validFrame->setStyleSheet("background-color: rgba(0, 0, 0, 50);");

        /* Mark object as invalid */
        this->valid = false;
        break;
    case ObjectManualState::ToBlur:

        /* Set validation frame color to yellow */
        this->ui->validFrame->setStyleSheet("background-color: rgba(255, 255, 0, 50);");

        /* Mark object as invalid */
        this->valid = false;
        break;
    }

    /* Update parent rect manual state */
    this->parent_rect_copy->setObjectManualState( state );
}

/* Function to set item automatic state */
void ObjectItem::setItemAutomaticState(int state)
{
    /* Assign value */
    this->automatic_state = state;

    /* Assign proper border color based on input state */
    switch(state)
    {
    case ObjectAutomaticState::Valid:

        /* Set border color to green */
        this->setStyleSheet(
            "QLabel#imageLabel{"
            "border: " + QString::number(this->border_size) + "px solid rgb(0, 255, 0);"
            "}"
        );
        break;
    case ObjectAutomaticState::Invalid:

         /* Set border color to red */
        this->setStyleSheet(
            "QLabel#imageLabel{"
            "border: " + QString::number(this->border_size) + "px solid rgb(255, 0, 0);"
            "}"
        );
        break;
    case ObjectAutomaticState::Manual:

         /* Set border color to light blue */
        this->setStyleSheet(
            "QLabel#imageLabel{"
            "border: " + QString::number(this->border_size) + "px solid rgb(0, 255, 255);"
            "}"
        );
        break;
    }
}

/* Function to set automatic status value */
void ObjectItem::setAutomaticStatus(QString value)
{
    /* Assign value */
    this->autoStatus = value;

    /* Update parent rect automatic status with new value */
    this->parent_rect_copy->setAutomaticStatus( value );
}

/* Automatic status getter */
QString ObjectItem::getAutomaticStatus()
{
    /* Return value */
    return this->autoStatus;
}

/* Function to set manual status value */
void ObjectItem::setManualStatus(QString value)
{
    /* Assign value */
    this->manualStatus = value;

    /* Update parent rect manual status with new value */
    this->parent_rect_copy->setManualStatus( value );
}

/* Manual status getter */
QString ObjectItem::getManualStatus()
{
    /* Return value */
    return this->manualStatus;
}

/* Function to determine if object is valid */
bool ObjectItem::isValid()
{
    /* Return value */
    return this->valid;
}

/* Function to determine if object is marked for removal */
bool ObjectItem::toBeRemoved()
{
    /* Return value */
    return this->needs_removal;
}

/* Mouse press event */
void ObjectItem::mousePressEvent(QMouseEvent* event) {

    /* Check presence of left click */
    if(event->buttons() & Qt::LeftButton)
    {
        /* Toggle item selected flag */
        this->setSelected(!this->selected);
    }
}

/* Mouse double click event */
void ObjectItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    /* Check presence of right click */
    if(event->buttons() & Qt::RightButton)
    {
        /* Create and show a new edition window */
        EditView* w = new EditView(this->parent_pano, this->parent_rect_copy, this->parent_pano->image_info, this, EditMode::Single);
        w->setAttribute( Qt::WA_DeleteOnClose );
        w->show();
    }
}
