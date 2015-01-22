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

#ifndef EDITVIEW_H
#define EDITVIEW_H

/* Imports */
#include <QMainWindow>
#include <QDesktopWidget>
#include <QStandardItemModel>

#include "objectitem.h"
#include "g2g_point.h"
#include "objectrect.h"
#include "panoramaviewer.h"

/* Edit modes struct */
struct EditMode
{
    enum Type
    {
        /* Edit one object */
        Single = 1,

        /* Edit parent PanoramaViewer object directly */
        Scene = 2
    };
};

/* Default class container */
namespace Ui {
class EditView;
}

/* Main class */
class EditView : public QMainWindow
{
    Q_OBJECT

/* Public functions / variables */
public:

    /* Constructor */
    explicit EditView(QWidget *parent, ObjectRect* rect, image_info_struct image_info, ObjectItem* item, int mode = EditMode::Scene);

    /* Destructor */
    ~EditView();

/* Private slots */
private slots:

    /* UI components signal functions */
    void on_cancelButton_clicked();
    void on_deleteButton_clicked();
    void on_confirmButton_clicked();
    void on_validCheckBox_clicked();
    void on_blurCheckBox_clicked();

/* Private functions / variables */
private:

    /* Main UI container */
    Ui::EditView *ui;

    /* View mode container */
    int mode;

    /* Main PanoramaViewer container */
    PanoramaViewer* pano;

    /* Parent PanoramaViewer container */
    PanoramaViewer* pano_parent;

    /* Parent rect to be edited */
    ObjectRect* ref_rect;

    /* Parent object item */
    ObjectItem* item;

    /* Locat copy of rect for editing */
    ObjectRect* rect_copy;

    /* Function to merge rect to specified destination */
    void mergeEditedRect(ObjectRect* destination);

    /* Function to merge item to specified destination */
    void mergeEditedItem(ObjectItem* destination);

signals:

    /* Signal to refresh parent window status labels */
    void refreshLabels();

};

#endif // EDITVIEW_H
