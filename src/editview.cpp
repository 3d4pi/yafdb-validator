#include "editview.h"
#include "ui_editview.h"

EditView::EditView(QWidget *parent, ObjectRect* rect, image_info_struct image_info, ObjectItem* item, int mode) :
    QMainWindow(parent),
    ui(new Ui::EditView)
{
    ui->setupUi(this);

    /* Assign view mode */
    this->mode = mode;

    /* Assign parent item */
    this->item = item;

    /* Assign parent rect */
    this->ref_rect = rect;

    /* Assign parent PanoramaViewer */
    this->pano_parent = qobject_cast<PanoramaViewer *>(parent);

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

    /* Disable firt element on the type selection dropdown "Select..." */
    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(this->ui->typeList->model());
    QModelIndex firstIndex = model->index(0, this->ui->typeList->modelColumn(),
                                          this->ui->typeList->rootModelIndex());
    QStandardItem* firstItem = model->itemFromIndex(firstIndex);
    firstItem->setSelectable(false);

    /* Connect signal for labels refresh */
    connect(this, SIGNAL(refreshLabels()), parent, SLOT(refreshLabels_slot()));

    /* Start window maximized */
    this->showMaximized();

    /* Create panorama viewer */
    this->pano = new PanoramaViewer(this, false);

    /* Set image settings */
    this->pano->image_info = image_info;

    /* Add panorama viewer to current window */
    this->ui->mainLayout->addWidget(this->pano);

    /* Configure panorama viewer */
    this->pano->setup(
        this->size().width(), // Default width
        this->size().height(), // Default height
        pano_parent->scale_factor,   // Image scale factor
        pano_parent->zoom_min,  // Minimum zoom
        pano_parent->zoom_max, // Maximum zoom
        100.0, // Default zoom level
        pano_parent->threads_count // Number of threads
    );

    /* Setup panorama view with rect parameters */
    this->pano->setView( rect->proj_azimuth(), rect->proj_elevation() );
    this->pano->setZoom( rect->proj_aperture() / (LG_PI / 180.0) );

    /* Configure panorama features */
    this->pano->setMoveEnabled( false );
    this->pano->setZoomEnabled( false );
    this->pano->setCreateEnabled( false );
    this->pano->setEditEnabled( false );

    /* Check if rect have an automatic status */
    if (this->ref_rect->getAutomaticStatus() != "None")
    {
        /* Configure panorama features */
        this->ui->typeList->setEnabled( false );
        this->ui->subTypeList->setEnabled( false );
        this->ui->deleteButton->setEnabled( false );
    }


    /* Set-up type combobox value and type label */
    switch(this->ref_rect->getObjectType())
    {

    /* None type */
    case ObjectType::None:

        /* Set selected item */
        this->ui->typeList->setCurrentIndex( 0 );

        /* Set label text */
        this->ui->classNameLabel->setText("Class name: None");
        break;

    /* Face type */
    case ObjectType::Face:

        /* Set selected item */
        this->ui->typeList->setCurrentIndex( 1 );

        /* Set label text */
        this->ui->classNameLabel->setText("Class name: Face");
        break;

    /* Numberplate type */
    case ObjectType::NumberPlate:

        /* Set selected item */
        this->ui->typeList->setCurrentIndex( 2 );

        /* Set label text */
        this->ui->classNameLabel->setText("Class name: NumberPlate");
        break;

    /* "ToBlur" type */
    case ObjectType::ToBlur:

        /* Set selected item */
        this->ui->typeList->setCurrentIndex( 3 );

        /* Set label text */
        this->ui->classNameLabel->setText("Class name: ToBlur");

        /* Configure panorama features */
        this->ui->validCheckBox->setEnabled( false );
        this->ui->blurCheckBox->setEnabled( false );
        break;
    }

    /* Set-up subtype combobox value and subtype label */
    switch(this->ref_rect->getObjectSubType())
    {

    /* None type */
    case ObjectSubType::None:

        /* Set selected item */
        this->ui->subTypeList->setCurrentIndex( 0 );

        /* Set label text */
        this->ui->subClassLabel->setText("Sub class name: None");
        break;

    /* Front type */
    case ObjectSubType::Front:

        /* Set selected item */
        this->ui->subTypeList->setCurrentIndex( 1 );

        /* Set label text */
        this->ui->subClassLabel->setText("Sub class name: Front");
        break;

    /* Profile type */
    case ObjectSubType::Profile:

        /* Set selected item */
        this->ui->subTypeList->setCurrentIndex( 2 );

        /* Set label text */
        this->ui->subClassLabel->setText("Sub class name: Profile");
        break;

    /* Back type */
    case ObjectSubType::Back:

        /* Set selected item */
        this->ui->subTypeList->setCurrentIndex( 3 );

        /* Set label text */
        this->ui->subClassLabel->setText("Sub class name: Back");
        break;

    /* Top type */
    case ObjectSubType::Top:

        /* Set selected item */
        this->ui->subTypeList->setCurrentIndex( 4 );

        /* Set label text */
        this->ui->subClassLabel->setText("Sub class name: Top");
        break;

    /* Eye type */
    case ObjectSubType::Eyes:

        /* Set selected item */
        this->ui->subTypeList->setCurrentIndex( 5 );

        /* Set label text */
        this->ui->subClassLabel->setText("Sub class name: Eyes");
        break;
    }

    /* Set size labels text */
    this->ui->widthLabel->setText("Width: " + QString::number( (int) this->ref_rect->getSize().width() ));
    this->ui->heightLabel->setText("Height: " + QString::number( (int) this->ref_rect->getSize().height() ));

    /* Set pre-filter label text */
    this->ui->preFiltersLabel->setText("Pre-filter status: " + this->ref_rect->getAutomaticStatus());

    /* Assign checkboxes default values */
    this->ui->validCheckBox->setChecked( this->ref_rect->isValidated() );
    this->ui->blurCheckBox->setChecked( this->ref_rect->isBlurred() );

    /* Copy referenced rect */
    this->rect_copy = this->ref_rect->copy();

    /* Map copied rect to current panorama viewer */
    this->rect_copy->mapTo(this->pano->dest_image_map.width(),
                           this->pano->dest_image_map.height(),
                           this->rect_copy->proj_azimuth(),
                           this->rect_copy->proj_elevation(),
                           this->rect_copy->proj_aperture());

    /* Add copied rect to panorama viewer */
    this->pano->rect_list.append( this->rect_copy );
    this->pano->scene->addItem( this->rect_copy );

}

