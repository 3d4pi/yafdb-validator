#include "batchview.h"
#include "ui_batchview.h"

#include "objectitem.h"
#include <QLayoutItem>

BatchView::BatchView(QWidget *parent, PanoramaViewer* pano) :
    QMainWindow(parent),
    ui(new Ui::BatchView)
{
    ui->setupUi(this);

    this->MainLayout = new FlowLayout();

    // Remove margins
    //this->setContentsMargins(-5, -5, -5, -5);
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

    this->MainLayout->setContentsMargins( 10, 10, 10, 10 ) ;

    foreach(ObjectRect* rect, pano->rect_list )
    {
        ObjectItem* object = new ObjectItem();

        object->setImage(pano->cropObject(rect));
        object->setType(1);
        object->setBlurred(true);

        this->MainLayout->addWidget(object);
        this->elements.append(object);
    }

    /*
    QDirIterator dirIt("/home/f0x/Bureau/FaceTiles");
    while (dirIt.hasNext()) {
        dirIt.next();
        if (QFileInfo(dirIt.filePath()).isFile())
            if (QFileInfo(dirIt.filePath()).suffix() == "png")
            {
                ObjectItem* object = new ObjectItem();

                object->LoadImage(dirIt.filePath());
                object->setType(1);
                object->setBlurred(true);

                MainLayout->addWidget(object);
            }
    }*/

    QWidget * dummy = new QWidget();
    dummy->setLayout(MainLayout);

    this->ui->scrollArea->setWidget(dummy);
}

BatchView::~BatchView()
{
    delete ui;
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
