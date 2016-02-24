/*
 *  liberatosthene - geodetic system
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016 EPFL CDH DHLAB
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

    # include "eratosthene-address.h"

/*
    source - translation methods
 */

    le_void_t le_address_address( le_real_t * const le_pose, le_byte_t * const le_address, le_size_t const le_depth ) {

        /* Digital buffer variables */
        le_byte_t le_buffer = 0;

        /* Parsing variables */
        le_size_t le_parse = 0;

        /* Coordinates normalisation on [0,1[ range */
        le_pose[0] = ( le_pose[0] + LE_GEODESY_LMIN ) / ( LE_GEODESY_LMAX - LE_GEODESY_LMIN );
        le_pose[1] = ( le_pose[1] + LE_GEODESY_AMIN ) / ( LE_GEODESY_AMAX - LE_GEODESY_AMIN );
        le_pose[2] = ( le_pose[2] + LE_GEODESY_HMIN ) / ( LE_GEODESY_HMAX - LE_GEODESY_HMIN );

        /* Assign implicit address first digit */
        le_address[0] = 0;

        /* Composing address */
        for ( le_parse = 1 ; le_parse < le_depth; le_parse ++ ) {

            /* Normalised longitude processing */
            if ( le_pose[0] >= 0.5 ) le_buffer = 1; else le_buffer = 0;

            /* Assign address digit */
            le_address[le_parse] = le_buffer;

            /* Update normalised coordinate */
            le_pose[0] = ( le_pose[0] * 2.0 ) - le_buffer;

            /* Asynchronous dimension management */
            if ( le_parse < 2 ) continue;

            /* Normalised latitude processing */
            if ( le_pose[1] >= 0.5 ) le_buffer = 1; else le_buffer = 0;

            /* Assign address digit */
            le_address[le_parse] |= le_buffer << 0x01;

            /* Update normalised coordinate */
            le_pose[1] = ( le_pose[1] * 2.0 ) - le_buffer;

            /* Asynchronous dimension management */
            if ( le_parse < 12 ) continue;

            /* Normalised altitude processing */
            if ( le_pose[2] >= 0.5 ) le_buffer = 1; else le_buffer = 0;

            /* Assign address digit */
            le_address[le_parse] |= le_buffer << 0x02;

        }

    }

