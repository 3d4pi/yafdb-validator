/*! \brief Gnomonic to gnomonic coordinates converter
  *
  *  This function converts the coordinates of a point seen in a rectilinear
  *  image in the frame of a secondary rectilinear image. The gnomonic
  *  projection parameters are expected in the way lg_etg_apperture function
  *  of libgnomonic understand them
  *
  *  \param  r_width  Width, in pixels, of the reference rectilinear image
  *  \param  r_height Height, in pixels, of the reference rectilinear image
  *  \param  r_azim   Azimuth of reference gnomonic center
  *  \param  r_elev   Elevation of reference gnomonic center
  *  \param  r_appe   Apperture of the reference gnomonic projection
  *  \param  r_x      X position of the point in reference rectilinear image
  *  \param  r_y      Y position of the point in reference rectilinear image
  *  \param  c_width  Width, in pixels, of the secondary rectilinear image
  *  \param  c_height Height, in pixels, of the secondary rectilinear image
  *  \param  c_azim   Azimuth of secondary gnomonic center
  *  \param  c_elev   Elevation of secondary gnomonic center
  *  \param  c_appe   Apperture of the secondary gnomonic projection
  *  \param  c_x      X position of the point in secondary rectilinear image
  *  \param  c_y      Y position of the point in secondary rectilinear image
  *
  *  \return Returns one if first order visibility test of the computed point
  *  coordinates is triggered, zero otherwise
  */

#include "g2g_point.h"

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

) {

    /* Matrix array */
    static double m[3][3] = { { 0.0 } };

    /* Position arrays */
    static double pi[3] = { 0.0 };
    static double pf[3] = { 0.0 };

    /* Pixel sizes */
    static double r_pixel = 0.0;
    static double c_pixel = 0.0;

    /* Compute pixel sizes */
    r_pixel = 2.0 * tan( r_appe / 2.0 ) / r_width;
    c_pixel = 2.0 * tan( c_appe / 2.0 ) / c_width;

    /* Compute position in reference rectilinear frame */
    pi[0] = + ( 1.0 );
    pi[1] = + ( r_x - ( r_width  / 2.0 ) ) * r_pixel;
    pi[2] = + ( r_y - ( r_height / 2.0 ) ) * r_pixel;

    /* Create rotation matrix */
    lg_algebra_r2erotation( m, r_azim, r_elev, 0 );

    /* Apply rotation on position */
    pf[0] = m[0][0] * pi[0] + m[0][1] * pi[1] + m[0][2] * pi[2];
    pf[1] = m[1][0] * pi[0] + m[1][1] * pi[1] + m[1][2] * pi[2];
    pf[2] = m[2][0] * pi[0] + m[2][1] * pi[1] + m[2][2] * pi[2];

    /* Create rotation matrix */
    lg_algebra_e2rrotation( m, c_azim, c_elev, 0 );

    /* Apply rotation on position */
    pi[0] = m[0][0] * pf[0] + m[0][1] * pf[1] + m[0][2] * pf[2];
    pi[1] = m[1][0] * pf[0] + m[1][1] * pf[1] + m[1][2] * pf[2];
    pi[2] = m[2][0] * pf[0] + m[2][1] * pf[1] + m[2][2] * pf[2];

    /* Check if destination variables are specified */
    if( c_x != NULL && c_y != NULL )
    {
        /* Compute coordinates in secondary rectilinear frame */
        * c_x = + ( ( pi[1] / pi[0] ) / c_pixel ) + ( c_width  / 2.0 );
        * c_y = + ( ( pi[2] / pi[0] ) / c_pixel ) + ( c_height / 2.0 );
    }

    /* Compute frist order visibility condition */
    if ( pi[0] > 0 ) return( 1 );
    else return( 0 );
}
