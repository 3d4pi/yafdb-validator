#ifndef BATCHVIEW_H
#define BATCHVIEW_H

/* Imports */
#include <QMainWindow>
#include <QDesktopWidget>
#include <QShortcut>
#include <QMessageBox>
#include <QLayoutItem>
#include <QMouseEvent>

#include "flowlayout.h"
#include "objectrect.h"
#include "objectitem.h"
#include "panoramaviewer.h"

/* Batch modes struct */
struct BatchMode
{
    enum Type
    {
        /* Show only manualy added objects */
        Manual = 0,

        /* Show only automaticaly detected objects */
        Auto = 1,

        /* Show only "ToBlur" class objects */
        ToBlur = 3
    };
};

/* Batch view modes struct */
struct BatchViewMode
{
    enum Type
    {
        /* Show all objects */
        All = 0,

        /* Show only untyped objects */
        OnlyUntyped = 1,

        /* Show only Faces */
        OnlyFaces = 2,

        /* Show only unnaproved faces */
        OnlyUnapprovedFaces = 3,

        /* Show only numberplates */
        OnlyNumberPlates = 4,

        /* Shw only unnaproved numberplates */
        OnlyUnapprovedNumberPlates = 5,

        /* Show only pre-invalidated objects */
        OnlyPreInvalidated = 6,

        /* Show only "ToBlur" class objects */
        OnlyToBlur = 7
    };
};

/* Default class container */
namespace Ui {
class BatchView;
}

/* Main class */
class BatchView : public QMainWindow
{
    Q_OBJECT

/* Public functions / variables */
public:

    /* Constructor */
    explicit BatchView(QWidget *parent, PanoramaViewer* pano, int batchmode, int batchviewmode);

    /* Destructor */
    ~BatchView();

    /* Function to set the window mode (manual objects, auto objects, etc) */
    void setMode(int mode);

/* Private slots */
private slots:

    /* UI components signal functions */
    void on_horizontalSlider_sliderMoved(int position);
    void on_CancelButton_clicked();
    void on_NoBlurButton_clicked();
    void on_BlurButton_clicked();
    void on_ValidateButton_clicked();
    void on_InvalidateButton_clicked();
    void on_setType_clicked();
    void on_ApplyButton_clicked();
    void on_selectAllButton_clicked();
    void on_unSelectAllButton_clicked();
    void on_invertSelectionButton_clicked();
    void on_deleteButton_clicked();
    void on_setSubType_clicked();

    /* Local UI actions */
    void selectAll();
    void unSelectAll();
    void invertSelection();

/* Private functions / variables */
private:

    /* Main UI container */
    Ui::BatchView *ui;

    /* View mode container */
    int mode;

    /* Main PanoramaViewer container */
    PanoramaViewer* pano;

    /* Main layout container to store/display tiles */
    FlowLayout * MainLayout;

    /* Main tiles list */
    QList<ObjectItem*> elements;

    /* List of id's to be removed */
    QList<int> toremove_ids;

    /* List of allocated tiles */
    QList<ObjectItem*> item_list;

    /* Key statuses container structure */
    struct pressed_keys_struct{
        bool CTRL;
    } pressed_keys;

    /* Function to insert a specified tile into view */
    void insertItem(ObjectRect* rect);

    /* Function to merge tiles to parent PanoramaViewer */
    void mergeResults();

    /* Function to draw tiles */
    void populate(int batchviewmode);

protected:

    /* Mouse wheel event handling function */
    void wheelEvent(QWheelEvent* event);

    /* Key press event handling function */
    void keyPressEvent(QKeyEvent *event);

    /* Key release event handling function */
    void keyReleaseEvent(QKeyEvent *event);

signals:

    /* Signal to refresh parent window status labels */
    void refreshLabels();

};

#endif // BATCHVIEW_H
