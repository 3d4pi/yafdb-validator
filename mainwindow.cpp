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

    this->timer = new QElapsedTimer();
    this->timer->start();

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
        this->size().width(), // Default width
        this->size().height(), // Default height
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

    int toblurcount = 0;

    foreach(ObjectRect* rect, this->pano->rect_list)
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
        case ObjectType::ToBlur:

            toblurcount++;
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

    if(facecount <= 0)
    {
        this->ui->facesButton->setEnabled(false);
    } else {
        this->ui->facesButton->setEnabled(true);
    }

    if(numberplatescount <= 0)
    {
        this->ui->platesButton->setEnabled(false);
    } else {
        this->ui->platesButton->setEnabled(true);
    }

    if(preinvalidatedcount <= 0)
    {
        this->ui->preInvalidatedButton->setEnabled(false);
    } else {
        this->ui->preInvalidatedButton->setEnabled(true);
    }

    if(toblurcount <= 0)
    {
        this->ui->toBlurButton->setEnabled(false);
    } else {
        this->ui->toBlurButton->setEnabled(true);
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

    this->ui->untypedLabel->setText("Untyped items: " + QString::number(untyped));
    this->ui->facesLabel->setText("Faces: " + QString::number(facesvalidated) + "/" + QString::number(facecount));
    this->ui->platesLabel->setText("Number plates: " + QString::number(numberplatesvalidated) + "/" + QString::number(numberplatescount));
    this->ui->preInvalidatedLabel->setText("Pre-invalidated: " + QString::number(preinvalidatedvalidated) + "/" + QString::number(preinvalidatedcount));
    this->ui->toBlurLabel->setText("To blur: " + QString::number(toblurcount));

}

MainWindow::~MainWindow()
{
    //delete this->pano;
    delete ui;
}

void MainWindow::on_untypedButton_clicked()
{
    BatchView* w = new BatchView(this, this->pano, BatchMode::Manual, BatchViewMode::OnlyUntyped);
    w->setAttribute( Qt::WA_DeleteOnClose );
    w->show();
}

void MainWindow::on_facesButton_clicked()
{
    BatchView* w = new BatchView(this, this->pano, BatchMode::Auto, BatchViewMode::OnlyFaces);
    w->setAttribute( Qt::WA_DeleteOnClose );
    w->show();
}

void MainWindow::on_platesButton_clicked()
{
    BatchView* w = new BatchView(this, this->pano, BatchMode::Auto, BatchViewMode::OnlyNumberPlates);
    w->setAttribute( Qt::WA_DeleteOnClose );
    w->show();
}

void MainWindow::on_preInvalidatedButton_clicked()
{
    BatchView* w = new BatchView(this, this->pano, BatchMode::Auto, BatchViewMode::OnlyPreInvalidated);
    w->setAttribute( Qt::WA_DeleteOnClose );
    w->show();
}

void MainWindow::on_toBlurButton_clicked()
{
    BatchView* w = new BatchView(this, this->pano, BatchMode::ToBlur, BatchViewMode::OnlyToBlur);
    w->setAttribute( Qt::WA_DeleteOnClose );
    w->show();
}

void MainWindow::on_pushButton_clicked()
{
    srand( this->timer->nsecsElapsed() );

    for (int x = 0; x < 5; x++)
    {

        // Create selection object
        ObjectRect* rect = new ObjectRect();

        //int randtype = (rand() % 4) + 1;
        rect->setType( ObjectType::Face );

        rect->setObjectRectState(ObjectRectState::None);

        //int randstate = (rand() % 2);
        rect->setObjectRectType( ObjectItemRectType::Valid );
        rect->setAutomaticStatus( "Valid" );

        rect->setBlurred(true);

        rect->setId(this->pano->rect_list_index++);

        float rect_size = 64.0;
        float pos_x = ((float) (rand() % this->pano->dest_image_map.width() - rect_size));
        float pos_y = ((float) (rand() % this->pano->dest_image_map.height() - rect_size));

        rect->setPoints(QPointF(pos_x, pos_y),
                        QPointF(pos_x, pos_y + rect_size),
                        QPointF(pos_x + rect_size, pos_y + rect_size),
                        QPointF(pos_x + rect_size, pos_y));

        rect->setProjectionPoints();

        rect->setProjectionParametters(this->pano->position.azimuth,
                                       this->pano->position.elevation,
                                       this->pano->position.aperture,
                                       this->pano->dest_image.width(),
                                       this->pano->dest_image.height());

        // Add selection object to scene
        this->pano->rect_list.append( rect );
        this->pano->scene->addItem(rect);
    }

    for (int x = 0; x < 5; x++)
    {

        // Create selection object
        ObjectRect* rect = new ObjectRect();

        //int randtype = (rand() % 4) + 1;
        rect->setType( ObjectType::Face );

        rect->setObjectRectState(ObjectRectState::None);

        //int randstate = (rand() % 2);
        rect->setObjectRectType( ObjectItemRectType::Invalid );
        rect->setAutomaticStatus( "Ratio" );

        rect->setBlurred(true);

        rect->setId(this->pano->rect_list_index++);

        float rect_size = 64.0;
        float pos_x = ((float) (rand() % this->pano->dest_image_map.width() - rect_size));
        float pos_y = ((float) (rand() % this->pano->dest_image_map.height() - rect_size));

        rect->setPoints(QPointF(pos_x, pos_y),
                        QPointF(pos_x, pos_y + rect_size),
                        QPointF(pos_x + rect_size, pos_y + rect_size),
                        QPointF(pos_x + rect_size, pos_y));

        rect->setProjectionPoints();

        rect->setProjectionParametters(this->pano->position.azimuth,
                                       this->pano->position.elevation,
                                       this->pano->position.aperture,
                                       this->pano->dest_image.width(),
                                       this->pano->dest_image.height());

        // Add selection object to scene
        this->pano->rect_list.append( rect );
        this->pano->scene->addItem(rect);
    }

    for (int x = 0; x < 5; x++)
    {

        // Create selection object
        ObjectRect* rect = new ObjectRect();

        //int randtype = (rand() % 4) + 1;
        rect->setType( ObjectType::NumberPlate );

        rect->setObjectRectState(ObjectRectState::None);

        //int randstate = (rand() % 2);
        rect->setObjectRectType( ObjectItemRectType::Valid );
        rect->setAutomaticStatus( "Valid" );

        rect->setBlurred(true);

        rect->setId(this->pano->rect_list_index++);

        float rect_size = 64.0;
        float pos_x = ((float) (rand() % this->pano->dest_image_map.width() - rect_size));
        float pos_y = ((float) (rand() % this->pano->dest_image_map.height() - rect_size));

        rect->setPoints(QPointF(pos_x, pos_y),
                        QPointF(pos_x, pos_y + rect_size),
                        QPointF(pos_x + rect_size, pos_y + rect_size),
                        QPointF(pos_x + rect_size, pos_y));

        rect->setProjectionPoints();

        rect->setProjectionParametters(this->pano->position.azimuth,
                                       this->pano->position.elevation,
                                       this->pano->position.aperture,
                                       this->pano->dest_image.width(),
                                       this->pano->dest_image.height());

        // Add selection object to scene
        this->pano->rect_list.append( rect );
        this->pano->scene->addItem(rect);
    }

    /*
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

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    this->pano->backupPosition();
    this->pano->scale_factor = (position / 100.0);
    this->pano->render();
}

void MainWindow::on_pushButton_2_clicked()
{
    foreach(ObjectRect* rect, this->pano->rect_list)
    {
        this->pano->rect_list.removeOne( rect );
        delete rect;
    }

    emit refreshLabels();
}
