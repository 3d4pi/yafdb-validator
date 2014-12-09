#ifndef BATCHVIEW_H
#define BATCHVIEW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QShortcut>
#include <QMessageBox>
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
    explicit BatchView(QWidget *parent, PanoramaViewer* pano, int batchmode, int batchviewmode);
    ~BatchView();

    FlowLayout * MainLayout;
    QList<ObjectItem*> elements;

    void setMode(int mode);
    void insertItem(ObjectRect* rect);


private slots:
    void on_horizontalSlider_sliderMoved(int position);

    void on_CancelButton_clicked();

    void on_NoBlurButton_clicked();

    void on_BlurButton_clicked();

    void selectAll();
    void unSelectAll();
    void invertSelection();

    void on_ValidateButton_clicked();

    void on_InvalidateButton_clicked();

    void on_setType_clicked();

    void on_ApplyButton_clicked();

private:
    Ui::BatchView *ui;

    PanoramaViewer* pano;

    void mergeResults();
    void populate(int batchviewmode);

signals:
    void refreshLabels();

};

#endif // BATCHVIEW_H
