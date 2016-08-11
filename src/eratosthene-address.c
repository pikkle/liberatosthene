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

    # include "eratosthene-address.h"

/*
    source - accessor methods
 */

    inline le_time_t le_address_get_time( le_address_t const * const le_address ) {

        /* Return address time */
        return( le_address->as_time );

    }

    inline le_size_t le_address_get_size( le_address_t const * const le_address ) {

        /* Return address size */
        return( le_address->as_size );

    }

    inline le_byte_t le_address_get_digit( le_address_t const * const le_address, le_size_t const le_offset ) {

        /* Check consistency - return address digit */
        return( le_address->as_digit[le_offset] );

    }

    inline le_size_t le_address_get_depth( le_address_t const * const le_address ) {

        /* Return address depth */
        return( le_address->as_depth );

    }

    le_enum_t le_address_get_equal( le_address_t const * const le_addr1, le_address_t const * const le_addr2 ) {

        /* Fields comparison - send message */
        if ( le_addr1->as_size  != le_addr2->as_size  ) return( _LE_FALSE );
        if ( le_addr1->as_time  != le_addr2->as_time  ) return( _LE_FALSE );
        if ( le_addr2->as_depth != le_addr2->as_depth ) return( _LE_FALSE );

        /* Digits comparison */
        for ( le_size_t le_parse = 0; le_parse < le_addr1->as_size; le_parse ++ ) {

            /* Compare digits */
            if ( le_addr1->as_digit[le_parse] != le_addr2->as_digit[le_parse] ) {

                /* Send message */
                return( _LE_FALSE );

            }

        }

        /* Send message */
        return( _LE_TRUE );

    }

    le_void_t le_address_get_pose( le_address_t const * const le_address, le_real_t * const le_pose ) {
       
        /* Scales variables */
        le_real_t le_scale[3] = { 1.0, 1.0, 1.0 };

        /* Initialise spatial coordinates */
        le_pose[0] = 0.0;
        le_pose[1] = 0.0;
        le_pose[2] = 0.0;

        /* Decomposing address */
        for ( le_size_t le_parse = 0; le_parse < le_address->as_size; le_parse ++ ) {

            /* Analyse address digit and coordinates update */
            le_pose[0] += ( ( le_real_t ) ( le_address->as_digit[le_parse] & 0x01 ) ) * ( le_scale[0] /= 2.0 );

            /* Asynchronous dimension management */
            if ( le_parse < LE_GEODESY_ASYP ) continue;

            /* Analyse address digit and coordinates update */
            le_pose[1] += ( ( le_real_t ) ( ( le_address->as_digit[le_parse] & 0x02 ) >> 1 ) ) * ( le_scale[1] /= 2.0 );

            /* Asynchronous dimension management */
            if ( le_parse < LE_GEODESY_ASYA ) continue;

            /* Analyse address digit and coordinates update */
            le_pose[2] += ( ( le_real_t ) ( ( le_address->as_digit[le_parse] & 0x04 ) >> 2 ) ) * ( le_scale[2] /= 2.0 );

        }

        /* Coordinates denormalisation */
        le_pose[0] = LE_GEODESY_LMIN + le_pose[0] * LE_GEODESY_LRAN;
        le_pose[1] = LE_GEODESY_AMIN + le_pose[1] * LE_GEODESY_ARAN;
        le_pose[2] = LE_GEODESY_HMIN + le_pose[2] * LE_GEODESY_HRAN;

    }

/*
    source - mutator methods
 */

    inline le_void_t le_address_set_time( le_address_t * const le_address, le_time_t const le_time ) {

        /* Assign address time */
        le_address->as_time = le_time;

    }

    inline le_void_t le_address_set_size( le_address_t * const le_address, le_size_t const le_size ) {

        /* Assign address size */
        le_address->as_size = le_size;

    }

    inline le_void_t le_address_set_digit( le_address_t * const le_address, le_size_t const le_offset, le_byte_t const le_digit ) {

        /* Assign address digit */
        le_address->as_digit[le_offset] = le_digit;

    }

    inline le_void_t le_address_set_depth( le_address_t * const le_address, le_size_t const le_depth ) {

        /* Assign address depth */
        le_address->as_depth = le_depth;

    }

    le_void_t le_address_set_pose( le_address_t * const le_address, le_real_t * const le_pose ) {

        /* Digital buffer variables */
        le_byte_t le_buffer = 0;

        /* Coordinates normalisation on [0,1[ range */
        le_pose[0] = ( le_pose[0] - LE_GEODESY_LMIN ) / LE_GEODESY_LRAN;
        le_pose[1] = ( le_pose[1] - LE_GEODESY_AMIN ) / LE_GEODESY_ARAN;
        le_pose[2] = ( le_pose[2] - LE_GEODESY_HMIN ) / LE_GEODESY_HRAN;

        /* Composing address */
        for ( le_size_t le_parse = 0 ; le_parse < le_address->as_size; le_parse ++ ) {

            /* Assign address digit */
            le_address->as_digit[le_parse] = ( le_buffer = ( le_pose[0] >= 0.5 ) ? 1 : 0 );

            /* Update normalised coordinate */
            le_pose[0] = ( le_pose[0] * 2.0 ) - le_buffer;

            /* Asynchronous dimension management */
            if ( le_parse < LE_GEODESY_ASYP ) continue;

            /* Assign address digit */
            le_address->as_digit[le_parse] |= ( le_buffer = ( le_pose[1] >= 0.5 ) ? 1 : 0 ) << 0x01;

            /* Update normalised coordinate */
            le_pose[1] = ( le_pose[1] * 2.0 ) - le_buffer;

            /* Asynchronous dimension management */
            if ( le_parse < LE_GEODESY_ASYA ) continue;

            /* Assign address digit */
            le_address->as_digit[le_parse] |= ( le_buffer = ( le_pose[2] >= 0.5 ) ? 1 : 0 ) << 0x02;

            /* Update normalised coordinate */
            le_pose[2] = ( le_pose[2] * 2.0 ) - le_buffer;

        }

    }

