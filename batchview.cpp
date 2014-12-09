#include "batchview.h"
#include "ui_batchview.h"

#include "objectitem.h"
#include <QLayoutItem>

BatchView::BatchView(QWidget *parent, PanoramaViewer* pano, int batchmode, int batchviewmode) :
    QMainWindow(parent),
    ui(new Ui::BatchView)
{
    ui->setupUi(this);

    this->pano = pano;

    this->MainLayout = new FlowLayout();

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

    this->MainLayout->setContentsMargins( 10, 10, 10, 10 ) ;

    this->setMode(batchmode);
    this->populate(batchviewmode);

    QWidget * dummy = new QWidget();
    dummy->setLayout(MainLayout);

    this->ui->scrollArea->setWidget(dummy);

    // Keyboard shortcuts
    new QShortcut(QKeySequence("Ctrl+a"), this, SLOT(selectAll()));
    new QShortcut(QKeySequence("Ctrl+d"), this, SLOT(unSelectAll()));
    new QShortcut(QKeySequence("Ctrl+i"), this, SLOT(invertSelection()));

    QStandardItemModel* model =
            qobject_cast<QStandardItemModel*>(this->ui->TypeList->model());
    QModelIndex firstIndex = model->index(0, this->ui->TypeList->modelColumn(),
            this->ui->TypeList->rootModelIndex());
    QStandardItem* firstItem = model->itemFromIndex(firstIndex);
    firstItem->setSelectable(false);

    // Connect signal for labels refresh
    connect(this, SIGNAL(refreshLabels()), parent, SLOT(refreshLabels()));
}

void BatchView::populate(int batchviewmode)
{

    foreach(ObjectRect* rect, pano->rect_list )
    {
        switch(batchviewmode)
        {
            case BatchViewMode::All:
                this->insertItem(rect);
                break;
            case BatchViewMode::OnlyUntyped:
                if(rect->getType() == ObjectType::None)
                {
                    this->insertItem(rect);
                }
                break;
            case BatchViewMode::OnlyFaces:
                if(rect->getType() == ObjectType::Face &&
                        (rect->getAutomaticStatus() == "Valid" || rect->getAutomaticStatus() == "None"))
                {
                    this->insertItem(rect);
                }
                break;
            case BatchViewMode::OnlyUnapprovedFaces:
                if(rect->getType() == ObjectType::Face
                        && rect->getManualStatus() == "None"
                        && rect->getAutomaticStatus() == "Valid")
                {
                    this->insertItem(rect);
                }
                break;
            case BatchViewMode::OnlyNumberPlates:
                if(rect->getType() == ObjectType::NumberPlate)
                {
                    this->insertItem(rect);
                }
                break;
            case BatchViewMode::OnlyUnapprovedNumberPlates:
                if(rect->getType() == ObjectType::NumberPlate
                        && rect->getManualStatus() == "None"
                        && rect->getAutomaticStatus() == "Valid")
                {
                    this->insertItem(rect);
                }
                break;
            case BatchViewMode::OnlyPreInvalidated:
                if(rect->getType() != ObjectType::None
                        && rect->getAutomaticStatus() != "Valid"
                        && rect->getAutomaticStatus() != "None")
                {
                    this->insertItem(rect);
                }
                break;
        }
    }

}

void BatchView::insertItem(ObjectRect *rect)
{
    ObjectItem* object = new ObjectItem();
    object->setId(rect->getId());
    object->setImage(pano->cropObject(rect));
    object->setType(rect->getType());
    object->setBlurred(rect->isBlurred());
    object->setValidState(rect->isValidated());
    object->setManualStatus(rect->getManualStatus());
    object->setAutomaticStatus(rect->getAutomaticStatus());

    if(rect->getAutomaticStatus() != "None")
    {
        if(rect->getAutomaticStatus() == "Valid")
        {
            object->setRectType(ObjectItemRectType::Valid);
        } else {
            object->setRectType(ObjectItemRectType::Invalid);
        }
    } else {
        object->setRectType(ObjectItemRectType::Manual);
    }

    this->MainLayout->addWidget(object);
    this->elements.append(object);
}

BatchView::~BatchView()
{
    delete ui;
}

