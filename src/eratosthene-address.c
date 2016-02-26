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
    source - accessor methods
 */

    le_size_t le_address_get_size( le_address_t const * const le_address ) {

        /* Return address size */
        return( le_address->as_size );

    }

    le_byte_t le_address_get_digit( le_address_t const * const le_address, le_size_t const le_offset ) {

        /* Return address digit */
        return( le_address->as_addr[le_offset] );

    }

/*
    source - mutator methods
 */

    le_void_t le_address_set_size( le_address_t * const le_address, le_size_t const le_size ) {

        /* Assign address size */
        le_address->as_size = le_size;

    }

    le_void_t le_address_set_digit( le_address_t * const le_address, le_size_t const le_offset, le_byte_t const le_digit ) {

        /* Assign address digit */
        le_address->as_addr[le_offset] = le_digit;

    }

/*
    source - address methods
 */

    le_void_t le_address( le_address_t * const le_address, le_real_t * const le_pose ) {

        /* Digital buffer variables */
        le_byte_t le_buffer = 0;

        /* Parsing variables */
        le_size_t le_parse = 0;

        /* Coordinates normalisation on [0,1[ range */
        le_pose[0] = ( le_pose[0] - LE_GEODESY_LMIN ) / ( LE_GEODESY_LMAX - LE_GEODESY_LMIN );
        le_pose[1] = ( le_pose[1] - LE_GEODESY_AMIN ) / ( LE_GEODESY_AMAX - LE_GEODESY_AMIN );
        le_pose[2] = ( le_pose[2] - LE_GEODESY_HMIN ) / ( LE_GEODESY_HMAX - LE_GEODESY_HMIN );

        /* Composing address */
        for ( ; le_parse < le_address->as_size; le_parse ++ ) {

            /* Normalised longitude processing */
            if ( le_pose[0] >= 0.5 ) le_buffer = 1; else le_buffer = 0;

            /* Assign address digit */
            le_address->as_addr[le_parse] = le_buffer;

            /* Update normalised coordinate */
            le_pose[0] = ( le_pose[0] * 2.0 ) - le_buffer;

            /* Asynchronous dimension management */
            if ( le_parse < 2 ) continue;

            /* Normalised latitude processing */
            if ( le_pose[1] >= 0.5 ) le_buffer = 1; else le_buffer = 0;

            /* Assign address digit */
            le_address->as_addr[le_parse] |= le_buffer << 0x01;

            /* Update normalised coordinate */
            le_pose[1] = ( le_pose[1] * 2.0 ) - le_buffer;

            /* Asynchronous dimension management */
            if ( le_parse < 10 ) continue;

            /* Normalised altitude processing */
            if ( le_pose[2] >= 0.5 ) le_buffer = 1; else le_buffer = 0;

            /* Assign address digit */
            le_address->as_addr[le_parse] |= le_buffer << 0x02;

            /* Update normalised coordinate */
            le_pose[2] = ( le_pose[2] * 2.0 ) - le_buffer;

        }

    }

/*
    source - conversion methods
 */

    le_void_t le_address_binary_string( le_address_t const * const le_address, le_char_t * const le_string ) {

        /* Parsing variables */
        le_size_t le_parse = 0;

        /* Conversion loop */
        for ( ; le_parse < le_address->as_size; le_parse ++ ) le_string[le_parse] = le_address->as_addr[le_parse] + 48;

    }

    le_void_t le_address_string_binary( le_address_t * const le_address, le_char_t const * const le_string ) {

        /* Parsing variables */
        le_size_t le_parse = 0;

        /* Assign address length */
        le_address->as_size = strlen( ( char * ) le_string );
 
        /* Conversion loop */
        for ( ; le_parse < le_address->as_size; le_parse ++ ) le_address->as_addr[le_parse] = le_string[le_parse] - 48;

    }

