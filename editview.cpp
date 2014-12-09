#include "editview.h"
#include "ui_editview.h"

EditView::EditView(QWidget *parent, ObjectRect* rect) :
    QMainWindow(parent),
    ui(new Ui::EditView)
{
    ui->setupUi(this);

    this->ref_rect = rect;

    pano_parent = qobject_cast<PanoramaViewer *>(parent);

    // Connect signal for labels refresh
    connect(this, SIGNAL(refreshLabels()), parent, SLOT(refreshLabels_slot()));

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
    this->ui->mainLayout->addWidget(this->pano);

    // Configure panorama viewer
    this->pano->setup(
        this->size().width(), // Default width
        this->size().height(), // Default height
        pano_parent->scale_factor,   // Image scale factor
        pano_parent->zoom_min,  // Minimum zoom
        pano_parent->zoom_max, // Maximum zoom
        100.0, // Default zoom level
        pano_parent->threads_count // Number of threads
    );

    this->pano->setView( rect->proj_azimuth(), rect->proj_elevation() );
    this->pano->setZoom( rect->proj_aperture() / (LG_PI / 180.0) );

    this->pano->setMoveEnabled( false );

    // Load input image
    this->pano->loadImage( pano_parent->src_image );

    qDebug() << this->pano->size();

    // Set-up labels
    this->ui->subClassLabel->setText("Sub classes: None");

    switch(this->ref_rect->getType())
    {
        case ObjectType::None:
            this->ui->clannNameLabel->setText("Class name: None");
            break;
        case ObjectType::Face:
            this->ui->clannNameLabel->setText("Class name: Face");
            break;
        case ObjectType::NumberPlate:
            this->ui->clannNameLabel->setText("Class name: NumberPlate");
            break;
        case ObjectType::ToBlur:
            this->ui->clannNameLabel->setText("Class name: ToBlur");
            break;
    }

    this->ui->widthLabel->setText("Width: " + QString::number( (int) this->ref_rect->getSize().width() ));
    this->ui->heightLabel->setText("Height: " + QString::number( (int) this->ref_rect->getSize().height() ));

    this->ui->preFiltersLabel->setText("Pre-filter status: " + this->ref_rect->getAutomaticStatus());

    ObjectRect* rect_copy = new ObjectRect();
    rect_copy->setObjectRectType( this->ref_rect->getObjectRectType() );
    rect_copy->setObjectRectState( this->ref_rect->getObjectRectState() );
    rect_copy->setManualStatus( this->ref_rect->getManualStatus() );
    rect_copy->setBlurred( this->ref_rect->isBlurred() );
    rect_copy->setId( this->ref_rect->getId() );

    rect_copy->setProjectionParametters(this->ref_rect->proj_azimuth(),
                                        this->ref_rect->proj_elevation(),
                                        this->ref_rect->proj_aperture(),
                                        this->ref_rect->proj_width(),
                                        this->ref_rect->proj_height());

    double p1_x = 0.0;
    double p1_y = 0.0;
    double p2_x = 0.0;
    double p2_y = 0.0;
    double p3_x = 0.0;
    double p3_y = 0.0;
    double p4_x = 0.0;
    double p4_y = 0.0;

    g2g_point(this->ref_rect->proj_width(),
              this->ref_rect->proj_height(),
              this->ref_rect->proj_azimuth(),
              this->ref_rect->proj_elevation(),
              this->ref_rect->proj_aperture(),
              this->ref_rect->proj_point_1().x(),
              this->ref_rect->proj_point_1().y(),

              this->pano->dest_image_map.width(),
              this->pano->dest_image_map.height(),
              this->pano->position.azimuth,
              this->pano->position.elevation,
              this->pano->position.aperture,
              &p1_x,
              &p1_y);

    g2g_point(this->ref_rect->proj_width(),
              this->ref_rect->proj_height(),
              this->ref_rect->proj_azimuth(),
              this->ref_rect->proj_elevation(),
              this->ref_rect->proj_aperture(),
              this->ref_rect->proj_point_2().x(),
              this->ref_rect->proj_point_2().y(),

              this->pano->dest_image_map.width(),
              this->pano->dest_image_map.height(),
              this->pano->position.azimuth,
              this->pano->position.elevation,
              this->pano->position.aperture,
              &p2_x,
              &p2_y);

    g2g_point(this->ref_rect->proj_width(),
              this->ref_rect->proj_height(),
              this->ref_rect->proj_azimuth(),
              this->ref_rect->proj_elevation(),
              this->ref_rect->proj_aperture(),
              this->ref_rect->proj_point_3().x(),
              this->ref_rect->proj_point_3().y(),

              this->pano->dest_image_map.width(),
              this->pano->dest_image_map.height(),
              this->pano->position.azimuth,
              this->pano->position.elevation,
              this->pano->position.aperture,
              &p3_x,
              &p3_y);

    g2g_point(this->ref_rect->proj_width(),
              this->ref_rect->proj_height(),
              this->ref_rect->proj_azimuth(),
              this->ref_rect->proj_elevation(),
              this->ref_rect->proj_aperture(),
              this->ref_rect->proj_point_4().x(),
              this->ref_rect->proj_point_4().y(),

              this->pano->dest_image_map.width(),
              this->pano->dest_image_map.height(),
              this->pano->position.azimuth,
              this->pano->position.elevation,
              this->pano->position.aperture,
              &p4_x,
              &p4_y);

    rect_copy->setPoints(QPointF(p1_x, p1_y),
                         QPointF(p2_x, p2_y),
                         QPointF(p3_x, p3_y),
                         QPointF(p4_x, p4_y));

    this->pano->rect_list.append( rect_copy );
    this->pano->scene->addItem( rect_copy );

}

EditView::~EditView()
{
    delete ui;
}

void EditView::on_pushButton_clicked()
{
    foreach(ObjectRect* rect, this->pano->rect_list)
    {
        qDebug() << rect->getPoints();
    }
}

void EditView::on_cancelButton_clicked()
{
    this->close();
}

void EditView::on_deleteButton_clicked()
{
    pano_parent->rect_list.removeOne( this->ref_rect );
    delete this->ref_rect;
    emit refreshLabels();
    this->close();
}
