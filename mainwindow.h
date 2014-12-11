#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void refreshLabels();

    void on_untypedButton_clicked();

    void on_facesButton_clicked();

    void on_platesButton_clicked();

    void on_preInvalidatedButton_clicked();

    void on_pushButton_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_pushButton_2_clicked();

private:

    Ui::MainWindow *ui;
    PanoramaViewer* pano;

    QString good_color;
    QString warn_color;

    QElapsedTimer* timer;
};

#endif // MAINWINDOW_H
