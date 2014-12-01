#ifndef BATCHVIEW_H
#define BATCHVIEW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <flowlayout.h>
#include "panoramaviewer.h"
#include "objectrect.h"
#include "objectitem.h"

namespace Ui {
class BatchView;
}

class BatchView : public QMainWindow
{
    Q_OBJECT

public:
    explicit BatchView(QWidget *parent, PanoramaViewer* pano);
    ~BatchView();

    FlowLayout * MainLayout;
    QList<ObjectItem*> elements;


private slots:
    void on_horizontalSlider_sliderMoved(int position);

    void on_CancelButton_clicked();

    void on_NoBlurButton_clicked();

    void on_BlurButton_clicked();

private:
    Ui::BatchView *ui;
};

#endif // BATCHVIEW_H
