/* Includes */
#include "utils.h"

/* Function to convert an OpenCV IplImage into a QImage */
QImage* IplImage2QImage(IplImage *iplImg)
{
    /* Save image dismensions */
    int h = iplImg->height;
    int w = iplImg->width;

    /* Get image channels */
    int channels = iplImg->nChannels;

    /* Allocate destination image */
    QImage *qimg = new QImage(w, h, QImage::Format_ARGB32);

    /* Get source image data */
    char *data = iplImg->imageData;

    /* Iterate over image Y axis */
    for (int y = 0; y < h; y++, data += iplImg->widthStep)
    {
        /* Iterate over image X axis */
        for (int x = 0; x < w; x++)
        {
            /* Initialize pixel color containers */
            char r = 0;
            char g = 0;
            char b = 0;
            char a = 0;

            /* Channels check */
            if (channels == 1)
            {
                /* Set red channel */
                r = data[x * channels];

                /* Set green channel */
                g = data[x * channels];

                /* Set blue channel */
                b = data[x * channels];
            }
            else if (channels == 3 || channels == 4)
            {
                /* Set red channel */
                r = data[x * channels + 2];

                /* Set green channel */
                g = data[x * channels + 1];

                /* Set blue channel */
                b = data[x * channels];
            }

            /* Channels check */
            if (channels == 4)
            {
                /* Set alpha channel */
                a = data[x * channels + 3];

                /* Set pixel */
                qimg->setPixel(x, y, qRgba(r, g, b, a));
            }
            else
            {
                /* Set pixel */
                qimg->setPixel(x, y, qRgb(r, g, b));
            }
        }
    }
    return qimg;
}

/* Function to export an object to disk */
void exportRect(ObjectRect *rect, image_info_struct image_info, QString destination, float zoom_level)
{
    /* Copy object */
    ObjectRect* rect_mapped = rect->copy();

    /* Map copied object to source object projection parameters */
    rect_mapped->mapTo(rect->proj_width(),
                       rect->proj_height(),
                       rect->proj_azimuth(),
                       rect->proj_elevation(),
                       rect->proj_aperture() / zoom_level);

    /* Update object projection points */
    rect_mapped->setProjectionPoints();

    /* Get selection from object's points */
    QRect rect_sel(
        QPoint(rect_mapped->proj_point_1().x() + rect_mapped->getBorderWidth(), rect_mapped->proj_point_1().y() + rect_mapped->getBorderWidth()),
        QPoint(rect_mapped->proj_point_3().x() - rect_mapped->getBorderWidth(), rect_mapped->proj_point_3().y() - rect_mapped->getBorderWidth())
    );

    /* Delete temp rect */
    delete rect_mapped;

    /* Create temporary destination image */
    QImage temp_dest(rect->proj_width(), rect->proj_height(), QImage::Format_RGB32);

    /* Determine best number of threads */
    int threads_count = QThread::idealThreadCount();

    /* Check if rect sizes are correct */
    if( rect->getSize().width() >= 1 &&
           rect->getSize().height() >= 1 )
    {

        /* Project gnomonic image */
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

        /* Crop and save image */
        QImage element = temp_dest.copy(rect_sel);
        element.save( destination );
    }
}

/* Function to clamp a specified value */
float clamp(float x, float a, float b)
{
    /* Return result */
    return x < a ? a : (x > b ? b : x);
}

/* Function to clamp a specified radian value */
float clampRad(float x, float a, float b)
{
    /* Check if value is greather than maximum */
    if(x > b)
    {
        /* Determine delta between value and maximum */
        float delta = (x - b);

        /* Return maximum + delta */
        return (a + delta);
    }

    /* Check if value is lesser than minimum */
    else if (x < a) {

        /* Determine delta between value and minimum */
        float delta = (a - x);

        /* Return minimum + delta */
        return (b - delta);
    } else {

        /* Return value */
        return x;
    }
}
