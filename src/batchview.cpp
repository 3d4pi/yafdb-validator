#include "batchview.h"
#include "ui_batchview.h"

/* Constructor */
BatchView::BatchView(QWidget *parent, PanoramaViewer* pano, int batchmode, int batchviewmode) :
    QMainWindow(parent),
    ui(new Ui::BatchView)
{
    ui->setupUi(this);

    /* Default configurations */
    this->mode = BatchMode::Manual;
    this->pressed_keys.CTRL = false;

    /* Assign parent PanoramaViewer */
    this->pano = pano;

    /* Initialize new FlowLayout to display tiles */
    this->MainLayout = new FlowLayout();

    /* Set proper window margins */
    this->MainLayout->setContentsMargins( 10, 10, 10, 10 ) ;

    /* Register layout */
    QWidget * dummy = new QWidget;
    dummy->setLayout(MainLayout);
    this->ui->scrollArea->setWidget(dummy);

    /* Set window mode */
    this->setMode(batchmode);

    /* Populate window */
    this->populate(batchviewmode);

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

    /* Register keyboard shortcuts */
    new QShortcut(QKeySequence("Ctrl+a"), this, SLOT(selectAll()));
    new QShortcut(QKeySequence("Ctrl+d"), this, SLOT(unSelectAll()));
    new QShortcut(QKeySequence("Ctrl+i"), this, SLOT(invertSelection()));

    /* Disable firt element on the type selection dropdown "Select..." */
    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(this->ui->TypeList->model());
    QModelIndex firstIndex = model->index(0, this->ui->TypeList->modelColumn(),
                                          this->ui->TypeList->rootModelIndex());
    QStandardItem* firstItem = model->itemFromIndex(firstIndex);
    firstItem->setSelectable(false);

    /* Connect signal for labels refresh */
    connect(this, SIGNAL(refreshLabels()), parent, SLOT(refreshLabels()));
}

/* Destructor */
BatchView::~BatchView()
{
    /* Iterate over allocated tiles */
    foreach(ObjectItem* item, this->item_list)
    {
        /* Remove tile */
        this->item_list.removeOne( item );
        delete item;
    }

    delete ui;
}

/* Function to draw tiles */
void BatchView::populate(int batchviewmode)
{
    /* Iterate over tiles */
    foreach(ObjectRect* rect, pano->rect_list )
    {
        /* View mode switch */
        switch(batchviewmode)
        {
        /* All elements */
        case BatchViewMode::All:

            /* Insert tile */
            this->insertItem(rect);
            break;

        /* Only untyped items */
        case BatchViewMode::OnlyUntyped:

            /* Check if rect type is untyped */
            if(rect->getObjectType() == ObjectType::None)
            {
                /* Insert tile */
                this->insertItem(rect);
            }
            break;

        /* Only faces items */
        case BatchViewMode::OnlyFaces:

            /* Check if rect is a valid face */
            if(rect->getObjectType() == ObjectType::Face &&
                    (rect->getAutomaticStatus() == "Valid" || rect->getAutomaticStatus() == "None"))
            {
                /* Insert tile */
                this->insertItem(rect);
            }
            break;

        /* Only unnaproved faces */
        case BatchViewMode::OnlyUnapprovedFaces:

            /* Check if rect is unnaproved */
            if(rect->getObjectType() == ObjectType::Face
                    && rect->getManualStatus() == "None"
                    && rect->getAutomaticStatus() == "Valid")
            {
                /* Insert tile */
                this->insertItem(rect);
            }
            break;

        /* Only number plates */
        case BatchViewMode::OnlyNumberPlates:

            /* Check if rect is a numberplate */
            if(rect->getObjectType() == ObjectType::NumberPlate)
            {
                /* Insert tile */
                this->insertItem(rect);
            }
            break;

        /* Only unnaproved number plates */
        case BatchViewMode::OnlyUnapprovedNumberPlates:

            /* Check if rect is an unnaproved numberplate */
            if(rect->getObjectType() == ObjectType::NumberPlate
                    && rect->getManualStatus() == "None"
                    && rect->getAutomaticStatus() == "Valid")
            {
                /* Insert tile */
                this->insertItem(rect);
            }
            break;

        /* Only pre-invalidated items */
        case BatchViewMode::OnlyPreInvalidated:

            /* Check if rect is pre-invalidated */
            if(rect->getObjectType() != ObjectType::None
                    && rect->getAutomaticStatus() != "Valid"
                    && rect->getAutomaticStatus() != "None")
            {
                /* Insert tile */
                this->insertItem(rect);
            }
            break;

        /* Only "ToBlur" items */
        case BatchViewMode::OnlyToBlur:

            /* Check if rect is a "to-blur" object */
            if(rect->getObjectType() == ObjectType::ToBlur)
            {
                /* Insert tile */
                this->insertItem(rect);
            }
            break;
        }
    }

}

