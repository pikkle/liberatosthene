/*
 *  liberatosthene
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2018 DHLAB, EPFL
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

    # include "eratosthene-uv3.h"

/*
    source - mutator methods
 */

    le_byte_t * le_uv3_set_sort( le_byte_t * const le_buffer, le_size_t const le_size, le_size_t const le_scfg ) {

        /* buffer variable */
        le_byte_t * le_dual[2] = { le_buffer, NULL };

        /* switch variable */
        le_size_t le_swip = 0;
        le_size_t le_swis = 1;

        le_size_t le_copy = 0;

        /* algorithm variable */
        le_size_t le_range = 1;
        le_size_t le_merge = 2;
        le_size_t le_count = 0;

        /* merge variable */
        le_size_t le_index = 0;
        le_size_t le_start = 0;
        le_size_t le_middl = 0;
        le_size_t le_inner = 0;
        le_size_t le_limit = 0;

        /* allocate buffer memory */
        if ( ( le_dual[le_swis] = ( le_byte_t * ) malloc( le_size ) ) == NULL ) {

            /* send message */
            return( NULL );

        }

        /* sorting algorithm */
        while ( le_merge > 1 ) {

            /* reset algorithm variable */
            le_merge = 0;

            /* reset index */
            le_index = 0;

            le_size_t le_track = le_dual[le_swip][LE_ARRAY_DATA_POSE];

            /* merging procedure */
            while ( le_index < le_size ) {

                /* range specification */
                le_limit = ( le_start = le_index ) + LE_ARRAY_DATA_POSE;

                le_count = 0;

                le_enum_t le_flag = _LE_FALSE;

                while ( le_flag == _LE_FALSE ) {

                    if ( ( le_limit += LE_ARRAY_DATA ) < le_size ) {

                        if ( ( -- le_track ) == 0 ) {

                            le_track = le_dual[le_swip][le_limit];

                            if ( ( ++ le_count ) == le_range ) {

                                le_middl = ( le_inner = le_limit - LE_ARRAY_DATA_POSE );

                                le_flag = _LE_TRUE;

                            }

                        }

                    } else {

                        le_limit = ( le_middl = ( le_inner = le_size ) );

                        le_flag = _LE_TRUE;

                    }

                }

                if ( le_inner < le_size ) {

                    le_flag = _LE_FALSE;

                    le_count = 0;

                    while ( le_flag == _LE_FALSE ) {

                        if ( ( le_limit += LE_ARRAY_DATA ) < le_size ) {

                            if ( ( -- le_track ) == 0 ) {

                                le_track = le_dual[le_swip][le_limit];

                                if ( ( ++ le_count ) == le_range ) {

                                    le_limit -= LE_ARRAY_DATA_POSE;

                                    le_flag = _LE_TRUE;

                                }

                            }

                        } else {

                            le_limit = le_size;

                            le_flag = _LE_TRUE;

                        }

                    }

                }

                while ( le_index < le_limit ) {

                    if ( le_start == le_middl ) {

                        memcpy( le_dual[le_swis] + le_index, le_dual[le_swip] + le_inner, le_limit - le_inner );

                        le_index = le_limit;

                    } else if ( le_inner == le_limit ) {

                        memcpy( le_dual[le_swis] + le_index, le_dual[le_swip] + le_start, le_middl - le_start );

                        le_index = le_limit;

                    } else {

                        if ( le_address_get_greater( ( le_real_t * ) ( le_dual[le_swip] + le_start ), ( le_real_t * ) ( le_dual[le_swip] + le_inner ), le_scfg ) == _LE_TRUE ) {

                            le_copy = le_uv3_get_type( le_dual[le_swip] + le_inner ) * LE_ARRAY_DATA;

                            memcpy( le_dual[le_swis] + le_index, le_dual[le_swip] + le_inner, le_copy );

                            le_inner += le_copy;

                        } else {

                            le_copy = le_uv3_get_type( le_dual[le_swip] + le_start ) * LE_ARRAY_DATA;

                            memcpy( le_dual[le_swis] + le_index, le_dual[le_swip] + le_start, le_copy );

                            le_start += le_copy;

                        }

                        le_index += le_copy;

                    }

                }

                le_merge ++;

            }

            /* update algorithm variable */
            le_range *= 2;

            /* update switch */
            le_swip = 1 - le_swip;
            le_swis = 1 - le_swis;

        }

        /* release buffer memory */
        free( le_dual[le_swis] );

        /* return buffer pointer */
        return( le_dual[le_swip] );

    }

