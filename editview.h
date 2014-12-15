#ifndef EDITVIEW_H
#define EDITVIEW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QThread>
#include <QDebug>
#include <QStandardItemModel>

#include "objectitem.h"
#include "g2g_point.h"
#include "objectrect.h"
#include "panoramaviewer.h"
#include "types.h"

namespace Ui {
class EditView;
}

class EditView : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditView(QWidget *parent, ObjectRect* rect, ObjectItem* item, int mode = EditMode::Scene);
    ~EditView();

private slots:

    void on_cancelButton_clicked();

    void on_deleteButton_clicked();

    void on_confirmButton_clicked();

    void on_validCheckBox_clicked();

private:
    Ui::EditView *ui;

    PanoramaViewer* pano;
    ObjectRect* ref_rect;
    ObjectItem* item;
    PanoramaViewer* pano_parent;
    ObjectRect* rect_copy;
    int mode;

    void mergeEditedRect(ObjectRect* destination);

signals:
    void refreshLabels();

};

#endif // EDITVIEW_H
