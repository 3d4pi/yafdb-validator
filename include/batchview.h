#ifndef BATCHVIEW_H
#define BATCHVIEW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QShortcut>
#include <QMessageBox>
#include <QLayoutItem>
#include <QMouseEvent>

#include "flowlayout.h"
#include "panoramaviewer.h"
#include "objectrect.h"
#include "objectitem.h"
#include "panoramaviewer.h"

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
    QList<int> toremove_ids;

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

    void on_selectAllButton_clicked();

    void on_unSelectAllButton_clicked();

    void on_invertSelectionButton_clicked();

    void on_deleteButton_clicked();

    void on_setSubType_clicked();

private:
    Ui::BatchView *ui;

    PanoramaViewer* pano;

    void mergeResults();
    void populate(int batchviewmode);

    struct {
        bool CTRL;
    } pressed_keys;

protected:
    void wheelEvent(QWheelEvent* event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

signals:
    void refreshLabels();

};

#endif // BATCHVIEW_H