/* Function to insert a specified tile into view */
void BatchView::insertItem(ObjectRect *rect)
{

    /* Cheate a new tile */
    ObjectItem* object = new ObjectItem(0, this->pano, rect);

    /* Adjust tile size */
    object->setSize( QSize(this->ui->horizontalSlider->value(), this->ui->horizontalSlider->value()) );

    /* Append to list */
    this->item_list.append( object );

    /* Insert tile */
    this->MainLayout->addWidget(object);
    this->elements.append(object);
}

/* Function to set the window mode (manual objects, auto objects, etc) */
void BatchView::setMode(int mode)
{   
    /* Save imput mode */
    this->mode = mode;

    /* Input mod switch */
    switch(mode)
    {

    /* Display only manual objects */
    case BatchMode::Manual:

        /* Hide unneccesary controls */
        this->ui->InvalidateButton->setEnabled(false);
        this->ui->ValidateButton->setEnabled(false);
        break;

    /* Display only automatic objects */
    case BatchMode::Auto:

        /* Hide unneccesary controls */
        this->ui->setType->setEnabled(false);
        this->ui->TypeList->setEnabled(false);
        this->ui->setSubType->setEnabled(false);
        this->ui->SubTypeList->setEnabled(false);
        this->ui->deleteButton->setEnabled(false);
        break;

    /* Display only "ToBlur" objects */
    case BatchMode::ToBlur:

        /* Hide unneccesary controls */
        this->ui->setType->setEnabled(false);
        this->ui->TypeList->setEnabled(false);
        this->ui->InvalidateButton->setEnabled(false);
        this->ui->ValidateButton->setEnabled(false);
        this->ui->BlurButton->setEnabled(false);
        this->ui->NoBlurButton->setEnabled(false);
        break;
    }
}

/* Function to merge tiles to parent PanoramaViewer */
void BatchView::mergeResults()
{
    /* Iterate over objects */
    foreach(ObjectItem* item, this->elements )
    {
        /* Iterate over parent PanoramaViewer objects */
        foreach (ObjectRect* rect, this->pano->rect_list) {

            /* Check if object have the same id as parent object */
            if(rect->getId() == item->getId())
            {
                /* Merge object */
                rect->mergeWith( item->getParentRect() );
            }
        }
    }

    /* Delete requested objects */
    foreach(ObjectItem* item, this->elements )
    {
        /* Iterate over parent PanoramaViewer objects */
        foreach (ObjectRect* rect, this->pano->rect_list) {

            /* Check if object have the same id as parent object and need to be removed */
            if(item->toBeRemoved() && (item->getId() == rect->getId()))
            {
                /* Delete object from parent PanoramaViewer */
                this->pano->rect_list.removeOne( rect );

                /* Delete object */
                delete rect;
            }
        }
    }
}

/* (UI action) select all tiles */
void BatchView::selectAll()
{
    /* Iterate over objects */
    foreach(ObjectItem* item, this->elements )
    {
        /* Set tile selected */
        item->setSelected(true);
    }
}

/* (UI action) unselect all tiles */
void BatchView::unSelectAll()
{
    /* Iterate over objects */
    foreach(ObjectItem* item, this->elements )
    {
        /* Set tile unselected */
        item->setSelected(false);
    }
}

