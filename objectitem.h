#ifndef OBJECTITEM_H
#define OBJECTITEM_H

#include <QWidget>
#include <QImage>
#include <QPixmap>

struct ObjectType
{
    enum Type
    {
        Face = 1, NumberPlate = 2
    };
};

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

    void setType(int type);
    void setBlurred(bool value);
    void setSelected(bool value);

    bool selected;

    ~ObjectItem();

private:
    Ui::ObjectItem *ui;

    void mousePressEvent(QMouseEvent *ev);

};

#endif // OBJECTITEM_H
