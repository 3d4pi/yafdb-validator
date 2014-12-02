#include "batchview.h"
#include "ui_batchview.h"

#include "objectitem.h"
#include <QLayoutItem>

BatchView::BatchView(QWidget *parent, PanoramaViewer* pano) :
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

    foreach(ObjectRect* rect, pano->rect_list )
    {
        ObjectItem* object = new ObjectItem();

        object->setId(rect->id);
        object->setImage(pano->cropObject(rect));
        object->setType(rect->type);
        object->setBlurred(rect->blurred);
        object->setValid(rect->valid);

        this->MainLayout->addWidget(object);
        this->elements.append(object);
    }

    QWidget * dummy = new QWidget();
    dummy->setLayout(MainLayout);

    this->ui->scrollArea->setWidget(dummy);

    // Keyboard shortcuts
    new QShortcut(QKeySequence("Ctrl+a"), this, SLOT(selectAll()));
    new QShortcut(QKeySequence("Ctrl+d"), this, SLOT(unSelectAll()));
    new QShortcut(QKeySequence("Ctrl+i"), this, SLOT(invertSelection()));

}

BatchView::~BatchView()
{
    delete ui;
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
            item->setValid(true);
        }
    }
}

void BatchView::on_InvalidateButton_clicked()
{
    foreach(ObjectItem* item, this->elements )
    {
        if(item->selected)
        {
            item->setValid(false);
        }
    }
}

void BatchView::on_setType_clicked()
{
    foreach(ObjectItem* item, this->elements )
    {
        if(item->selected)
        {
            item->setType(this->ui->TypeList->currentIndex() + 1);
        }
    }
}

void BatchView::on_ApplyButton_clicked()
{
    foreach(ObjectItem* item, this->elements )
    {
        foreach (ObjectRect* rect, this->pano->rect_list) {
            if(rect->id == item->id)
            {
                rect->setValid(item->valid);
                rect->setBlurred(item->blurred);
                rect->setType(item->type);
            }
        }
    }

    this->close();
}