/* Destructor */
EditView::~EditView()
{
    /* Delete UI */
    delete ui;
}

/* Function to merge rect to specified destination */
void EditView::mergeEditedRect(ObjectRect* destination)
{

    /* Apply selected type to rect */
    switch(this->ui->typeList->currentIndex())
    {

    /* Face type */
    case 1:

        /* Apply rect type */
        destination->setObjectType( ObjectType::Face );
        break;

    /* Numberplate type */
    case 2:

        /* Apply rect type */
        destination->setObjectType( ObjectType::NumberPlate );
        break;

    /* "ToBlur" type */
    case 3:

        /* Apply rect type */
        destination->setObjectType( ObjectType::ToBlur );
        break;
    }

    /* Apply selected subtype to rect */
    switch(this->ui->subTypeList->currentIndex())
    {

    /* None type */
    case 0:

        /* Apply rect subtype */
        destination->setObjectSubType( ObjectSubType::None );
        break;

    /* Front type */
    case 1:

        /* Apply rect subtype */
        destination->setObjectSubType( ObjectSubType::Front );
        break;

    /* Profile type */
    case 2:

        /* Apply rect subtype */
        destination->setObjectSubType( ObjectSubType::Profile );
        break;

    /* Back type */
    case 3:

        /* Apply rect subtype */
        destination->setObjectSubType( ObjectSubType::Back );
        break;

    /* Top type */
    case 4:

        /* Apply rect subtype */
        destination->setObjectSubType( ObjectSubType::Top );
        break;

    /* Eye type */
    case 5:

        /* Apply rect subtype */
        destination->setObjectSubType( ObjectSubType::Eyes );
        break;
    }
}

