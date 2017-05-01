/*
 *  liberatosthene
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2017 EPFL CDH DHLAB
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

    le_byte_t le_address_get_size( le_address_t const * const le_address ) {

        /* return address size */
        return( le_address->as_size );

    }

    le_byte_t le_address_get_mode( le_address_t const * const le_address ) {

        /* return address mode */
        return( le_address->as_mode );

    }

    le_time_t le_address_get_time( le_address_t const * const le_address, le_size_t const le_offset ) {

        /* return address time */
        return( le_address->as_times[le_offset] );

    }

    le_byte_t le_address_get_digit( le_address_t const * const le_address, le_size_t const le_offset ) {

        /* return address digit */
        return( le_address->as_digit[le_offset] );

    }

    le_byte_t le_address_get_span( le_address_t const * const le_address ) {

        /* return address depth */
        return( le_address->as_span );

    }

    le_enum_t le_address_get_equal( le_address_t const * const le_addr1, le_address_t const * const le_addr2 ) {

        /* digits array pointer variables */
        le_byte_t * er_ptr1 = ( le_byte_t * ) ( le_addr1->as_digit + le_addr1->as_size );
        le_byte_t * er_ptr2 = ( le_byte_t * ) ( le_addr2->as_digit + le_addr2->as_size );

        /* fields comparison - send message */
        if ( le_addr1->as_size != le_addr2->as_size ) return( _LE_FALSE );
        if ( le_addr1->as_mode != le_addr2->as_mode ) return( _LE_FALSE );
        if ( le_addr1->as_span != le_addr2->as_span ) return( _LE_FALSE );

        /* times comparison - send message */
        if ( le_addr1->as_times[0] != le_addr2->as_times[0] ) return( _LE_FALSE );
        if ( le_addr1->as_times[1] != le_addr2->as_times[1] ) return( _LE_FALSE );

        /* digits comparison - send message */
        while ( ( -- er_ptr2, -- er_ptr1 ) >= le_addr1->as_digit ) if ( ( * er_ptr1 ) != ( * er_ptr2 ) ) return( _LE_FALSE );

        /* send message */
        return( _LE_TRUE );

    }

    le_void_t le_address_get_pose_( le_address_t const * const le_address, le_size_t le_size, le_real_t * const le_pose ) {

        /* scales variables */
        le_real_t le_scale[3] = { 1.0, 1.0, 1.0 };

        /* initialise spatial coordinates */
        le_pose[0] = 0.0;
        le_pose[1] = 0.0;
        le_pose[2] = 0.0;

        /* decomposing address */
        for ( le_size_t le_parse = 0; le_parse < le_size; le_parse ++ ) {

            /* update dimension scale */
            le_scale[0] *= 0.5;

            /* analyse address digit and coordinates update */
            if ( ( le_address->as_digit[le_parse] & 0x01 ) != 0 ) le_pose[0] += le_scale[0];

            /* asynchronous dimension management */
            if ( le_parse < LE_ADDRESS_SYNP ) continue;

            /* update dimension scale */
            le_scale[1] *= 0.5;

            /* analyse address digit and coordinates update */
            if ( ( le_address->as_digit[le_parse] & 0x02 ) != 0 ) le_pose[1] += le_scale[1];

            /* asynchronous dimension management */
            if ( le_parse < LE_ADDRESS_SYNA ) continue;

            /* update dimension scale */
            le_scale[2] *= 0.5;

            /* analyse address digit and coordinates update */
            if ( ( le_address->as_digit[le_parse] & 0x04 ) != 0 ) le_pose[2] += le_scale[2];

        }

        /* coordinates denormalisation */
        le_pose[0] = LE_ADDRESS_MINL + le_pose[0] * LE_ADDRESS_RNGL;
        le_pose[1] = LE_ADDRESS_MINA + le_pose[1] * LE_ADDRESS_RNGA;
        le_pose[2] = LE_ADDRESS_MINH + le_pose[2] * LE_ADDRESS_RNGH;

    }

