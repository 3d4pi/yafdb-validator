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
#include <QDebug>

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

    /*qDebug() << "r_width" << r_width;
    qDebug() << "r_height" << r_height;
    qDebug() << "r_azim" << r_azim;
    qDebug() << "r_elev" << r_elev;
    qDebug() << "r_appe" << r_appe;
    qDebug() << "c_width" << c_width;
    qDebug() << "c_height" << c_height;
    qDebug() << "c_azim" << c_azim;
    qDebug() << "c_elev" << c_elev;
    qDebug() << "c_appe" << c_appe;*/

    /*if( r_width != c_width )
        qDebug() << "r_width(" << r_width << ") != c_width(" << c_width << ")";

    if( r_height != c_height )
        qDebug() << "r_height(" << r_height << ") != c_height(" << c_height << ")";

    if( r_azim != c_azim )
        qDebug() << "r_azim(" << r_azim << ") != c_azim(" << c_azim << ")";

    if( r_elev != c_elev )
        qDebug() << "r_elev(" << r_elev << ") != c_elev(" << c_elev << ")";

    if( r_appe != c_appe )
        qDebug() << "r_appe(" << r_appe << ") != c_appe(" << c_appe << ")";*/

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

    /* Compute coordinates in secondary rectilinear frame */
    * c_x = + ( ( pi[1] / pi[0] ) / c_pixel ) + ( c_width  / 2.0 );
    * c_y = + ( ( pi[2] / pi[0] ) / c_pixel ) + ( c_height / 2.0 );

    /*qDebug() << "c_x" << * c_y;
    qDebug() << "c_y" << * c_x;
    qDebug() << "";*/

    /* Compute frist order visibility condition */
    if ( pi[0] > 0 ) return( 1 ); else return( 0 );

}
