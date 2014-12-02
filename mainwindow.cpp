#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>
#include "ymlparser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Remove margins
    this->setContentsMargins(-5, -5, -5, -5);
    //this->centralWidget()->layout()->setContentsMargins(50,50,50,50);

    // Center window on screen
    this->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            qApp->desktop()->availableGeometry()
    ));

    // Start window maximized
    this->showMaximized();

    // Create panorama viewer
    this->pano = new PanoramaViewer(this);

    // Add panorama viewer to current window
    this->ui->gridLayout->addWidget(this->pano);

    // Determine best multi-threading setup
    int threads_count = QThread::idealThreadCount();

    // Configure panorama viewer
    this->pano->setup(
        0.6,   // Image scale factor
        20.0,  // Minimum zoom
        100.0, // Maximum zoom
        100.0, // Default zoom level
        threads_count // Number of threads
    );

    //this->ui->horizontalSlider->setValue(this->pano->scale_factor * 100);

    // Load input image
    this->pano->loadImage("/home/f0x/Bureau/Photo_RMLL_2014_panoramique,_WE_grand_public.jpeg");

}

void MainWindow::refreshLabels()
{
    int untyped = 0;

    foreach(ObjectRect* rect, this->pano->rect_list)
    {
        switch(rect->type)
        {
            case RectType::None:
                untyped++;
                break;
        }
    }

    this->ui->untypedLabel->setText("Untyped items: " + QString::number(untyped));
}

MainWindow::~MainWindow()
{
    //delete this->pano;
    delete ui;
}

/*
void MainWindow::on_pushButton_clicked()
{
    foreach(ObjectRect* rect, this->pano->rect_list)
    {
        delete rect;
        this->pano->rect_list.removeOne(rect);
    }

}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    this->pano->scale_factor = (position / 100.0);
    this->pano->render();
}

void MainWindow::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save image"), "", tr("Image Files (*.png *.jpg *.bmp)"));

    if(fileName.length() > 0)
    {
        this->pano->dest_image.save(fileName);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    int idx = 0;

    foreach(ObjectRect* rect, this->pano->rect_list)
    {

        QString path = "/home/f0x/Bureau/crops/crop_" + QString::number(idx) + ".png";
        this->pano->cropObject(rect).save(path, "PNG");

        idx++;
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    BatchView* w = new BatchView(this, this->pano);
    w->show();
}
*/

void MainWindow::on_untypedButton_clicked()
{
    BatchView* w = new BatchView(this, this->pano);
    w->show();
}
