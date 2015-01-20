#ifndef ObjectRect_H
#define ObjectRect_H

/* Includes */
#include <QGraphicsPolygonItem>
#include <QPen>
#include <QPaintEvent>
#include <QDebug>

#include "etg_point.h"
#include "g2g_point.h"

/* Automatic states struct */
struct ObjectAutomaticState
{
    enum Type
    {
        /* Object is valid */
        Valid = 1,

        /* Object is invalid */
        Invalid = 2,

        /* Object has been manualy added */
        Manual = 3
    };
};

/* Manual states struct */
struct ObjectManualState
{
    enum Type
    {
        /* Not validated */
        None = 0,

        /* Object is valid */
        Valid = 1,

        /* Object is invalid */
        Invalid = 2,

        /* Object is to be blured */
        ToBlur = 3
    };
};

/* Object types struct */
struct ObjectType
{
    enum Type
    {
        None = 0, Face = 1, NumberPlate = 2, ToBlur = 3
    };
};

/* Object sub-types struct */
struct ObjectSubType
{
    enum Type
    {
        None = 0, Front = 1, Profile = 2, Back = 3, Top = 4, Eyes = 5
    };
};

/* Main class */
class ObjectRect : public QGraphicsPolygonItem
{

/* Public functions / variables */
public:

    /* Constructor */
    ObjectRect();

    /* Childrens container */
    QList<ObjectRect*> childrens;

    /* Points setters */
    void setPoint1(QPointF point);
    void setPoint2(QPointF point);
    void setPoint3(QPointF point);
    void setPoint3_Rigid(QPointF point);
    void setPoint3_Rigid(QPointF point, QPointF offset);
    void setPoint4(QPointF point);
    void setPoints(QPointF point1, QPointF point2, QPointF point3, QPointF point4);

    /* Points getters */
    QPointF getPoint1();
    QPointF getPoint2();
    QPointF getPoint3();
    QPointF getPoint4();
    QVector<QPointF> getPoints();

    /* Size setters */
    void setSize(QSizeF size);

    /* Size getters */
    QSizeF getSize();
    QSizeF getSizeCurrent();

    /* Object parameters setters */
    void setObjectAutomaticState(int state);
    void setObjectManualState(int state);
    void setObjectType(int value);

    /* Object parameters getters */
    int getObjectAutomaticState();
    int getObjectManualState();
    int getObjectType();

    /* SubType setter/getter */
    void setObjectSubType(int value);
    int getObjectSubType();

    /* Id setter/getter */
    void setId(int id);
    int getId();

    /* Projection paramaters setter */
    void setProjectionParametters(float width,
                                  float height,
                                  float azimuth,
                                  float elevation,
                                  float aperture);

    /* Source image setter/getter */
    void setSourceImagePath(QString path);
    QString getSourceImagePath();

    /* projection points setter/getter */
    void setProjectionPoints();
    void setProjectionPoints(QPointF p1,
                             QPointF p2,
                             QPointF p3,
                             QPointF p4);

    /* Projection parameters getters */
    float proj_azimuth();
    float proj_elevation();
    float proj_aperture();

    QPointF proj_point_1();
    QPointF proj_point_2();
    QPointF proj_point_3();
    QPointF proj_point_4();

    float proj_width();
    float proj_height();

    /* Function to move object to given points */
    void moveObject(QPointF pos,
                    QPointF offset_1,
                    QPointF offset_2,
                    QPointF offset_3,
                    QPointF offset_4);

    /* Function to simulate object moving, returns new corrdinates based on current scene parameters */
    QVector<QPointF> simulate_moveObject(QPointF pos,
                                         QPointF offset_1,
                                         QPointF offset_2,
                                         QPointF offset_3,
                                         QPointF offset_4);

    /* Function to get object's border width */
    float getBorderWidth();

    /* Function to copy object */
    ObjectRect* copy();

    /* Function to merge object with another */
    void mergeWith(ObjectRect* rect);

    /* Function to map object to specified projection paramaters */
    void mapTo(float width,
               float height,
               float azimuth,
               float elevation,
               float aperture);

    /* Function to convert spherical coordinates to gnomonic coordinates */
    void mapFromSpherical(float source_width,
                          float source_height,
                          float dest_width,
                          float dest_height,
                          float dest_azimuth,
                          float dest_elevation,
                          float dest_aperture,
                          float dest_zoom_min,
                          float dest_zoom_max);

    /* Function to determine if object is valid */
    bool isValidated();

    /* Blur setter/getter */
    void setBlurred(bool value);
    bool isBlurred();

    /* Manual status setter/getter */
    void setManualStatus(QString value);
    QString getManualStatus();

    /* Automatic status setter/getter */
    void setAutomaticStatus(QString value);
    QString getAutomaticStatus();

    /* Resize setter/getter */
    void setResizeEnabled(bool value);
    bool isResizeEnabled();

/* Private functions / variables */
private:

    /* Id container */
    int id;

    /* State container */
    int manual_state;

    /* Type containers */
    int automatic_state;
    int type;

    /* Resize enabled state variable */
    bool resizeEnabled;

    /* Contour objects */
    QGraphicsPolygonItem* contour;
    QGraphicsPolygonItem* contour2;

    /* Resized rect object */
    QGraphicsPolygonItem* resize_rect;

    /* Points container */
    QVector<QPointF> points;

    /* Polygon container */
    QPolygonF polygon;
    QPolygonF resize_rect_polygon;

    /* Pen containers */
    QPen* pen;
    QPen* contour_pen;
    QPen* contour2_pen;

    /* Brush container */
    QBrush* brush;

    /* Projection parameters structure */
    struct projection_parameters_struct{
        float azimuth;
        float elevation;
        float aperture;
        float width;
        float height;
        QVector<QPointF> points;
        QString source_image;
    } projection_parameters;

    /* Object infos structure */
    struct info_struct{
        int type;
        int sub_type;
        bool blurred;
        bool validated;
        QString manual_status;
        QString automatic_status;
    } info;

    /* Function to render object */
    void render();

};

#endif // ObjectRect_H
