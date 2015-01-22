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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* Includes */
#include <QMainWindow>
#include <QDesktopWidget>
#include <QThread>
#include <QLabel>
#include <QPalette>
#include <iostream>

#include "panoramaviewer.h"
#include "batchview.h"

/* Default class container */
namespace Ui {
class MainWindow;
}

/* Main class */
class MainWindow : public QMainWindow
{
    Q_OBJECT

/* Public functions / variables */
public:

    /* Constructor */
    explicit MainWindow(QWidget *parent, QString sourceImagePath, QString detectorYMLPath, QString destinationYMLPath);

    /* Destructor */
    ~MainWindow();

    /* Main window options container */
    struct Options_Container {
        QString sourceImagePath;
        QString detectorYMLPath;
        QString destinationYMLPath;
    } options;

/* Private slots */
private slots:

    /* Initial setup function */
    void initializeValidator(QString sourceImagePath, QString detectorYMLPath, QString destinationYMLPath);

    /* UI components signal functions */
    void on_untypedButton_clicked();
    void on_facesButton_clicked();
    void on_platesButton_clicked();
    void on_preInvalidatedButton_clicked();
    void on_horizontalSlider_sliderMoved(int position);
    void on_toBlurButton_clicked();
    void on_allVisRadio_clicked();
    void on_autoVisRadio_clicked();
    void on_manualVisRadio_clicked();
    void on_inCreationVisRadio_clicked();

    /* Local UI actions */
    void refreshLabels();
    void updateScaleSlider(int value);
    void onESC();

/* Private functions / variables */
private:

    /* Main UI container */
    Ui::MainWindow *ui;

    /* Main PanoramaViewer container */
    PanoramaViewer* pano;

    /* System colors container */
    QString good_color;
    QString warn_color;

/* Protected elements */
protected:

    /* Window close event */
    void closeEvent (QCloseEvent *event);

};

#endif // MAINWINDOW_H
