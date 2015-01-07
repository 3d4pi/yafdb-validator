#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>

#include <QMainWindow>
#include <QDesktopWidget>
#include <QThread>
#include <QLabel>
#include <QPalette>
#include <QElapsedTimer>

#include "panoramaviewer.h"
#include "batchview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent, QString sourceImagePath, QString detectorYMLPath, QString destinationYMLPath);
    ~MainWindow();

    struct Options_Container {
        QString sourceImagePath;
        QString detectorYMLPath;
        QString destinationYMLPath;
    } options;

private slots:

    void onESC();

    void refreshLabels();
    void updateScaleSlider(int value);

    void on_untypedButton_clicked();

    void on_facesButton_clicked();

    void on_platesButton_clicked();

    void on_preInvalidatedButton_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_toBlurButton_clicked();

private:

    void closeEvent (QCloseEvent *event);

    Ui::MainWindow *ui;
    PanoramaViewer* pano;

    QString good_color;
    QString warn_color;

    QElapsedTimer* timer;

    QString output_yml;
};

#endif // MAINWINDOW_H