/*
    source - mutator methods
 */

    le_void_t le_address_set_size( le_address_t * const le_address, le_byte_t const le_size ) {

        /* assign address size */
        le_address->as_size = le_size;

    }

    le_void_t le_address_set_mode( le_address_t * const le_address, le_byte_t const le_mode ) {

        /* assign address mode */
        le_address->as_mode = le_mode;

    }

    le_void_t le_address_set_time( le_address_t * const le_address, le_size_t const le_offset, le_time_t const le_time ) {

        /* assign address time */
        le_address->as_times[le_offset] = le_time;

    }

    le_void_t le_address_set_digit( le_address_t * const le_address, le_size_t const le_offset, le_byte_t const le_digit ) {

        /* assign address digit */
        le_address->as_digit[le_offset] = le_digit;

    }

    le_void_t le_address_set_span( le_address_t * const le_address, le_byte_t const le_span ) {

        /* assign address depth */
        le_address->as_span = le_span;

    }

    le_void_t le_address_set_pose( le_address_t * const le_address, le_real_t * const le_pose ) {

        /* coordinates normalisation on [0,1[ range */
        le_pose[0] = ( le_pose[0] - LE_ADDRESS_MINL ) / LE_ADDRESS_RNGL;
        le_pose[1] = ( le_pose[1] - LE_ADDRESS_MINA ) / LE_ADDRESS_RNGA;
        le_pose[2] = ( le_pose[2] - LE_ADDRESS_MINH ) / LE_ADDRESS_RNGH;

        /* composing address */
        for ( le_size_t le_parse = 0 ; le_parse < le_address->as_size; le_parse ++ ) {

            /* update dimension value */
            le_pose[0] *= 2.0;

            /* check dimension value */
            if ( le_pose[0] >= 1.0 ) {

                /* assign address digit component */
                le_address->as_digit[le_parse] = 1;

                /* update dimension value */
                le_pose[0] -= 1.0;

            } else {

                /* assign address digit component */
                le_address->as_digit[le_parse] = 0;

            }

            /* asynchronous dimension management */
            if ( le_parse < LE_ADDRESS_SYNP ) continue;

            /* update dimension value */
            le_pose[1] *= 2.0;

            /* check dimension value */
            if ( le_pose[1] >= 1.0 ) {

                /* assign address digit component */
                le_address->as_digit[le_parse] |= 0x2;

                /* update dimension value */
                le_pose[1] -= 1.0;

            }

            /* asynchronous dimension management */
            if ( le_parse < LE_ADDRESS_SYNA ) continue;

            /* update dimension value */
            le_pose[2] *= 2.0;

            /* check dimension value */
            if ( le_pose[2] >= 1.0 ) {

                /* assign address digit component */
                le_address->as_digit[le_parse] |= 0x4;

                /* update dimension value */
                le_pose[2] -= 1.0;

            }

        }

    }

/*
    source - serialisation methods
 */

    le_size_t le_address_serial( le_address_t * const le_address, le_array_t * const le_array, le_size_t const le_offset, le_enum_t const le_mode ) {

        /* serialisation variables */
        le_byte_t * le_base = le_array_get_byte( le_array ) + le_offset;

        /* serialisation variables */
        le_time_t * le_time = ( le_time_t * ) le_base;

        /* check serialisation mode */
        if ( le_mode == _LE_GET ) {

            /* serialise times */
            le_address->as_times[0] = le_time[0];
            le_address->as_times[1] = le_time[1];

            /* update serial base */
            le_base += LE_ARRAY_ADDR_TIME;

            /* serialise address configration */
            le_address->as_size = le_base[0];
            le_address->as_mode = le_base[1];
            le_address->as_span = le_base[2];

            /* serialise address digits */
            memcpy( le_address->as_digit, le_base + LE_ARRAY_ADDR_DESC, _LE_USE_DEPTH );

        } else {

            /* serialise times */
            le_time[0] = le_address->as_times[0];
            le_time[1] = le_address->as_times[1];

            /* update serial base */
            le_base += LE_ARRAY_ADDR_TIME;

            /* serialise address configuration */
            le_base[0] = le_address->as_size;
            le_base[1] = le_address->as_mode;
            le_base[2] = le_address->as_span;

            /* serialise address digits */
            memcpy( le_base + LE_ARRAY_ADDR_DESC, le_address->as_digit, _LE_USE_DEPTH );

        }

        /* return updated offset */
        return( le_offset + LE_ARRAY_ADDR );

    }

/*
    source - conversion methods
 */

    le_void_t le_address_ct_string( le_address_t const * const le_address, le_char_t * const le_string ) {

        /* conversion buffer variables */
        le_char_t le_data[_LE_USE_DEPTH] = { 0 };

        /* convert address */
        for ( le_size_t le_parse = 0; le_parse < le_address->as_size; le_parse ++ ) {

            /* digit/character conversion */
            le_data[le_parse] = le_address->as_digit[le_parse] + _LE_USE_ASCII_ITOA;

        }

        /* compose address string */
        sprintf( ( char * ) le_string, "/%" _LE_BYTE_P "/%" _LE_TIME_P ",%" _LE_TIME_P "/%s/%" _LE_BYTE_P, le_address->as_mode, le_address->as_times[0], le_address->as_times[1], le_data, le_address->as_span );

    }

    le_void_t le_address_cf_string( le_address_t * const le_address, le_char_t const * const le_string ) {

        /* conversion buffer variables */
        le_char_t le_data[_LE_USE_DEPTH] = { 0 };

        /* decompose address string */
        sscanf( ( char * ) le_string, "/%" _LE_BYTE_S "/%" _LE_TIME_S ",%" _LE_TIME_S "/%[^/]/%" _LE_BYTE_S, & le_address->as_mode, le_address->as_times, le_address->as_times + 1, le_data, & le_address->as_span );

        /* compute address size */
        le_address->as_size = strlen( ( char * ) le_data );

        /* convert address */
        for ( le_size_t le_parse = 0; le_parse < le_address->as_size; le_parse ++ ) {

            /* digit/character conversion */
            le_address->as_digit[le_parse] = le_data[le_parse] - _LE_USE_ASCII_ITOA;

        }

    }

