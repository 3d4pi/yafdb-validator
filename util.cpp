#include "util.h"

QPointF util::normalize(QPointF value, normalization_struct norm_params)
{
    return QPointF(
                    (((value.x()) / norm_params.pano_width)  * norm_params.scale_factor),
                    (((value.y()) / norm_params.pano_height) * norm_params.scale_factor)
                );
}

float util::normalize(float value, normalization_struct norm_params)
{
    return (((value) / norm_params.pano_width)  * norm_params.scale_factor);
}

QPointF util::denormalize(QPointF value, normalization_struct norm_params, int descale)
{
    float scale_factor = (descale != 0 ? norm_params.scale_factor : 1.0);

    return QPointF(
                    ((value.x() * norm_params.pano_width) / scale_factor),
                    ((value.y() * norm_params.pano_height) / scale_factor)
                );
}

float util::denormalize(float value, normalization_struct norm_params, int descale)
{
    float scale_factor = (descale != 0 ? norm_params.scale_factor : 1.0);

    return ((value * norm_params.pano_width) / scale_factor);
}

