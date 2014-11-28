#include "batchview.h"
#include "ui_batchview.h"

#include "objectitem.h"

BatchView::BatchView(QWidget *parent, PanoramaViewer* pano) :
    QMainWindow(parent),
    ui(new Ui::BatchView)
{
    ui->setupUi(this);

    FlowLayout * MainLayout = new FlowLayout();

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

    MainLayout->setContentsMargins( 10, 10, 10, 10 ) ;

    foreach(ObjectRect* rect, pano->rect_list )
    {
        ObjectItem* object = new ObjectItem();

        object->setImage(pano->cropObject(rect));
        object->setType(1);
        object->setBlurred(true);

        MainLayout->addWidget(object);
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