void BatchView::setMode(int mode)
{
    switch(mode)
    {
        case BatchMode::Manual:
            this->ui->setType->setEnabled(true);
            this->ui->TypeList->setEnabled(true);
            this->ui->InvalidateButton->setEnabled(false);
            this->ui->ValidateButton->setEnabled(false);
            break;
        case BatchMode::Auto:
            this->ui->setType->setEnabled(false);
            this->ui->TypeList->setEnabled(false);
            this->ui->InvalidateButton->setEnabled(true);
            this->ui->ValidateButton->setEnabled(true);
            break;
    }
}

void BatchView::selectAll()
{
    foreach(ObjectItem* item, this->elements )
    {
        item->setSelected(true);
    }
}

void BatchView::unSelectAll()
{
    foreach(ObjectItem* item, this->elements )
    {
        item->setSelected(false);
    }
}

void BatchView::invertSelection()
{
    foreach(ObjectItem* item, this->elements )
    {
        item->setSelected( !item->selected );
    }
}

void BatchView::on_horizontalSlider_sliderMoved(int position)
{
    foreach(ObjectItem* item, this->elements )
    {
        item->setSize(QSize(position, position));
    }
}

void BatchView::on_CancelButton_clicked()
{
    this->close();
}

void BatchView::on_NoBlurButton_clicked()
{
    foreach(ObjectItem* item, this->elements )
    {
        if(item->selected)
        {
            item->setBlurred(false);
        }
    }
}

void BatchView::on_BlurButton_clicked()
{
    foreach(ObjectItem* item, this->elements )
    {
        if(item->selected)
        {
            item->setBlurred(true);
        }
    }
}

void BatchView::on_ValidateButton_clicked()
{
    foreach(ObjectItem* item, this->elements )
    {
        if(item->selected)
        {
            item->setValidState(ObjectValidState::Valid);
            item->setManualStatus("Valid");
        }
    }
}

void BatchView::on_InvalidateButton_clicked()
{
    foreach(ObjectItem* item, this->elements )
    {
        if(item->selected)
        {
            item->setValidState(ObjectValidState::Invalid);
            item->setManualStatus("Invalid");
        }
    }
}

void BatchView::on_setType_clicked()
{
    foreach(ObjectItem* item, this->elements )
    {
        if(item->selected)
        {
            item->setType(this->ui->TypeList->currentIndex());
        }
    }
}

void BatchView::mergeResults()
{
    foreach(ObjectItem* item, this->elements )
    {
        foreach (ObjectRect* rect, this->pano->rect_list) {
            if(rect->getId() == item->id)
            {

                rect->setValidated(item->validstate);
                rect->setBlurred(item->blurred);
                rect->setType(item->type);
                rect->setManualStatus(item->manualStatus);
            }
        }
    }
}

void BatchView::on_ApplyButton_clicked()
{
    bool replyMSG = true;
    bool haveNoClass = false;
    bool haveNoManualState = false;

    foreach(ObjectItem* item, this->elements)
    {
        if(item->type == ObjectType::None)
        {
            haveNoClass = true;
            break;
        }
    }

    foreach(ObjectItem* item, this->elements)
    {
        if(item->manualStatus == "None")
        {
            haveNoManualState = true;
            break;
        }
    }

    if(haveNoClass)
    {
        QMessageBox::StandardButton reply;
         reply = QMessageBox::question(this, "Warning", "Not all objects have a class defined, quit anyway?",
                                       QMessageBox::Yes|QMessageBox::No);
         if (reply == QMessageBox::No) {
           replyMSG = false;
         }

        if(replyMSG)
        {
            this->mergeResults();
            emit refreshLabels();
            this->close();
        }
    } else if(!haveNoClass && haveNoManualState) {
        QMessageBox::StandardButton reply;
         reply = QMessageBox::question(this, "Warning", "Not all objects have been validated, quit anyway?",
                                       QMessageBox::Yes|QMessageBox::No);
         if (reply == QMessageBox::No) {
           replyMSG = false;
         }

        if(replyMSG)
        {
            this->mergeResults();
            emit refreshLabels();
            this->close();
        }
    } else {
        this->mergeResults();
        emit refreshLabels();
        this->close();
    }

}
