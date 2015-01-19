#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>

#include <QMainWindow>
#include <QDesktopWidget>
#include <QThread>
#include <QLabel>
#include <QPalette>

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
