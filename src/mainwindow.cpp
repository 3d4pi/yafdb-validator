#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QGraphicsProxyWidget>

#include "ymlparser.h"

/* Constructor */
MainWindow::MainWindow(QWidget *parent, QString sourceImagePath, QString detectorYMLPath, QString destinationYMLPath) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->initializeValidator(sourceImagePath, detectorYMLPath, destinationYMLPath);
}

/* Destructor */
MainWindow::~MainWindow()
{
    delete ui;
}

/* Initial setup function */
void MainWindow::initializeValidator(QString sourceImagePath, QString detectorYMLPath, QString destinationYMLPath)
{
    ui->setupUi(this);

    /* Set-up options */
    this->options.sourceImagePath = sourceImagePath.length() > 0 ? sourceImagePath : "";
    this->options.detectorYMLPath = detectorYMLPath.length() > 0 ? detectorYMLPath : "";
    this->options.destinationYMLPath = destinationYMLPath.length() > 0 ? destinationYMLPath : "";

    /* Determine good labels colors based on system theme */
    QString good_color_string = "rgb(%1, %2, %3)";
    QColor  good_color_color = QApplication::palette().color(QPalette::Text);
    good_color_string = good_color_string.arg(good_color_color.red()).arg(good_color_color.green()).arg(good_color_color.blue());

    QString warn_color_string = "rgb(%1, %2, %3)";
    QColor  warn_color_color = QApplication::palette().color(QPalette::Highlight);
    warn_color_string = warn_color_string.arg(warn_color_color.red()).arg(warn_color_color.green()).arg(warn_color_color.blue());

    this->good_color = good_color_string;
    this->warn_color = warn_color_string;

    /* Remove margins */
    this->setContentsMargins(-5, -5, -5, -5);

    /* Center window on screen */
    this->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            qApp->desktop()->availableGeometry()
        ));

    /* Start window maximized */
    this->showMaximized();

    /* Create panorama viewer */
    this->pano = new PanoramaViewer(this);

    /* Add panorama viewer to current window */
    this->ui->gridLayout->addWidget(this->pano);

    /* Determine best multi-threading setup */
    int threads_count = QThread::idealThreadCount();

    /* Configure panorama viewer */
    this->pano->setup(
        this->size().width(), // Default width
        this->size().height(), // Default height
        0.6,   // Default image scale factor
        20.0,  // Minimum zoom
        100.0, // Maximum zoom
        100.0, // Default zoom level
        threads_count // Number of threads
    );

    /* Variables to store files presence */
    bool sourceImageFile_exists = false;
    bool detectorYMLFile_exists = false;
    bool destinationYMLFile_exists = false;

    /* Check if source image exists */
    if( this->options.sourceImagePath.length() > 0 )
    {
        QFileInfo sourceImageFile( this->options.sourceImagePath );
        sourceImageFile_exists = ( sourceImageFile.exists() && sourceImageFile.isFile( ));
    }

    /* Check if detector YML exists */
    if( this->options.detectorYMLPath.length() > 0 )
    {
        QFileInfo detectorYMLFile( this->options.detectorYMLPath );
        detectorYMLFile_exists = ( detectorYMLFile.exists() && detectorYMLFile.isFile( ));
    }

    /* Check if destination YML exists */
    if( this->options.destinationYMLPath.length() > 0 )
    {
        QFileInfo destinationYMLFile( this->options.destinationYMLPath );
        destinationYMLFile_exists = ( destinationYMLFile.exists() && destinationYMLFile.isFile( ));
    }

    /* Display proper messages */
    if( !sourceImageFile_exists )
    {
        std::cout << "[ERROR] Invalid source image path: " << this->options.sourceImagePath.toStdString() << std::endl;
        exit( 0 );
    }

    if( this->options.detectorYMLPath.length() > 0 && !detectorYMLFile_exists )
    {
        std::cout << "[ERROR] Invalid detector YML path: " << this->options.detectorYMLPath.toStdString() << std::endl;
    }

    /* Load input image */
    this->pano->loadImage( this->options.sourceImagePath );

    /* Intialize YML parser */
    YMLParser parser;

    /* If detector YML path is specified */
    if( this->options.detectorYMLPath.length() > 0 )
    {
        /* If detector YML file exists */
        if( detectorYMLFile_exists )
        {
            /* If destination YML file exists */
            if( destinationYMLFile_exists )
            {
                /* Load validator YML */
                QList<ObjectRect*> loaded_rects = parser.loadYML( this->options.destinationYMLPath, YMLType::Validator );

                /* Iterate over loaded rects */
                foreach(ObjectRect* rect, loaded_rects)
                {
                    /* Map rect to current scene */
                    rect->mapTo(this->pano->dest_image_map.width(),
                                this->pano->dest_image_map.height(),
                                this->pano->position.azimuth,
                                this->pano->position.elevation,
                                this->pano->position.aperture);

                    /* Append mapped rect to scene */
                    rect->setId( this->pano->rect_list_id_index++ );

                    /* Assign childrens ID's */
                    foreach(ObjectRect* child, rect->childrens)
                    {
                        child->setId( this->pano->rect_list_id_index++ );
                    }

                    this->pano->rect_list.append( rect );
                    this->pano->scene->addItem( rect );

                    /* Check object visibility */
                    if( !this->pano->isObjectVisible( rect ) )
                        rect->setVisible( false );
                }

            /* Destination YML not exists*/
            } else {

                /* Load detector YML */
                QList<ObjectRect*> loaded_rects = parser.loadYML( this->options.detectorYMLPath, YMLType::Detector );

                /* Iterate over loaded rects */
                foreach(ObjectRect* rect, loaded_rects)
                {
                    /* Map rect to current scene */
                    rect->mapFromSpherical(this->pano->image_info.width,
                                           this->pano->image_info.height,
                                           this->pano->dest_image_map.width(),
                                           this->pano->dest_image_map.height(),
                                           this->pano->position.azimuth,
                                           this->pano->position.elevation,
                                           this->pano->position.aperture,
                                           this->pano->zoom_min * ( LG_PI / 180.0 ),
                                           this->pano->zoom_max * ( LG_PI / 180.0 ));

                    /* Append mapped rect to scene */
                    rect->setId( this->pano->rect_list_id_index++ );

                    /* Assign childrens ID's */
                    foreach(ObjectRect* child, rect->childrens)
                    {
                        child->setId( this->pano->rect_list_id_index++ );
                    }

                    this->pano->rect_list.append( rect );
                    this->pano->scene->addItem( rect );

                    /* Check object visibility */
                    if( !this->pano->isObjectVisible( rect ) )
                        rect->setVisible( false );
                }
            }
        }

    /* Detector YML path not specified */
    } else {

        /* Check if destination YML path is specified */
        if( this->options.destinationYMLPath.length() > 0 )
        {
            /* Check if destination YML file exists */
            if( destinationYMLFile_exists )
            {
                /* Load validator YML */
                QList<ObjectRect*> loaded_rects = parser.loadYML( this->options.destinationYMLPath, YMLType::Validator );

                /* Iterate over loaded rects */
                foreach(ObjectRect* rect, loaded_rects)
                {

                    /* Map rect to current scene */
                    rect->mapTo(this->pano->dest_image_map.width(),
                                this->pano->dest_image_map.height(),
                                this->pano->position.azimuth,
                                this->pano->position.elevation,
                                this->pano->position.aperture);

                    /* Append mapped rect to scene */
                    rect->setId( this->pano->rect_list_id_index++ );

                    /* Assign childrens ID's */
                    foreach(ObjectRect* child, rect->childrens)
                    {
                        child->setId( this->pano->rect_list_id_index++ );
                    }

                    this->pano->rect_list.append( rect );
                    this->pano->scene->addItem( rect );

                    /* Check object visibility */
                    if( !this->pano->isObjectVisible( rect ) )
                        rect->setVisible( false );
                }
            }

        /* Destination YML path not specified */
        } else {

            /* Exit program */
            exit( 0 );
        }
    }

    /* Bind ESC key to window close */
    new QShortcut(QKeySequence("Esc"), this, SLOT(onESC()));

    /* Initialize labels */
    emit refreshLabels();
}

