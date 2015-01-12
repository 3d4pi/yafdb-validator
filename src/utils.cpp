#include "utils.h"

QImage* IplImage2QImage(IplImage *iplImg)
{
    int h = iplImg->height;
    int w = iplImg->width;
    int channels = iplImg->nChannels;
    QImage *qimg = new QImage(w, h, QImage::Format_ARGB32);
    char *data = iplImg->imageData;

    for (int y = 0; y < h; y++, data += iplImg->widthStep)
    {
        for (int x = 0; x < w; x++)
        {
            char r = 0;
            char g = 0;
            char b = 0;
            char a = 0;

            if (channels == 1)
            {
                r = data[x * channels];
                g = data[x * channels];
                b = data[x * channels];
            }
            else if (channels == 3 || channels == 4)
            {
                r = data[x * channels + 2];
                g = data[x * channels + 1];
                b = data[x * channels];
            }

            if (channels == 4)
            {
                a = data[x * channels + 3];
                qimg->setPixel(x, y, qRgba(r, g, b, a));
            }
            else
            {
                qimg->setPixel(x, y, qRgb(r, g, b));
            }
        }
    }
    return qimg;
}

void exportRect(ObjectRect *rect, image_info_struct image_info, QString destination, float zoom_level)
{
    // Convert points to a QRect and deduce borders sizes
    ObjectRect* rect_mapped = rect->copy();

    rect_mapped->mapTo(rect->proj_width(),
                       rect->proj_height(),
                       rect->proj_azimuth(),
                       rect->proj_elevation(),
                       rect->proj_aperture() / zoom_level);
    rect_mapped->setProjectionPoints();

    QRect rect_sel(
        QPoint(rect_mapped->proj_point_1().x() + rect_mapped->getBorderWidth(), rect_mapped->proj_point_1().y() + rect_mapped->getBorderWidth()),
        QPoint(rect_mapped->proj_point_3().x() - rect_mapped->getBorderWidth(), rect_mapped->proj_point_3().y() - rect_mapped->getBorderWidth())
    );

    delete rect_mapped;

    QImage temp_dest(rect->proj_width(), rect->proj_height(), QImage::Format_RGB32);
    int threads_count = QThread::idealThreadCount();

    if( rect->getSize().width() >= 1 &&
           rect->getSize().height() >= 1 )
    {
        lg_etg_apperturep(

            ( inter_C8_t * ) image_info.image->bits(),
            image_info.width,
            image_info.height,
            image_info.channels,
            ( inter_C8_t * ) temp_dest.bits(),
            rect->proj_width(),
            rect->proj_height(),
            image_info.channels,
            rect->proj_azimuth(),
            rect->proj_elevation(),
            0.0,
            rect->proj_aperture() / zoom_level,
            li_bilinearf,
            threads_count
        );

        // Crop and return image
        QImage element = temp_dest.copy(rect_sel);
        element.save( destination );
    }
}
