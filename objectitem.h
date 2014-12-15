#ifndef OBJECTITEM_H
#define OBJECTITEM_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>
#include "types.h"
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
    void setSize(QSize size);
    void setType(int type);
    void setBlurred(bool value);
    void setSelected(bool value);
    void setRect(ObjectRect* rect);
    void setPano(PanoramaViewer* pano);
    void remove(bool value);

    void setValidState(int state);

    void setRectType(int type);

    void setAutomaticStatus(QString value);
    void setManualStatus(QString value);

    bool selected;
    QImage image;

    QString manualStatus;
    QString autoStatus;

    int id;
    int type;
    int recttype;
    bool valid;
    int validstate;
    bool blurred;
    ObjectRect* rect;
    PanoramaViewer* parent_pano;
    QList<ObjectItem*> parent_elements;
    QList<int> parent_toremove_ids;
    bool toBeRemoved;

    ~ObjectItem();

private:
    Ui::ObjectItem *ui;

    void mousePressEvent(QMouseEvent *ev);
    void mouseDoubleClickEvent ( QMouseEvent * event );

    int border_size;

};

#endif // OBJECTITEM_H
