#ifndef G2G_POINT_H
#define G2G_POINT_H

#include <inter-all.h>
#include <gnomonic-all.h>

int g2g_point(

    double   const r_width,
    double   const r_height,
    double   const r_azim,
    double   const r_elev,
    double   const r_appe,
    double   const r_x,
    double   const r_y,
    double   const c_width,
    double   const c_height,
    double   const c_azim,
    double   const c_elev,
    double   const c_appe,
    double * const c_x,
    double * const c_y

);

#endif // G2G_POINT_H