/* (UI action) Refresh labels */
void MainWindow::refreshLabels()
{
    /* Types / States counter variables */
    int untyped = 0;

    int facecount = 0;
    int facesvalidated = 0;

    int numberplatescount = 0;
    int numberplatesvalidated = 0;

    int preinvalidatedcount = 0;
    int preinvalidatedvalidated = 0;

    int toblurcount = 0;

    /* Iterate over PanoramaViewer rects */
    foreach(ObjectRect* rect, this->pano->rect_list)
    {
        /* Rect type swith */
        switch(rect->getObjectType())
        {

        /* Untyped rect */
        case ObjectType::None:

            /* Increment untyped items */
            untyped++;
            break;

        /* Face */
        case ObjectType::Face:

            /* If automatic status is valid or automatic status is None */
            if(rect->getAutomaticStatus() == "Valid" || rect->getAutomaticStatus() == "None")
            {
                /* Increment faces count */
                facecount++;
            }

            /* If automatic status is valid or automatic status is None and manual status is not None */
            if( (rect->getAutomaticStatus() == "Valid" || rect->getAutomaticStatus() == "None")
                    && rect->getManualStatus() != "None")
            {
                /* Increment validated faces count */
                facesvalidated++;
            }

            /* If automatic status is pre-filtered */
            if(rect->getAutomaticStatus() != "None" && rect->getAutomaticStatus() != "Valid")
            {
                /* Increment pre-filtered items count */
                preinvalidatedcount++;
            }

            /* If automatic status is pre-filtered and manualy validated */
            if(rect->getAutomaticStatus() != "None" && rect->getAutomaticStatus() != "Valid" && rect->getManualStatus() != "None")
            {
                /* Increment pre-filtered (manualy validated) items count */
                preinvalidatedvalidated++;
            }

            break;

        /* NumberPlate */
        case ObjectType::NumberPlate:

            /* Increment NumberPlate items count */
            numberplatescount++;

            /* If object is manualy validated */
            if(rect->getManualStatus() != "None")
            {
                /* Increment NumberPlate (manualy validated) items count */
                numberplatesvalidated++;
            }

            break;

        /* "ToBlur" */
        case ObjectType::ToBlur:

            /* Increment "ToBlur" objects count */
            toblurcount++;
            break;
        }
    }

    /* Untyped items labels update */
    if(untyped > 0)
    {
        this->ui->untypedLabel->setStyleSheet("QLabel {color: " + this->warn_color + "; }");
        this->ui->untypedButton->setEnabled(true);
    } else {
        this->ui->untypedLabel->setStyleSheet("QLabel {color: " + this->good_color + "; }");
        this->ui->untypedButton->setEnabled(false);
    }

    /* Face items labels update */
    if(facecount <= 0)
    {
        this->ui->facesButton->setEnabled(false);
    } else {
        this->ui->facesButton->setEnabled(true);
    }

    /* NumberPlate items labels update */
    if(numberplatescount <= 0)
    {
        this->ui->platesButton->setEnabled(false);
    } else {
        this->ui->platesButton->setEnabled(true);
    }

    /* Pre-invalidated items labels update */
    if(preinvalidatedcount <= 0)
    {
        this->ui->preInvalidatedButton->setEnabled(false);
    } else {
        this->ui->preInvalidatedButton->setEnabled(true);
    }

    /* "ToBlur" items labels update */
    if(toblurcount <= 0)
    {
        this->ui->toBlurButton->setEnabled(false);
    } else {
        this->ui->toBlurButton->setEnabled(true);
    }

    /* Assign proper label color if not all Faces are manualy vlidated */
    if(facesvalidated != facecount)
    {
        this->ui->facesLabel->setStyleSheet("QLabel {color: " + this->warn_color + "; }");
    } else {
        this->ui->facesLabel->setStyleSheet("QLabel {color: " + this->good_color + "; }");
    }

    /* Assign proper label color if not all NumberPlates are manualy vlidated */
    if(numberplatesvalidated != numberplatescount)
    {
        this->ui->platesLabel->setStyleSheet("QLabel {color: " + this->warn_color + "; }");
    } else {
        this->ui->platesLabel->setStyleSheet("QLabel {color: " + this->good_color + "; }");
    }

    /* Assign proper label color if not all pre-invalidated objects are manualy vlidated */
    if(preinvalidatedvalidated != preinvalidatedcount)
    {
        this->ui->preInvalidatedLabel->setStyleSheet("QLabel {color: " + this->warn_color + "; }");
    } else {
        this->ui->preInvalidatedLabel->setStyleSheet("QLabel {color: " + this->good_color + "; }");
    }

    /* Update labels text */
    /* Untyped items */
    this->ui->untypedLabel->setText("Untyped items: " + QString::number(untyped));

    /* Faces */
    this->ui->facesLabel->setText("Faces: " + QString::number(facesvalidated) + "/" + QString::number(facecount));

    /* NumberPlates */
    this->ui->platesLabel->setText("Number plates: " + QString::number(numberplatesvalidated) + "/" + QString::number(numberplatescount));

    /* Pre-invalidated */
    this->ui->preInvalidatedLabel->setText("Pre-invalidated: " + QString::number(preinvalidatedvalidated) + "/" + QString::number(preinvalidatedcount));

    /* "ToBlur" */
    this->ui->toBlurLabel->setText("To blur: " + QString::number(toblurcount));

}

