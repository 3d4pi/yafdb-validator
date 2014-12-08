#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QGraphicsProxyWidget>
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


    foreach(ObjectRect2* rect, this->pano->rect_list_v2)
    {
        switch(rect->getType())
        {
            case ObjectType::None:
                untyped++;
                break;
            case ObjectType::Face:

                if(rect->getAutomaticStatus() == "Valid" || rect->getAutomaticStatus() == "None")
                    facecount++;

                if( (rect->getAutomaticStatus() == "Valid" || rect->getAutomaticStatus() == "None")
                        && rect->getManualStatus() != "None")
                {
                    facesvalidated++;
                }

                if(rect->getAutomaticStatus() != "None" && rect->getAutomaticStatus() != "Valid")
                {
                    preinvalidatedcount++;
                }
                if(rect->getAutomaticStatus() != "None" && rect->getAutomaticStatus() != "Valid" && rect->getManualStatus() != "None")
                {
                    preinvalidatedvalidated++;
                }

                break;
            case ObjectType::NumberPlate:

                numberplatescount++;

                if(rect->getManualStatus() != "None")
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
    BatchView* w = new BatchView(this, this->pano, BatchMode::Auto, BatchViewMode::OnlyNumberPlates);
    w->show();
}

void MainWindow::on_preInvalidatedButton_clicked()
{
    BatchView* w = new BatchView(this, this->pano, BatchMode::Auto, BatchViewMode::OnlyPreInvalidated);
    w->show();
}

void MainWindow::on_pushButton_clicked()
{
    srand (time(NULL));

   /* for (int x = 0; x < 100; x++)
    {
        // Create selection object
        ObjectRect* rect = new ObjectRect();

        rect->setObjectType(ObjectType::Face);
        rect->setRectType(RectType::Auto);
        rect->setValidState(ObjectValidState::None);
        rect->setAutomaticStatus("Valid");
        rect->setBlurred(true);

        rect->id = this->pano->rect_list.length();

        normalization_struct norm_params;
        norm_params.pano_height = this->pano->height();
        norm_params.pano_width = this->pano->width();
        norm_params.scale_factor = this->pano->scale_factor;

        int rw = (rand() % (int)(this->pano->scene->width() - 1)) + 1;
        int rh = (rand() % (int)(this->pano->scene->height() - 1)) + 1;

        float norm_w = util::normalize(rw, norm_params);
        float norm_h = util::normalize(rh, norm_params);

        rect->setPos(QPointF(0.0, 0.0), QPointF(norm_w, norm_h), norm_params, RectMoveType::All);

        // Save projection parameters to it
        rect->projection_parameters.aperture  = this->pano->current_zoom_rad;
        rect->projection_parameters.azimuth   = this->pano->position.azimuth;
        rect->projection_parameters.elevation = this->pano->position.elevation;
        rect->projection_parameters.scale_factor = this->pano->scale_factor;

        // Add selection object to list
        this->pano->rect_list.append(rect);

        QGraphicsProxyWidget *proxyWidget = new QGraphicsProxyWidget();
        proxyWidget->setWidget(rect);

        // Add selection object to scene
        this->pano->scene->addItem(proxyWidget);
    }

    for (int x = 0; x < 100; x++)
    {
        // Create selection object
        ObjectRect* rect = new ObjectRect();

        rect->setObjectType(ObjectType::NumberPlate);
        rect->setRectType(RectType::Auto);
        rect->setAutomaticStatus("Valid");
        rect->setBlurred(true);

        rect->id = this->pano->rect_list.length();

        normalization_struct norm_params;
        norm_params.pano_height = this->pano->height();
        norm_params.pano_width = this->pano->width();
        norm_params.scale_factor = this->pano->scale_factor;

        int rw = (rand() % (int)(this->pano->scene->width() - 1)) + 1;
        int rh = (rand() % (int)(this->pano->scene->height() - 1)) + 1;

        float norm_w = util::normalize(rw, norm_params);
        float norm_h = util::normalize(rh, norm_params);

        rect->setPos(QPointF(0.0, 0.0), QPointF(norm_w, norm_h), norm_params, RectMoveType::All);

        // Save projection parameters to it
        rect->projection_parameters.aperture  = this->pano->current_zoom_rad;
        rect->projection_parameters.azimuth   = this->pano->position.azimuth;
        rect->projection_parameters.elevation = this->pano->position.elevation;
        rect->projection_parameters.scale_factor = this->pano->scale_factor;

        // Add selection object to list
        this->pano->rect_list.append(rect);

        QGraphicsProxyWidget *proxyWidget = new QGraphicsProxyWidget();
        proxyWidget->setWidget(rect);

        // Add selection object to scene
        this->pano->scene->addItem(proxyWidget);
    }

    for (int x = 0; x < 30; x++)
    {
        // Create selection object
        ObjectRect* rect = new ObjectRect();

        rect->setObjectType(ObjectType::Face);
        rect->setRectType(RectType::Auto);
        rect->setValidState(ObjectValidState::None);
        rect->setAutomaticStatus("Ratio");
        rect->setBlurred(false);

        normalization_struct norm_params;
        norm_params.pano_height = this->pano->height();
        norm_params.pano_width = this->pano->width();
        norm_params.scale_factor = this->pano->scale_factor;

        int rw = (rand() % (int)(this->pano->scene->width() - 1)) + 1;
        int rh = (rand() % (int)(this->pano->scene->height() - 1)) + 1;

        float norm_w = util::normalize(rw, norm_params);
        float norm_h = util::normalize(rh, norm_params);

        rect->setPos(QPointF(0.0, 0.0), QPointF(norm_w, norm_h), norm_params, RectMoveType::All);

        rect->id = this->pano->rect_list.length();

        // Save projection parameters to it
        rect->projection_parameters.aperture  = this->pano->current_zoom_rad;
        rect->projection_parameters.azimuth   = this->pano->position.azimuth;
        rect->projection_parameters.elevation = this->pano->position.elevation;
        rect->projection_parameters.scale_factor = this->pano->scale_factor;

        // Add selection object to list
        this->pano->rect_list.append(rect);

        QGraphicsProxyWidget *proxyWidget = new QGraphicsProxyWidget();
        proxyWidget->setWidget(rect);

        // Add selection object to scene
        this->pano->scene->addItem(proxyWidget);
    }*/

    emit refreshLabels();
}

void MainWindow::on_pushButton_2_clicked()
{


}

void MainWindow::on_pushButton_3_clicked()
{
    this->pano->position.old_azimuth = this->pano->position.azimuth;

    this->pano->position.azimuth += 1 * (LG_PI / 180);
    this->pano->render();
}

void MainWindow::on_pushButton_4_clicked()
{
    //this->pano->position.old_azimuth = this->pano->position.azimuth;

    this->pano->position.azimuth -= 1 * (LG_PI / 180);
    this->pano->render();
}

void MainWindow::on_pushButton_5_clicked()
{
    this->pano->position.old_azimuth = this->pano->position.azimuth;
    this->pano->position.old_elevation = this->pano->position.elevation;
    this->pano->position.old_aperture = this->pano->position.aperture;

    //this->pano->position.old_elevation = this->pano->position.elevation;

    //this->pano->position.elevation += 1.0 * (LG_PI / 180);
    this->pano->render();
}

void MainWindow::on_pushButton_6_clicked()
{
    this->pano->position.old_azimuth = this->pano->position.azimuth;
    this->pano->position.old_elevation = this->pano->position.elevation;
    this->pano->position.old_aperture = this->pano->position.aperture;

    //this->pano->position.elevation -= 1.0 * (LG_PI / 180);
    this->pano->render();
}

void MainWindow::on_pushButton_7_clicked()
{
}
