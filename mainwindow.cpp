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

    // Determine good labels colors based on system theme
    QString good_color_string = "rgb(%1, %2, %3)";
    QColor  good_color_color = QApplication::palette().color(QPalette::Text);
    good_color_string = good_color_string.arg(good_color_color.red()).arg(good_color_color.green()).arg(good_color_color.blue());

    QString warn_color_string = "rgb(%1, %2, %3)";
    QColor  warn_color_color = QApplication::palette().color(QPalette::Highlight);
    warn_color_string = warn_color_string.arg(warn_color_color.red()).arg(warn_color_color.green()).arg(warn_color_color.blue());

    this->good_color = good_color_string;
    this->warn_color = warn_color_string;

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

    // Initialize labels
    emit refreshLabels();

}

void MainWindow::refreshLabels()
{
    int untyped = 0;

    int facecount = 0;
    int facesvalidated = 0;

    int numberplatescount = 0;
    int numberplatesvalidated = 0;

    int preinvalidatedcount = 0;
    int preinvalidatedvalidated = 0;

    foreach(ObjectRect* rect, this->pano->rect_list)
    {
        switch(rect->objecttype)
        {
            case ObjectType::None:
                untyped++;
                break;
            case ObjectType::Face:

                if(rect->autoStatus == "Valid" || rect->autoStatus == "None")
                    facecount++;

                if( (rect->autoStatus == "Valid" || rect->autoStatus == "None")
                        && rect->manualStatus != "None")
                {
                    facesvalidated++;
                }

                if(rect->autoStatus != "None" && rect->autoStatus != "Valid")
                {
                    preinvalidatedcount++;
                }
                if(rect->autoStatus != "None" && rect->autoStatus != "Valid" && rect->manualStatus != "None")
                {
                    preinvalidatedvalidated++;
                }

                break;
            case ObjectType::NumberPlate:

                numberplatescount++;

                if(rect->manualStatus != "None")
                {
                    numberplatesvalidated++;
                }

                break;
        }
    }

    if(untyped > 0)
    {
        this->ui->untypedLabel->setStyleSheet("QLabel {color: " + this->warn_color + "; }");
        this->ui->untypedButton->setEnabled(true);
    } else {
        this->ui->untypedLabel->setStyleSheet("QLabel {color: " + this->good_color + "; }");
        this->ui->untypedButton->setEnabled(false);
    }

    if(facesvalidated != facecount)
    {
       this->ui->facesLabel->setStyleSheet("QLabel {color: " + this->warn_color + "; }");
    } else {
        this->ui->facesLabel->setStyleSheet("QLabel {color: " + this->good_color + "; }");
    }

    if(numberplatesvalidated != numberplatescount)
    {
       this->ui->platesLabel->setStyleSheet("QLabel {color: " + this->warn_color + "; }");
    } else {
        this->ui->platesLabel->setStyleSheet("QLabel {color: " + this->good_color + "; }");
    }

    if(preinvalidatedvalidated != preinvalidatedcount)
    {
       this->ui->preInvalidatedLabel->setStyleSheet("QLabel {color: " + this->warn_color + "; }");
    } else {
        this->ui->preInvalidatedLabel->setStyleSheet("QLabel {color: " + this->good_color + "; }");
    }

    if(facecount <= 0)
    {
        //this->ui->facesButton->setEnabled(false);
    } else {

    }

    this->ui->untypedLabel->setText("Untyped items: " + QString::number(untyped));
    this->ui->facesLabel->setText("Faces: " + QString::number(facesvalidated) + "/" + QString::number(facecount));
    this->ui->platesLabel->setText("Number plates: " + QString::number(numberplatesvalidated) + "/" + QString::number(numberplatescount));
    this->ui->preInvalidatedLabel->setText("Pre-invalidated: " + QString::number(preinvalidatedvalidated) + "/" + QString::number(preinvalidatedcount));
}

MainWindow::~MainWindow()
{
    //delete this->pano;
    delete ui;
}

void MainWindow::on_untypedButton_clicked()
{
    BatchView* w = new BatchView(this, this->pano, BatchMode::Manual, BatchViewMode::OnlyUntyped);
    w->show();
}

void MainWindow::on_facesButton_clicked()
{
    BatchView* w = new BatchView(this, this->pano, BatchMode::Auto, BatchViewMode::OnlyFaces);
    w->show();
}

void MainWindow::on_platesButton_clicked()
{
    BatchView* w = new BatchView(this, this->pano, BatchMode::Auto, BatchViewMode::OnlyUnapprovedNumberPlates);
    w->show();
}

void MainWindow::on_preInvalidatedButton_clicked()
{
    BatchView* w = new BatchView(this, this->pano, BatchMode::Auto, BatchViewMode::OnlyPreInvalidated);
    w->show();
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