/* (UI action) Update scale factor slider */
void MainWindow::updateScaleSlider(int value)
{
    /* Assign valude */
    this->ui->horizontalSlider->setValue( value );
}

/* Window close event */
void MainWindow::closeEvent (QCloseEvent *event)
{
    /* Warn user about exit and ask to save changes */
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "",
                                                                tr("Do you want to save your work\n(You can resume later)"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    /* Cancel */
    if (resBtn == QMessageBox::Cancel) {

        /* Ignore action */
        event->ignore();

    /* Yes */
    } else if( resBtn == QMessageBox::Yes ) {

        /* Initialize YMLparser */
        YMLParser parser;

        /* Save YML */
        parser.writeYML( this->pano->rect_list, this->options.destinationYMLPath );

        /* Accept event */
        event->accept();

    /* No */
    } else if(resBtn == QMessageBox::No) {

        /* Accept event */
        event->accept();
    }
}

/* (Key signal) ESC key pressed */
void MainWindow::onESC()
{
    /* CLose window */
    this->close();
}

/* (UI component signal) Untyped button clicked */
void MainWindow::on_untypedButton_clicked()
{
    /* Initialize untyped items batch view */
    BatchView* w = new BatchView(this, this->pano, BatchMode::Manual, BatchViewMode::OnlyUntyped);

    /* Set delete on close flag */
    w->setAttribute( Qt::WA_DeleteOnClose );

    /* Show window */
    w->show();
}

/* (UI component signal) faces button clicked */
void MainWindow::on_facesButton_clicked()
{
    /* Initialize faces batch view */
    BatchView* w = new BatchView(this, this->pano, BatchMode::Auto, BatchViewMode::OnlyFaces);

    /* Set delete on close flag */
    w->setAttribute( Qt::WA_DeleteOnClose );

    /* Show window */
    w->show();
}

/* (UI component signal) Numberplates button clicked */
void MainWindow::on_platesButton_clicked()
{
    /* Initialize NumberPlates batch view */
    BatchView* w = new BatchView(this, this->pano, BatchMode::Auto, BatchViewMode::OnlyNumberPlates);

    /* Set delete on close flag */
    w->setAttribute( Qt::WA_DeleteOnClose );

    /* Show window */
    w->show();
}

/* (UI component signal) Pre-invalidated button clicked */
void MainWindow::on_preInvalidatedButton_clicked()
{
    /* Initialize pre-invalidated batch view */
    BatchView* w = new BatchView(this, this->pano, BatchMode::Auto, BatchViewMode::OnlyPreInvalidated);

    /* Set delete on close flag */
    w->setAttribute( Qt::WA_DeleteOnClose );

    /* Show window */
    w->show();
}

/* (UI component signal) "ToBlur" button clicked */
void MainWindow::on_toBlurButton_clicked()
{
    /* Initialize "ToBlur" batch view */
    BatchView* w = new BatchView(this, this->pano, BatchMode::ToBlur, BatchViewMode::OnlyToBlur);

    /* Set delete on close flag */
    w->setAttribute( Qt::WA_DeleteOnClose );

    /* Show window */
    w->show();
}

/* (UI component signal) Scale slider moved */
void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    /* Save PanoramaViewer positions */
    this->pano->backupPosition();

    /* Change scale factor */
    this->pano->scale_factor = (position / 10.0);

    /* Render panorama */
    this->pano->render();
}

/* (UI component signal) "All" radio button checked */
void MainWindow::on_allVisRadio_clicked()
{
    /* Update PanoramaViewer visibility group */
    this->pano->setVisGroup( PanoramaViewerVisGroups::All );
}

/* (UI component signal) "Auto" radio button checked */
void MainWindow::on_autoVisRadio_clicked()
{
    /* Update PanoramaViewer visibility group */
    this->pano->setVisGroup( PanoramaViewerVisGroups::Automatic );
}

/* (UI component signal) "Manual" radio button checked */
void MainWindow::on_manualVisRadio_clicked()
{
    /* Update PanoramaViewer visibility group */
    this->pano->setVisGroup( PanoramaViewerVisGroups::Manual );
}

/* (UI component signal) "In Creation" radio button checked */
void MainWindow::on_inCreationVisRadio_clicked()
{
    /* Update PanoramaViewer visibility group */
    this->pano->setVisGroup( PanoramaViewerVisGroups::InCreation );
}
