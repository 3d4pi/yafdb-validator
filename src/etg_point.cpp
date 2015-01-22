/*
 * yafdb-validate - Yafdb validation tool
 *
 * Copyright (c) 2014-2015 FOXEL SA - http://foxel.ch
 * Please read <http://foxel.ch/license> for more information.
 *
 *
 * Author(s):
 *
 *      Nils Hamel <n.hamel@foxel.ch>
 *
 *
 * This file is part of the FOXEL project <http://foxel.ch>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Additional Terms:
 *
 *      You are required to preserve legal notices and author attributions in
 *      that material or in the Appropriate Legal Notices displayed by works
 *      containing it.
 *
 *      You are required to attribute the work as explained in the "Usage and
 *      Attribution" section of <http://foxel.ch/license>.
 */

#include "etg_point.h"

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

) {

    /* Matrix array */
    static double m[3][3] = { { 0.0 } };

    /* Position arrays */
    static double pi[3] = { 0.0 };
    static double pf[3] = { 0.0 };

    /* Pixel sizes */
    static double c_pixel = 0.0;

    /* Pixel spherical angles */
    static double s_x = 0.0;
    static double s_y = 0.0;

    /* Compute pixel sizes */
    c_pixel = 2.0 * tan( c_appe / 2.0 ) / c_width;

    /* Compute spherical angles */
    s_x = ( ( e_x / e_width  ) * LG_PI2 );
    s_y = ( ( e_y / e_height ) - 0.5 ) * LG_PI;

    /* Compute position in reference frame */
    pf[0] = + cos( s_y );
    pf[1] = + sin( s_x ) * pf[0];
    pf[0] = + cos( s_x ) * pf[0];
    pf[2] = + sin( s_y );

    /* Create rotation matrix */
    lg_algebra_e2rrotation( m, c_azim, c_elev, 0 );

    /* Apply rotation on position */
    pi[0] = m[0][0] * pf[0] + m[0][1] * pf[1] + m[0][2] * pf[2];
    pi[1] = m[1][0] * pf[0] + m[1][1] * pf[1] + m[1][2] * pf[2];
    pi[2] = m[2][0] * pf[0] + m[2][1] * pf[1] + m[2][2] * pf[2];

    /* Compute coordinates in secondary rectilinear frame */
    * c_x = + ( ( pi[1] / pi[0] ) / c_pixel ) + ( c_width  / 2.0 );
    * c_y = + ( ( pi[2] / pi[0] ) / c_pixel ) + ( c_height / 2.0 );

    /* Compute frist order visibility condition */
    if ( pi[0] > 0 ) return( 1 ); else return( 0 );

}
