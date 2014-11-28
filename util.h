#ifndef UTIL_H
#define UTIL_H

#include <QPoint>
#include <QPointF>

struct normalization_struct {
    int pano_width;
    int pano_height;
    float scale_factor;
};

class util
{
public:
    util();

    static QPointF normalize(QPointF value, normalization_struct norm_params);
    static float normalize(float value, normalization_struct norm_params);

    static QPointF denormalize(QPointF value, normalization_struct norm_params, int descale = 0);
    static float denormalize(float value, normalization_struct norm_params, int descale = 0);

};

#endif // UTIL_H
