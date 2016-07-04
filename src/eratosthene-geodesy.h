/*
 *  liberatosthene - eratosthene indexation server
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016 EPFL CDH DHLAB
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

    /*! \file   eratosthene-geodesy.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Geodetic module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_GEODESY__
    # define __LE_GEODESY__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - includes
 */

    # include "eratosthene.h"

/*
    header - preprocessor definitions
 */

    /* Define WGS84 ellipsoid parameters */
    # define LE_GEODESY_WGS84_A ( _LE_REAL_L( 6378137.0 ) )
    # define LE_GEODESY_WGS84_F ( _LE_REAL_L( 298.257223563L ) )

    /* Asynchronous dimension edges */
    # define LE_GEODESY_ASYP    (  1 )
    # define LE_GEODESY_ASYA    ( 10 )

    /* Define ellispoidal coordinates boundaries */
    # define LE_GEODESY_LMIN    ( - LE_PI )
    # define LE_GEODESY_LMAX    ( + LE_PI )
    # define LE_GEODESY_AMIN    ( - LE_P2 )
    # define LE_GEODESY_AMAX    ( + LE_P2 )
    # define LE_GEODESY_HMIN    ( - ( LE_PI * LE_GEODESY_WGS84_A ) / _LE_REAL_L( 1024.0 ) )
    # define LE_GEODESY_HMAX    ( + ( LE_PI * LE_GEODESY_WGS84_A ) / _LE_REAL_L( 1024.0 ) )

    /* Define ellipsoidal coordinates ranges */
    # define LE_GEODESY_LRAN    ( LE_GEODESY_LMAX - LE_GEODESY_LMIN )
    # define LE_GEODESY_ARAN    ( LE_GEODESY_AMAX - LE_GEODESY_AMIN )
    # define LE_GEODESY_HRAN    ( LE_GEODESY_HMAX - LE_GEODESY_HMIN )

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

/*
    header - function prototypes
 */

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    }
    # endif

/*
    header - inclusion guard
 */

    # endif

