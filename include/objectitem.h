#ifndef OBJECTITEM_H
#define OBJECTITEM_H

/* Includes */
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>
#include "objectrect.h"
#include "panoramaviewer.h"

namespace Ui {
class ObjectItem;
}

/* Main class */
class ObjectItem : public QWidget
{
    Q_OBJECT

/* Public functions / variables */
public:

    /* Constructors */
    explicit ObjectItem(QWidget *parent = 0);
    explicit ObjectItem(QWidget *parent, PanoramaViewer* pano, ObjectRect* rect);

    /* Destructor */
    ~ObjectItem();

    /* Set source image */
    bool setImage(QImage image);

    /* ID setter/getter */
    void setId(int id);
    int  getId();

    /* Function to change the tile size */
    void setSize(QSize size);

    /* Item type setter/getter */
    void setItemType(int type);
    int  getItemType();

    /* Item sub-type setter/getter */
    void setItemSubType(int sub_type);
    int  getItemSubType();

    /* Blurred flag setter/getter */
    void setBlurred(bool value);
    bool isBlurred();

    /* Selected flag setter/getter */
    void setSelected(bool value);
    bool isSelected();

    /* Function to assign a parent ObjectRect to item */
    void setParentRect(ObjectRect* rect);

    /* Function to get parent ObjectRect */
    ObjectRect* getParentRect();

    /* Function to assign parent PanoramaViewer */
    void setPano(PanoramaViewer* pano);

    /* Function to remove item */
    void remove(bool value);

    /* Manual state setter/getter */
    void setItemManualState(int state);
    QString getItemManualStatus();

    /* Automatic state setter/getter */
    void setItemAutomaticState(int state);
    QString getItemAutomaticStatus();

    /* Automatic status setter */
    void setAutomaticStatus(QString value);

    /* Manual status setter */
    void setManualStatus(QString value);

    /* Function to determine if object is valid */
    bool isValid();

    /* Function to determine if object is marked for removal */
    bool toBeRemoved();

/* Private functions / variables */
private:

    /* Main UI container */
    Ui::ObjectItem *ui;

    /* Item ID container */
    int id;

    /* Item type container */
    int item_type;

    /* Item sub-type container */
    int item_sub_type;

    /* Item automatic state container */
    int automatic_state;

    /* Item manual state container */
    int manual_state;

    /* Item border size container */
    int border_size;

    /* Selected status container */
    bool selected;

    /* Valid status container */
    bool valid;

    /* Blurred status container */
    bool blurred;

    /* "Tagged for removal ?" status container */
    bool needs_removal;

    /* Item tile image */
    QImage image;

    /* Manual status container */
    QString manualStatus;

    /* Automatic status container */
    QString autoStatus;

    /* Parent rect copy container */
    ObjectRect* parent_rect_copy;

    /* Parent PanoramaViewer container */
    PanoramaViewer* parent_pano;

/* Protected elements */
protected:

    /* Mouse press event */
    void mousePressEvent(QMouseEvent *ev);

    /* Mouse double click event */
    void mouseDoubleClickEvent ( QMouseEvent * event );

};

#endif // OBJECTITEM_H
