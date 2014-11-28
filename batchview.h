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

private:
    Ui::BatchView *ui;
};

#endif // BATCHVIEW_H