/*
    source - i/o methods
 */

    le_enum_t le_address_io_read( le_address_t * const le_address, le_sock_t const le_socket ) {

        /* Socket i/o buffer variables */
        le_byte_t le_data[LE_NETWORK_SB_ADDR] = LE_NETWORK_C;

        /* Buffer mapping variables */
        le_size_t * le_smap = ( le_size_t * ) ( le_data     );
        le_time_t * le_tmap = ( le_time_t * ) ( le_smap + 1 );
        le_size_t * le_dmap = ( le_size_t * ) ( le_tmap + 1 );
        le_byte_t * le_gmap = ( le_byte_t * ) ( le_dmap + 1 );

        /* Read buffer from socket - send message */
        if ( read( le_socket, le_data, LE_NETWORK_SB_ADDR ) != LE_NETWORK_SB_ADDR ) return( LE_ERROR_IO_READ );

        /* Read socket buffer */
        memcpy( le_address->as_digit, le_gmap, _LE_USE_DEPTH );

        /* Read socket buffer */
        le_address->as_size  = le_smap[0];
        le_address->as_time  = le_tmap[0];
        le_address->as_depth = le_dmap[0];

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

    le_enum_t le_address_io_write( le_address_t const * const le_address, le_sock_t const le_socket ) {

        /* Socket i/o buffer variables */
        le_byte_t le_data[LE_NETWORK_SB_ADDR] = LE_NETWORK_C;

        /* Buffer mapping variables */
        le_size_t * le_smap = ( le_size_t * ) ( le_data     );
        le_time_t * le_tmap = ( le_time_t * ) ( le_smap + 1 );
        le_size_t * le_dmap = ( le_size_t * ) ( le_tmap + 1 );
        le_byte_t * le_gmap = ( le_byte_t * ) ( le_dmap + 1 );

        /* Write socket buffer */
        le_smap[0] = le_address->as_size;
        le_tmap[0] = le_address->as_time;
        le_dmap[0] = le_address->as_depth;

        /* Write socket buffer */
        memcpy( le_gmap, le_address->as_digit, _LE_USE_DEPTH );

        /* Write buffer on socket - send message */
        if ( write( le_socket, le_data, LE_NETWORK_SB_ADDR ) != LE_NETWORK_SB_ADDR ) return( LE_ERROR_IO_WRITE );

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

/*
    source - conversion methods
 */

    le_void_t le_address_ct_string( le_address_t const * const le_address, le_char_t * const le_string ) {

        /* Conversion buffer variables */
        le_char_t le_data[LE_NETWORK_SB_ADDR] = LE_NETWORK_C;

        /* Convert geodetic address */
        for ( le_size_t le_parse = 0 ; le_parse < le_address->as_size; le_parse ++ ) {

            /* Digit/character conversion */
            le_data[le_parse] = le_address->as_digit[le_parse] + _LE_USE_ASCII_ITOA;

        }

        /* Composing address string */
        sprintf( ( char * ) le_string, "/%" _LE_TIME_P "/%s/%" _LE_SIZE_P, le_address->as_time, le_data, le_address->as_depth );

    }

    le_void_t le_address_cf_string( le_address_t * const le_address, le_char_t const * const le_string ) {

        /* Conversion buffer variables */
        le_char_t le_data[LE_NETWORK_SB_ADDR] = LE_NETWORK_C;

        /* Decompose address string */
        sscanf( ( char * ) le_string, "/%" _LE_TIME_S "/%[^/]/%" _LE_SIZE_S, & le_address->as_time, le_data, & le_address->as_depth );

        /* Compute geodetic address size */
        le_address->as_size = strlen( ( char * ) le_data );

        /* Convert geodetic address */
        for ( le_size_t le_parse = 0 ; le_parse < le_address->as_size; le_parse ++ ) {

            /* Digit/character conversion */
            le_address->as_digit[le_parse] = le_data[le_parse] - _LE_USE_ASCII_ITOA;

        }

    }

