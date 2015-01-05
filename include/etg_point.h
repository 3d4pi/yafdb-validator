#ifndef ETG_POINT_H
#define ETG_POINT_H

#include <inter-all.h>
#include <gnomonic-all.h>

/*! \brief Equirectangular to gnomonic coordinates converter
 *
 *  This function converts the coordinates of a point seen on the reference
 *  equirectangular image in the rectilinear image coordinates system obtained
 *  using gnomonic projection parameters.
 *
 *  \param  e_width  Width, in pixels, of the equirectangular image
 *  \param  e_height Height, in pixels, of the equirectangular image
 *  \param  e_x      X position, in pixels, of the point in equirectangular mapping
 *  \param  e_y      Y position, in pixels, of the point in equirectangular mapping
 *  \param  c_width  Width, in pixels, of the rectilinear image
 *  \param  c_height Height, in pixels, of the rectilinear image
 *  \param  c_azim   Azimuth of gnomonic center
 *  \param  c_elev   Elevation of gnomonic center
 *  \param  c_appe   Apperture of the gnomonic projection
 *  \param  c_x      X position of the point in rectilinear image
 *  \param  c_y      Y position of the point in rectilinear image
 *
 *  \return Returns one if first order visibility test of the computed point
 *  coordinates is triggered, zero otherwise
 */

int etg_point(

    double   const e_width,
    double   const e_height,
    double   const e_x,
    double   const e_y,
    double   const c_width,
    double   const c_height,
    double   const c_azim,
    double   const c_elev,
    double   const c_appe,
    double * const c_x,
    double * const c_y

);

#endif // ETG_POINT_H
