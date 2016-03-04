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
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

    # include "eratosthene-address.h"

/*
    source - accessor methods
 */

    le_time_t le_address_get_time( le_address_t const * const le_address ) {

        /* Return address time */
        return( le_address->as_time );

    }

    le_size_t le_address_get_size( le_address_t const * const le_address ) {

        /* Return address size */
        return( le_address->as_size );

    }

    le_byte_t le_address_get_digit( le_address_t const * const le_address, le_size_t const le_offset ) {

        /* Check consistency */
        if ( le_offset >= le_address->as_size ) {

            /* Return null digit */
            return( _LE_BYTE_NULL );

        } else {

            /* Return address digit */
            return( le_address->as_addr[le_offset] );

        }

    }

    le_byte_t * le_address_get_digits( le_address_t const * const le_address ) {

        /* Return address digits pointer */
        return( ( le_byte_t * ) le_address->as_addr );

    }

    le_size_t le_address_get_depth( le_address_t const * const le_address ) {

        /* Return address depth */
        return( le_address->as_dept );

    }

    le_void_t le_address_get_pose( le_address_t const * const le_address, le_real_t * const le_pose ) {

        /* Parsing variables */
        le_size_t le_parse = 0;

        /* Scales variables */
        le_real_t le_scale[3] = { 1.0, 1.0, 1.0 };

        /* Initialise spatial coordinates */
        le_pose[0] = 0.0;
        le_pose[1] = 0.0;
        le_pose[2] = 0.0;

        /* Decomposing address */
        for ( ; le_parse < le_address->as_size; le_parse ++ ) {

            /* Analyse address digit and coordinates update */
            le_pose[0] += ( ( le_real_t ) ( le_address->as_addr[le_parse] & 0x01 ) ) * ( le_scale[0] /= 2.0 );

            /* Asynchronous dimension management */
            if ( le_parse < 2 ) continue;

            /* Analyse address digit and coordinates update */
            le_pose[1] += ( ( le_real_t ) ( ( le_address->as_addr[le_parse] & 0x02 ) >> 1 ) ) * ( le_scale[1] /= 2.0 );

            /* Asynchronous dimension management */
            if ( le_parse < 10 ) continue;

            /* Analyse address digit and coordinates update */
            le_pose[2] += ( ( le_real_t ) ( ( le_address->as_addr[le_parse] & 0x04 ) >> 2 ) ) * ( le_scale[2] /= 2.0 );

        }

        /* Coordinates denormalisation */
        le_pose[0] = LE_GEODESY_LMIN + le_pose[0] * ( LE_GEODESY_LMAX - LE_GEODESY_LMIN );
        le_pose[1] = LE_GEODESY_AMIN + le_pose[1] * ( LE_GEODESY_AMAX - LE_GEODESY_AMIN );
        le_pose[2] = LE_GEODESY_HMIN + le_pose[2] * ( LE_GEODESY_HMAX - LE_GEODESY_HMIN );

    }

/*
    source - mutator methods
 */

    le_void_t le_address_set_time( le_address_t * const le_address, le_time_t const le_time ) {

        /* Assign address time */
        le_address->as_time = le_time;

    }

    le_enum_t le_address_set_size( le_address_t * const le_address, le_size_t const le_size ) {

        /* Check consistency */
        if ( le_size >= _LE_USE_DEPTH ) {

            /* Send message */
            return( LE_ERROR_DEPTH );

        } else {

            /* Assign address size */
            le_address->as_size = le_size;

            /* Send message */
            return( LE_ERROR_SUCCESS );

        }

    }

    le_enum_t le_address_set_digit( le_address_t * const le_address, le_size_t const le_offset, le_byte_t const le_digit ) {

        /* Check consistency */
        if ( le_offset >= le_address->as_size ) {

            /* Send message */
            return( LE_ERROR_DEPTH );

        }

        /* Check consistency */
        if ( le_digit >= _LE_USE_BASE ) {

            /* Send message */
            return( LE_ERROR_BASE );

        }

        /* Assign address digit */
        le_address->as_addr[le_offset] = le_digit;

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

    le_void_t le_address_set_depth( le_address_t * const le_address, le_size_t const le_depth ) {

        /* Assign address depth */
        le_address->as_dept = le_depth;

    }

    le_void_t le_address_set_pose( le_address_t * const le_address, le_real_t * const le_pose ) {

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

    le_void_t le_address_cvas( le_address_t const * const le_address, le_char_t * const le_string ) {

        /* Conversion buffer variables */
        le_char_t le_buffer[LE_NETWORK_BUFFER_ADDR] = LE_NETWORK_BUFFER_C;

        /* Parsing variables */
        le_size_t le_parse = 0;

        /* Convert geodetic address */
        for ( ; le_parse < le_address->as_size; le_parse ++ ) le_buffer[le_parse] = le_address->as_addr[le_parse] + _LE_USE_ASCII_ITOA;

        /* Composing address string */
        sprintf( ( char * ) le_string, "/%" _LE_TIME_P "/%s/%" _LE_SIZE_P, le_address->as_time, le_buffer, le_address->as_dept );

    }

    le_void_t le_address_cvsa( le_address_t * const le_address, le_char_t const * const le_string ) {

        /* Conversion buffer variables */
        le_char_t le_buffer[LE_NETWORK_BUFFER_ADDR] = LE_NETWORK_BUFFER_C;

        /* Parsing variables */
        le_size_t le_parse = 0;

        /* Decompose address string */
        sscanf( ( char * ) le_string, "/%" _LE_TIME_S "/%[^/]/%" _LE_SIZE_S, & le_address->as_time, le_buffer, & le_address->as_dept );

        /* Compute geodetic address size */
        le_address->as_size = strlen( ( char * ) le_buffer );

        /* Convert geodetic address */
        for ( ; le_parse < le_address->as_size; le_parse ++ ) le_address->as_addr[le_parse] = le_buffer[le_parse] - _LE_USE_ASCII_ITOA;

    }