/* Function to merge item to specified destination */
void EditView::mergeEditedItem(ObjectItem* destination)
{

    /* Apply selected type to item */
    switch(this->ui->typeList->currentIndex())
    {

    /* Face type */
    case 1:

        /* Apply item type */
        destination->setItemType( ObjectType::Face );
        break;

    /* Numberplate type */
    case 2:

        /* Apply item type */
        destination->setItemType( ObjectType::NumberPlate );
        break;

    /* "ToBlur" type */
    case 3:

        /* Apply item type */
        destination->setItemType( ObjectType::ToBlur );
        break;
    }

    /* Apply selected subtype to item */
    switch(this->ui->subTypeList->currentIndex())
    {

    /* None type */
    case 0:

        /* Apply item subtype */
        destination->setItemSubType( ObjectSubType::None );
        break;

    /* Front type */
    case 1:

        /* Apply item subtype */
        destination->setItemSubType( ObjectSubType::Front );
        break;

    /* Profile type */
    case 2:

        /* Apply item subtype */
        destination->setItemSubType( ObjectSubType::Profile );
        break;

    /* Back type */
    case 3:

        /* Apply item subtype */
        destination->setItemSubType( ObjectSubType::Back );
        break;

    /* Top type */
    case 4:

        /* Apply item subtype */
        destination->setItemSubType( ObjectSubType::Top );
        break;

    /* Eye type */
    case 5:

        /* Apply item subtype */
        destination->setItemSubType( ObjectSubType::Eyes );
        break;
    }

    /* Check if item is tagged as "ToBlur" */
    if(destination->getItemType() == ObjectType::ToBlur)
    {
        /* Set item valid state */
        destination->setItemManualState( ObjectManualState::ToBlur );
    } else {

        /* Set item valid state */
        destination->setItemManualState( this->ui->validCheckBox->checkState() ? ObjectManualState::Valid : ObjectManualState::Invalid );
    }

    /* Set manual status */
    destination->setManualStatus( this->ui->validCheckBox->checkState() ? "Valid" : "Invalid" );

    /* Set blur */
    destination->setBlurred( this->ui->blurCheckBox->checkState() );
}

/* (UI signal) Cancel button clicked */
void EditView::on_cancelButton_clicked()
{
    /* Close window */
    this->close();
}

/* (UI signal) Delete button clicked */
void EditView::on_deleteButton_clicked()
{
    /* Window view mode switch */
    switch(this->mode)
    {

    /* Signle mode */
    case EditMode::Single:

        /* Remove item */
        this->item->remove( true );

        /* Refresh main window labels */
        emit refreshLabels();

        /* Close window */
        this->close();
        break;

    /* Scene mode */
    case EditMode::Scene:

        /* Remove parent panorama rect */
        pano_parent->rect_list.removeOne( this->ref_rect );

        /* Remove local rect */
        delete this->ref_rect;

        /* Refresh main window labels */
        emit refreshLabels();

        /* Close window */
        this->close();
        break;
    }
}

/* (UI signal) Confirm button clicked */
void EditView::on_confirmButton_clicked()
{
    /* Window view mode switch */
    switch(this->mode)
    {

    /* Signle mode */
    case EditMode::Single:

        /* Merge edited rect */
        this->ref_rect->mergeWith( this->rect_copy );

        /* Merge edited item */
        this->mergeEditedItem( this->item );

        /* Update item image */
        this->item->setImage( this->pano_parent->cropObject( this->rect_copy ) );
        break;

    /* Scene mode */
    case EditMode::Scene:

        /* Iterate over parent panorama viewer rects */
        foreach(ObjectRect* rect, this->pano_parent->rect_list)
        {
            /* If the object have the same id */
            if(rect->getId() == this->rect_copy->getId())
            {
                /* Merge rect */
                rect->mergeWith( this->rect_copy );

                /* Apply modifications to rect */
                this->mergeEditedRect( rect );
                break;
            }
        }

        /* Render parent panorama viewer */
        this->pano_parent->render();
        break;
    }

    /* Refresh main window labels */
    emit refreshLabels();

    /* Close window */
    this->close();
}

/* (UI signal) Valid checkbox checked */
void EditView::on_validCheckBox_clicked()
{
    /* Update rect state in current panorama viewer */
    this->rect_copy->setObjectManualState( this->ui->validCheckBox->checkState() ? ObjectManualState::Valid : ObjectManualState::Invalid );
}

/* (UI signal) Blur checkbox checked */
void EditView::on_blurCheckBox_clicked()
{
    /* Update rect blur state in current panorama viewer */
    this->rect_copy->setBlurred( this->ui->blurCheckBox->checkState() );
}
