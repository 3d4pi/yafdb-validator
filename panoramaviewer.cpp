#include "panoramaviewer.h"
#include <QDebug>
#include <QApplication>
#include <QGraphicsProxyWidget>
#include <QToolTip>
#include <QPushButton>

#define CHANNELS_COUNT 4

PanoramaViewer::PanoramaViewer(QWidget *parent) :
    QGraphicsView(parent)
{

    //this->centralWidget()->layout()->setContentsMargins(50,50,50,50);

    // Remove side bars on the panorama viewer
    this->verticalScrollBar()->blockSignals(true);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->horizontalScrollBar()->blockSignals(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Initialize position container
    this->position.start_x = 0;
    this->position.start_y = 0;
    this->position.x = 0;
    this->position.y = 0;
    this->position.start_azimuth = 0.0;
    this->position.start_elevation = 0.0;
    this->position.azimuth = 0.0;
    this->position.elevation = 0.0;
    this->position.old_azimuth = 0.0;
    this->position.old_elevation = 0.0;

    // Initialize default mode
    this->mode = Mode::None;

    // Configure default settings
    this->scale_factor = 1.0;
    this->zoom_min = 20.0;
    this->zoom_max = 120.0;
    this->position.aperture_delta = 100.0;
    this->position.aperture = ( this->position.aperture_delta * ( LG_PI / 180.0 ) );;
    this->position.old_aperture = this->position.aperture;
    this->threads_count = 1;
    this->pixmap_initialized = false;
    this->create_position.rect = NULL;
    this->selected_rect = NULL;

    // Create default scene
    this->scene = new QGraphicsScene();
    this->setScene(this->scene);

    // Connect signal for labels refresh
    connect(this, SIGNAL(refreshLabels()), parent, SLOT(refreshLabels()));

    this->polytest = new ObjectRect2();
    this->scene->addItem(this->polytest);

}

inline float clamp(float x, float a, float b)
{
    return x < a ? a : (x > b ? b : x);
}

float clampRad(float x, float a, float b)
{
    if(x > b)
    {
        float delta = (x - b);
        return (a + delta);
    } else if (x < a) {
        float delta = (a - x);
        return (b - delta);
    } else {
        return x;
    }
}

void PanoramaViewer::setup(float scale_factor, float zoom_min, float zoom_max, float zoom_def, int threads)
{
    this->scale_factor = scale_factor;
    this->zoom_min = zoom_min;
    this->zoom_max = zoom_max;
    this->position.aperture_delta = zoom_def;
    this->position.aperture = (zoom_def * (LG_PI / 180.0) );
    this->threads_count = threads;
}

void PanoramaViewer::loadImage(QString path)
{

    this->src_image.load(path);
    this->src_image_map = QPixmap::fromImage(this->src_image);

    this->render();

}

void PanoramaViewer::updateScene(float azimuth, float elevation, float zoom)
{

    int WIDTH = this->width();
    int HEIGHT = this->height();

    int DEST_WIDTH = WIDTH * scale_factor;
    int DEST_HEIGHT = HEIGHT * scale_factor;

    float AZIMUTH = clampRad(azimuth, -360.0, 360.0);
    float ELEVATION = clamp(elevation, -90.0, 90.0);

    this->dest_image = QImage (DEST_WIDTH, DEST_HEIGHT, QImage::Format_RGB32);

    lg_etg_apperturep(

        ( inter_C8_t * ) this->src_image.bits(),
        this->src_image.width(),
        this->src_image.height(),
        CHANNELS_COUNT,
        ( inter_C8_t * ) dest_image.bits(),
        DEST_WIDTH,
        DEST_HEIGHT,
        CHANNELS_COUNT,
        AZIMUTH,
        ELEVATION,
        0.0,
        zoom,
        li_bilinearf,
        this->threads_count

    );

    this->dest_image_map = QPixmap::fromImage(this->dest_image);

    if(this->pixmap_initialized)
    {
        this->scene->removeItem(this->last_pixmap);
        delete this->last_pixmap;
    } else {
        this->pixmap_initialized = true;
    }

    this->last_pixmap = this->scene->addPixmap(this->dest_image_map);
    this->last_pixmap->setZValue(-1);

    this->scene->setSceneRect(this->dest_image_map.rect());
    this->fitInView(this->dest_image_map.rect());

}

void PanoramaViewer::render()
{
    qDebug() << "this->position.old_azimuth: " << this->position.old_azimuth;
    qDebug() << "this->position.old_elevation: " << this->position.old_elevation;
    qDebug() << "this->position.old_aperture: " << this->position.old_aperture;
    qDebug() << "this->position.azimuth: " << this->position.azimuth;
    qDebug() << "this->position.elevation: " << this->position.elevation;
    qDebug() << "this->position.aperture: " << this->position.aperture;
    qDebug() << "";

    foreach(ObjectRect2* rect, this->rect_list_v2)
    {
        double p1_x = 0.0;
        double p1_y = 0.0;
        double p2_x = 0.0;
        double p2_y = 0.0;
        double p3_x = 0.0;
        double p3_y = 0.0;
        double p4_x = 0.0;
        double p4_y = 0.0;

        int state = g2g_point(this->dest_image_map.width(),
                  this->dest_image_map.height(),
                  this->position.old_azimuth,
                  this->position.old_elevation,
                  this->position.old_aperture,
                  rect->getPoint1().x(),
                  rect->getPoint1().y(),

                  this->dest_image_map.width(),
                  this->dest_image_map.height(),
                  this->position.azimuth,
                  this->position.elevation,
                  this->position.aperture,
                  &p1_x,
                  &p1_y);

        int state2 = g2g_point(this->dest_image_map.width(),
                  this->dest_image_map.height(),
                  this->position.old_azimuth,
                  this->position.old_elevation,
                  this->position.old_aperture,
                  rect->getPoint2().x(),
                  rect->getPoint2().y(),

                  this->dest_image_map.width(),
                  this->dest_image_map.height(),
                  this->position.azimuth,
                  this->position.elevation,
                  this->position.aperture,
                  &p2_x,
                  &p2_y);

        int state3 = g2g_point(this->dest_image_map.width(),
                  this->dest_image_map.height(),
                  this->position.old_azimuth,
                  this->position.old_elevation,
                  this->position.old_aperture,
                  rect->getPoint3().x(),
                  rect->getPoint3().y(),

                  this->dest_image_map.width(),
                  this->dest_image_map.height(),
                  this->position.azimuth,
                  this->position.elevation,
                  this->position.aperture,
                  &p3_x,
                  &p3_y);

        int state4 = g2g_point(this->dest_image_map.width(),
                  this->dest_image_map.height(),
                  this->position.old_azimuth,
                  this->position.old_elevation,
                  this->position.old_aperture,
                  rect->getPoint4().x(),
                  rect->getPoint4().y(),

                  this->dest_image_map.width(),
                  this->dest_image_map.height(),
                  this->position.azimuth,
                  this->position.elevation,
                  this->position.aperture,
                  &p4_x,
                  &p4_y);

        if(state == 1 && state2 == 1 && state3 == 1 && state4 == 1)
        {
            rect->setVisible( true );

            rect->setPoints(QPointF( p1_x, p1_y ),
                            QPointF( p2_x, p2_y ),
                            QPointF( p3_x, p3_y ),
                            QPointF( p4_x, p4_y ));
        } else {
            rect->setVisible( false );
        }
    }

    this->updateScene(
        this->position.azimuth,
        this->position.elevation,
        this->position.aperture
    );

}

// Function to update view of current scene
void PanoramaViewer::setView(float azimuth, float elevation)
{
    // Update view
    this->position.azimuth = azimuth;
    this->position.elevation = elevation;

    // Render scene
    this->render();
}

// Function to update zoom of current scene
void PanoramaViewer::setZoom(float zoom_level)
{

    // Convert zoom value to radians
    this->position.old_aperture = this->position.aperture;
    this->position.aperture = ( zoom_level * ( LG_PI / 180.0 ) );

    // Render scene
    this->render();
}

// Mouse wheel handler
void PanoramaViewer::wheelEvent(QWheelEvent* event)
{
    // Determine delta
    int delta = (event->delta() / 120);

    // Update current zoom level
    float old_zoom = this->position.aperture_delta;
    this->position.aperture_delta -= (delta * 1.5);
    this->position.aperture_delta = clamp(this->position.aperture_delta, this->zoom_min, this->zoom_max);

    // Apply zoom level
    if(this->position.aperture_delta != old_zoom)
        this->setZoom(this->position.aperture_delta);
}

// Mouse buttons click handler
void PanoramaViewer::mousePressEvent(QMouseEvent* event)
{
    // Store mouse coords
    int MouseX = event->x();
    int MouseY = event->y();

    // Store normalized mouse coords
    QPointF mouse_scene = this->mapToScene(event->pos());

    // Check presence of left click
    if(event->buttons() & Qt::LeftButton)
    {
        // Enable mouse tracking
        this->setMouseTracking(true);

        // Switch in mooving mode
        this->mode = Mode::Move;

        // Store base positions (Used to determine offset to move in panorama later)
        this->position.start_x = MouseX;
        this->position.start_y = MouseY;

         // Store base directions (Used to determine offset to move in panorama later)
        this->position.start_azimuth = this->position.azimuth / (LG_PI / 180.0);
        this->position.start_elevation = this->position.elevation / (LG_PI / 180.0);

    }
    // Check presence of right click
    else if (event->buttons() & Qt::RightButton)
    {
        // Get clicked object
        qDebug() << "1";

        ObjectRect2* clicked_rect = qgraphicsitem_cast<ObjectRect2*>(this->itemAt(MouseX, MouseY));

        // Verify that clicked object is a widget and is not null
        if (clicked_rect != NULL)
        {
            // Convert object to ObjectRect
            this->selected_rect = clicked_rect;
        }

        if(this->selected_rect)
        {

            qDebug() << "Mode::MoveCreate";
            // Store base positions (Used to determine offset to move in panorama later)
            this->position.start_x = MouseX;
            this->position.start_y = MouseY;

            // Determine mouse click pointer offset
            this->position.offset_1 = QPointF(
                            mouse_scene.x() - this->selected_rect->getPoint1().x(),
                            mouse_scene.y() - this->selected_rect->getPoint1().y()
                        );

            // Determine mouse click pointer offset
            this->position.offset_2 = QPointF(
                            mouse_scene.x() - this->selected_rect->getPoint2().x(),
                            mouse_scene.y() - this->selected_rect->getPoint2().y()
                        );

            // Determine mouse click pointer offset
            this->position.offset_3 = QPointF(
                            mouse_scene.x() - this->selected_rect->getPoint3().x(),
                            mouse_scene.y() - this->selected_rect->getPoint3().y()
                        );

            // Determine mouse click pointer offset
            this->position.offset_4 = QPointF(
                            mouse_scene.x() - this->selected_rect->getPoint4().x(),
                            mouse_scene.y() - this->selected_rect->getPoint4().y()
                        );

            // Switch in creation mode
            this->mode = Mode::MoveCreate;

            // Enable mouse tracking
            this->setMouseTracking(true);

        } else {

            qDebug() << "Mode::Create";

            // Switch in creation mode
            this->mode = Mode::Create;

            // Enable mouse tracking
            this->setMouseTracking(true);

            // Store base positions (Used to determine size and position of object later)
            this->create_position.start_x = MouseX;
            this->create_position.start_y = MouseY;

        }
    }
}

// Mouse buttons release handler
void PanoramaViewer::mouseReleaseEvent(QMouseEvent *)
{
    // Reset mode
    this->mode = Mode::None;

    // Reset temporary object
    this->create_position.rect = NULL;
    this->selected_rect = NULL;

    // Disable mouse tracking
    this->setMouseTracking(false);
}

// Mouse displacement handler
void PanoramaViewer::mouseMoveEvent(QMouseEvent* event)
{

    // Store mouse coords
    int MouseX = event->x();
    int MouseY = event->y();

    // Store normalized mouse coords
    QPointF mouse_scene = this->mapToScene(event->pos());

    // Moving mouse section (move in panorama)
    if(this->mode == Mode::None)
    {

    }
    else if(this->mode == Mode::Move)
    {

        // Determine the displacement delta
        int delta_x = (MouseX - this->position.start_x);
        int delta_y = (MouseY - this->position.start_y);

        // Apply delta to azimuth and elevation
        float azimuth   = (this->position.start_azimuth   - ( (delta_x * this->position.aperture) * 0.1 ) );
        float elevation = (this->position.start_elevation + ( (delta_y * this->position.aperture) * 0.1 ) );

        this->position.old_azimuth = this->position.azimuth;
        this->position.old_elevation = this->position.elevation;

        // Clamp azimuth and elevation
        this->position.azimuth   = clampRad(azimuth, -360.0, 360.0) * (LG_PI / 180.0);
        this->position.elevation = clamp(elevation, -90.0, 90.0) * (LG_PI / 180.0);

        // Render scene
        this->render();

    }
    // Section creation section (create a new object)
    else if (this->mode == Mode::Create)
    {

        // Check if temporary object is created
        if(this->create_position.rect == NULL)
        {

            this->create_position.rect = new ObjectRect2();
            this->create_position.rect->setObjectRectType(ObjectRectType::Manual);
            this->create_position.rect->setObjectRectState(ObjectRectState::Valid);
            this->create_position.rect->setId( this->rect_list_v2.length() );

            this->rect_list_v2.append( this->create_position.rect );
            this->scene->addItem( this->create_position.rect );

            this->create_position.rect->setProjectionParametters(this->position.azimuth,
                                                                 this->position.elevation,
                                                                 this->position.aperture);

            // Move selection object to mouse coords
            this->create_position.rect->setPoints(
                            mouse_scene,
                            mouse_scene,
                            mouse_scene,
                            mouse_scene
                        );

            // Refresh main window labels
            emit refreshLabels();

        } else  {

            // Move selection object to mouse coords
            this->create_position.rect->setPoint3_Rigid(
                            mouse_scene
                        );

            QToolTip::showText(event->globalPos(),
                               QString::number( this->create_position.rect->getSize().width() ) + "x" +
                               QString::number( this->create_position.rect->getSize().height() ),
                               this, rect() );

        }
    } else if(this->mode == Mode::MoveCreate)
    {
        this->selected_rect->moveObject(mouse_scene,
                                        this->position.offset_1,
                                        this->position.offset_2,
                                        this->position.offset_3,
                                        this->position.offset_4);
    }
}

// Viewer resize handler
void PanoramaViewer::resizeEvent(QResizeEvent *)
{

    // Store actual dimensions
    int WIDTH = this->width();
    int HEIGHT = this->height();

    // Check if dimensions has changed
    if(WIDTH != this->previous_width && HEIGHT != this->previous_height)
    {

        // Store new dimensions
        this->previous_width = WIDTH;
        this->previous_height = HEIGHT;

        // Render scene
        this->render();
    }
}

// Function to crop a selection object
QImage PanoramaViewer::cropObject(ObjectRect* rect)
{

    // Convert points to a QRect and deduce borders sizes
    QRect rect_sel(
                    QPoint(rect->point_1.x() + 4, rect->point_1.y() + 4),
                    QPoint(rect->point_2.x() - 4, rect->point_2.y() - 4)
                );

    // Crop and return image
    return this->dest_image.copy(rect_sel);

}

void PanoramaViewer::insertDetectedObject(DetectedObject obj)
{
    // Create selection object
    ObjectRect* rect = new ObjectRect();

    rect->setObjectType(ObjectType::Face);
    rect->setRectType(RectType::Auto);
    rect->setValidState(ObjectValidState::None);
    rect->setAutomaticStatus("Ratio");
    rect->setBlurred(false);

    // Save projection parameters to it
    rect->projection_parameters.aperture  = obj.params.aperture;
    rect->projection_parameters.azimuth   = obj.params.azimuth;
    rect->projection_parameters.elevation = obj.params.elevation;
    rect->projection_parameters.scale_factor = 0.5;

    rect->setPos(obj.area.p1, obj.area.p2, RectMoveType::All);

    //rect->id = this->rect_list.length();

    // Add selection object to list
    //this->rect_list.append(rect);

    QGraphicsProxyWidget *proxyWidget = new QGraphicsProxyWidget();
    proxyWidget->setWidget(rect);

    // Add selection object to scene
    this->scene->addItem(proxyWidget);
}

void PanoramaViewer::TestSquare(int x, int y)
{
    QPainter painter(&this->dest_image_map);
    painter.setPen(Qt::blue);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(0, 0, 100, 100);
}
