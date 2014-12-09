#ifndef OBJECTITEM_H
#define OBJECTITEM_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
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

    ~ObjectItem();

private:
    Ui::ObjectItem *ui;

    void mousePressEvent(QMouseEvent *ev);

    int border_size;

};

#endif // OBJECTITEM_H
