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

    void setId(int id);
    void setSize(QSize size);
    void setType(int type);
    void setBlurred(bool value);
    void setSelected(bool value);
    void setValid(bool value);

    void setAutomaticStatus(QString value);
    void setManualStatus(QString value);

    bool selected;
    QImage image;

    QString manualStatus;
    QString autoStatus;

    int id;
    int type;
    bool valid;
    bool blurred;

    ~ObjectItem();

private:
    Ui::ObjectItem *ui;

    void mousePressEvent(QMouseEvent *ev);

};

#endif // OBJECTITEM_H
