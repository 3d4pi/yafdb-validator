#ifndef OBJECTITEM_H
#define OBJECTITEM_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include "objectrect.h"
#include "types.h"

namespace Ui {
class ObjectItem;
}

class ObjectItem : public QWidget
{
    Q_OBJECT

public:
    explicit ObjectItem(QWidget *parent = 0);

    bool LoadImage(QString path);
    bool setImage(QImage image);

    void setSize(QSize size);
    void setType(int type);
    void setBlurred(bool value);
    void setSelected(bool value);

    void setAutomaticState(int state);
    void setManualState(int state);

    bool selected;
    QImage image;

    ~ObjectItem();

private:
    Ui::ObjectItem *ui;

    void mousePressEvent(QMouseEvent *ev);

};

#endif // OBJECTITEM_H