/* (UI action) invert all tiles selection */
void BatchView::invertSelection()
{
    /* Iterate over objects */
    foreach(ObjectItem* item, this->elements )
    {
        /* Invert tile selection */
        item->setSelected( !item->isSelected() );
    }
}

/* (UI signal) slider moved signal */
void BatchView::on_horizontalSlider_sliderMoved(int position)
{
    /* Iterate over objects */
    foreach(ObjectItem* item, this->elements )
    {
        /* Update tile size */
        item->setSize(QSize(position, position));
    }
}

/* (UI signal) Close button clicked signal */
void BatchView::on_CancelButton_clicked()
{
    /* Close widnow */
    this->close();
}

/* (UI signal) Blur button clicked signal */
void BatchView::on_BlurButton_clicked()
{
    /* Iterate over objects */
    foreach(ObjectItem* item, this->elements )
    {
        /* Check if item is selected */
        if(item->isSelected())
        {
            /* Mark object as blurred */
            item->setBlurred(true);
        }
    }
}

/* (UI signal) NoBlur button clicked signal */
void BatchView::on_NoBlurButton_clicked()
{
    /* Iterate over objects */
    foreach(ObjectItem* item, this->elements )
    {
        /* Check if item is selected */
        if(item->isSelected())
        {
            /* Mark object as unblurred */
            item->setBlurred(false);
        }
    }
}

/* (UI signal) Delete button clicked signal */
void BatchView::on_deleteButton_clicked()
{
    /* Iterate over objects */
    foreach(ObjectItem* item, this->elements )
    {
        /* Check if item is selected */
        if(item->isSelected())
        {
            /* Remove object */
            item->remove( true );
        }
    }
}

/* (UI signal) Validate button clicked signal */
void BatchView::on_ValidateButton_clicked()
{
    /* Iterate over objects */
    foreach(ObjectItem* item, this->elements )
    {
        /* Check if item is selected */
        if(item->isSelected())
        {
            /* Mark object as valid */
            item->setItemManualState( ObjectManualState::Valid );
            item->setManualStatus("Valid");
        }
    }
}

/* (UI signal) Invalidate button clicked signal */
void BatchView::on_InvalidateButton_clicked()
{
    /* Iterate over objects */
    foreach(ObjectItem* item, this->elements )
    {
        /* Check if item is selected */
        if(item->isSelected())
        {
            /* Mark object as invalid */
            item->setItemManualState( ObjectManualState::Invalid );
            item->setManualStatus("Invalid");
        }
    }
}

/* (UI signal) setType button clicked signal */
void BatchView::on_setType_clicked()
{
    /* Iterate over objects */
    foreach(ObjectItem* item, this->elements )
    {
        /* Check if item is selected */
        if(item->isSelected())
        {
            /* Update type */
            item->setItemType( this->ui->TypeList->currentIndex() );
        }
    }
}

/* (UI signal) setSubType button clicked signal */
void BatchView::on_setSubType_clicked()
{
    /* Iterate over objects */
    foreach(ObjectItem* item, this->elements )
    {
        /* Check if item is selected */
        if(item->isSelected())
        {
            /* Update sub type */
            item->setItemSubType( this->ui->SubTypeList->currentIndex() );
        }
    }
}

