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

#ifndef G2G_POINT_H
#define G2G_POINT_H

#include <inter-all.h>
#include <gnomonic-all.h>

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
     *  \param  c_x      X position of the point in secondary rectilinear image (Optionnal)
     *  \param  c_y      Y position of the point in secondary rectilinear image (Optionnal)
     *
     *  \return Returns one if first order visibility test of the computed point
     *  coordinates is triggered, zero otherwise
     */

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
    double * const c_x = NULL,
    double * const c_y = NULL

);

#endif // G2G_POINT_H
