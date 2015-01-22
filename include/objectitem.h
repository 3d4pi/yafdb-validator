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

#ifndef OBJECTITEM_H
#define OBJECTITEM_H

/* Includes */
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>
#include "objectrect.h"
#include "panoramaviewer.h"

namespace Ui {
class ObjectItem;
}

/* Main class */
class ObjectItem : public QWidget
{
    Q_OBJECT

/* Public functions / variables */
public:

    /* Constructors */
    explicit ObjectItem(QWidget *parent = 0);
    explicit ObjectItem(QWidget *parent, PanoramaViewer* pano, ObjectRect* rect);

    /* Destructor */
    ~ObjectItem();

    /* Set source image */
    bool setImage(QImage image);

    /* ID setter/getter */
    void setId(int id);
    int  getId();

    /* Function to change the tile size */
    void setSize(QSize size);

    /* Item type setter/getter */
    void setItemType(int type);
    int  getItemType();

    /* Item sub-type setter/getter */
    void setItemSubType(int sub_type);
    int  getItemSubType();

    /* Blurred flag setter/getter */
    void setBlurred(bool value);
    bool isBlurred();

    /* Selected flag setter/getter */
    void setSelected(bool value);
    bool isSelected();

    /* Function to assign a parent ObjectRect to item */
    void setParentRect(ObjectRect* rect);

    /* Function to get parent ObjectRect */
    ObjectRect* getParentRect();

    /* Function to assign parent PanoramaViewer */
    void setPano(PanoramaViewer* pano);

    /* Function to remove item */
    void remove(bool value);

    /* Manual state setter */
    void setItemManualState(int state);

    /* Automatic state setter */
    void setItemAutomaticState(int state);

    /* Automatic status setter/getter */
    void setAutomaticStatus(QString value);
    QString getAutomaticStatus();

    /* Manual status setter/getter */
    void setManualStatus(QString value);
    QString getManualStatus();

    /* Function to determine if object is valid */
    bool isValid();

    /* Function to determine if object is marked for removal */
    bool toBeRemoved();

/* Private functions / variables */
private:

    /* Main UI container */
    Ui::ObjectItem *ui;

    /* Item ID container */
    int id;

    /* Item type container */
    int item_type;

    /* Item sub-type container */
    int item_sub_type;

    /* Item automatic state container */
    int automatic_state;

    /* Item manual state container */
    int manual_state;

    /* Item border size container */
    int border_size;

    /* Selected status container */
    bool selected;

    /* Valid status container */
    bool valid;

    /* Blurred status container */
    bool blurred;

    /* "Tagged for removal ?" status container */
    bool needs_removal;

    /* Item tile image */
    QImage image;

    /* Manual status container */
    QString manualStatus;

    /* Automatic status container */
    QString autoStatus;

    /* Parent rect copy container */
    ObjectRect* parent_rect_copy;

    /* Parent PanoramaViewer container */
    PanoramaViewer* parent_pano;

/* Protected elements */
protected:

    /* Mouse press event */
    void mousePressEvent(QMouseEvent *ev);

    /* Mouse double click event */
    void mouseDoubleClickEvent ( QMouseEvent * event );

};

#endif // OBJECTITEM_H