/* (UI signal) Apply button clicked signal */
void BatchView::on_ApplyButton_clicked()
{
    /* Local status variables */
    bool replyMSG = true;
    bool haveNoClass = false;
    bool haveNoSubClass = false;
    bool haveNoManualState = false;

    /* Iterate over elements */
    foreach(ObjectItem* item, this->elements)
    {
        /* Check if object is untyped */
        if(item->getItemType() == ObjectType::None)
        {
            /* Assign flag */
            haveNoClass = true;
            break;
        }
    }

    /* Iterate over elements */
    foreach(ObjectItem* item, this->elements)
    {
        /* Check if object is a face */
        if(item->getItemType() == ObjectType::Face)
        {
            /* Check if object sub type is untyped */
            if(item->getItemSubType() == ObjectSubType::None)
            {
                /* Assign flag */
                haveNoSubClass = true;
                break;
            }
        }
    }

    /* Iterate over elements */
    foreach(ObjectItem* item, this->elements)
    {
        /* Check if object is unnaproved */
        if(item->getManualStatus() == "None")
        {
            /* Assign flag */
            haveNoManualState = true;
            break;
        }
    }

    /* Check if object is untyped */
    if(haveNoClass)
    {
        /* Display warning message */
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Warning", "Not all objects have a class defined, quit anyway?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No) {
            replyMSG = false;
        }

        /* Check message answer */
        if(replyMSG)
        {
            /* Merge elements with parent PanoramaViewer */
            this->mergeResults();

            /* Render parent PanoramaViewer */
            this->pano->render();

            /* Refresh main window status labels */
            emit refreshLabels();

            /* Close window */
            this->close();
        }

    /* Check if object is untyped and unapproved */
    } else if(!haveNoClass && haveNoManualState) {

        /* Display warning message */
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Warning", "Not all objects have been validated, quit anyway?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No) {
            replyMSG = false;
        }

        /* Check message answer */
        if(replyMSG)
        {
            /* Merge elements with parent PanoramaViewer */
            this->mergeResults();

            /* Render parent PanoramaViewer */
            this->pano->render();

            /* Refresh main window status labels */
            emit refreshLabels();

            /* Close window */
            this->close();
        }

    /* Check if object have no sub class and mode is manual */
    } else if( haveNoSubClass && ( this->mode == BatchMode::Manual ) ) {

        /* Display warning message */
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Warning", "Not all objects have a sub type defined, quit anyway?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No) {
            replyMSG = false;
        }

        /* Check message answer */
        if(replyMSG)
        {
            /* Merge elements with parent PanoramaViewer */
            this->mergeResults();

            /* Render parent PanoramaViewer */
            this->pano->render();

            /* Refresh main window status labels */
            emit refreshLabels();

            /* Close window */
            this->close();
        }
    } else {

        /* Merge elements with parent PanoramaViewer */
        this->mergeResults();

        /* Render parent PanoramaViewer */
        this->pano->render();

        /* Refresh main window status labels */
        emit refreshLabels();

        /* Close window */
        this->close();
    }

}

/* (UI signal) selectAll button clicked signal */
void BatchView::on_selectAllButton_clicked()
{
    /* Select all tiles */
    this->selectAll();
}

/* (UI signal) unSelectAll button clicked signal */
void BatchView::on_unSelectAllButton_clicked()
{
    /* Unselect all tiles */
    this->unSelectAll();
}

/* (UI signal) invertSelection button clicked signal */
void BatchView::on_invertSelectionButton_clicked()
{
    /* Invert all selected tiles */
    this->invertSelection();
}

/* Mouse wheel event handling function */
void BatchView::wheelEvent(QWheelEvent * event)
{
    /* Compute wheel delta */
    int delta = (event->delta() / 120) * 10;

    /* Check if CTRL key is pressed */
    if( this->pressed_keys.CTRL )
    {
        /* Retrieve value from slider and append delta to it */
        int newvalue = (this->ui->horizontalSlider->value() + delta);

        /* Value clamping */
        if (newvalue < this->ui->horizontalSlider->minimum()
                || newvalue > this->ui->horizontalSlider->maximum())
        {
            return;
        }

        /* Update slider value */
        this->ui->horizontalSlider->setValue( newvalue );

        /* Iterate over objects */
        foreach(ObjectItem* item, this->elements )
        {
            /* Update tile size */
            item->setSize(QSize(newvalue, newvalue));
        }
    }
}

/* Key press event handling function */
void BatchView::keyPressEvent(QKeyEvent *event)
{
    /* Check if CTRL key is pressed and assign flag */
    if(event->key() == Qt::Key_Control)
        this->pressed_keys.CTRL = true;
}

/* Key release event handling function */
void BatchView::keyReleaseEvent(QKeyEvent *)
{
    /* Unnasign tag */
    this->pressed_keys.CTRL = false;
}
