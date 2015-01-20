#ifndef OBJECTITEM_H
#define OBJECTITEM_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>
#include "objectrect.h"
#include "panoramaviewer.h"

namespace Ui {
class ObjectItem;
}

class ObjectItem : public QWidget
{
    Q_OBJECT

public:
    explicit ObjectItem(QWidget *parent = 0);
    explicit ObjectItem(QWidget *parent, PanoramaViewer* pano, ObjectRect* rect);

    bool LoadImage(QString path);
    bool setImage(QImage image);

    void setId(int id);
    int  getId();
    void setSize(QSize size);

    void setItemType(int type);
    int  getItemType();
    void setItemSubType(int sub_type);
    int  getItemSubType();

    void setBlurred(bool value);
    void setSelected(bool value);

    void setRect(ObjectRect* rect);
    void setPano(PanoramaViewer* pano);

    void remove(bool value);

    void setItemManualState(int state);
    QString getItemManualStatus();
    void setItemAutomaticState(int state);
    QString getItemAutomaticStatus();

    void setAutomaticStatus(QString value);
    void setManualStatus(QString value);

    bool isValid();
    bool isSelected();
    bool isBlurred();
    bool toBeRemoved();

    ObjectRect* getParentRect();

    ~ObjectItem();

private:
    Ui::ObjectItem *ui;

    void mousePressEvent(QMouseEvent *ev);
    void mouseDoubleClickEvent ( QMouseEvent * event );

    int id;
    int item_type;
    int item_sub_type;
    int automatic_state;
    int manual_state;
    int border_size;

    bool selected;
    bool valid;
    bool blurred;
    bool needs_removal;

    QImage image;

    QString manualStatus;
    QString autoStatus;

    ObjectRect* rect;

    PanoramaViewer* parent_pano;

    QList<ObjectItem*> parent_elements;
    QList<int> parent_toremove_ids;

};

#endif // OBJECTITEM_H
