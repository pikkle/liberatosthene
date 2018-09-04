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

        /* step variable */
        le_size_t le_step = LE_ARRAY_DATA;

        /* index variable */
        le_size_t le_index = 0;

        /* merge variable */
        le_size_t le_phead = 0;
        le_size_t le_pbulk = 0;
        le_size_t le_qhead = 0;
        le_size_t le_qbulk = 0;

        /* size variable */
        le_size_t le_copy = 0;

        /* allocate buffer memory */
        if ( ( le_dual[1] = ( le_byte_t * ) malloc( le_size ) ) == NULL ) {

            /* send message */
            return( NULL );

        }

        /* sorting algorithm */
        while ( le_step < le_size ) {

            /* reset index */
            le_index = 0;

            /* merging algorithm */
            while ( le_index < le_size ) {

                /* compute merge range */
                le_phead = le_index;

                if ( ( le_phead + le_step ) >= le_size ) {

                    le_pbulk = ( le_size - le_phead ) / LE_ARRAY_DATA;

                } else {

                    le_pbulk = le_step / LE_ARRAY_DATA;

                }

                /* compute merge range */
                le_qhead = le_index + le_step;

                if ( ( le_qhead + le_step ) >= le_size ) {

                    le_qbulk = le_size - le_qhead;

                    if ( le_qbulk > 0 ) {

                        le_qbulk /= LE_ARRAY_DATA;

                    } else {

                        le_qbulk = 0;

                    }

                } else {

                    le_qbulk = le_step / LE_ARRAY_DATA;

                }

                /* range merging algorithm */
                while ( ( le_pbulk | le_qbulk ) != 0 ) {

                    /* exhaust condition */
                    if ( le_pbulk == 0 ) {

                        le_copy = le_qbulk * LE_ARRAY_DATA;

                        /* merge remaining range */
                        memcpy( le_dual[le_swis] + le_index, le_dual[le_swip] + le_qhead, le_copy );

                        /* update index */
                        le_index += le_copy;

                        le_qbulk = 0;

                    } else if ( le_qbulk == 0 ) {

                        le_copy = le_pbulk * LE_ARRAY_DATA;

                        /* merge remaining range */
                        memcpy( le_dual[le_swis] + le_index, le_dual[le_swip] + le_phead, le_copy );

                        /* update index */
                        le_index += le_copy;

                        le_pbulk = 0;

                    } else {

                        /* compare address index */
                        if ( le_address_get_greater( ( le_real_t * ) ( le_dual[le_swip] + le_phead ), ( le_real_t * ) ( le_dual[le_swip] + le_qhead ), le_scfg ) == _LE_TRUE ) {

                            /* compute primitive size */
                            le_copy = LE_ARRAY_DATA * le_uv3_get_type( le_dual[le_swip] + le_qhead );

                            /* merge range */
                            memcpy( le_dual[le_swis] + le_index, le_dual[le_swip] + le_qhead, le_copy );

                            /* update head */
                            le_qhead += le_copy;

                            le_qbulk --;

                        } else {

                            /* compute primitive size */
                            le_copy = LE_ARRAY_DATA * le_uv3_get_type( le_dual[le_swip] + le_phead );

                            /* merge range */
                            memcpy( le_dual[le_swis] + le_index, le_dual[le_swip] + le_phead, le_copy );

                            /* update head */
                            le_phead += le_copy;

                            le_pbulk --;

                        }

                        /* update index */
                        le_index += le_copy;

                    }

                }

            }

            /* update step */
            le_step <<= 1;

            /* update switch */
            le_swip = 1 - le_swip;
            le_swis = 1 - le_swis;

        }

        /* release buffer memory */
        free( le_dual[le_swis] );

        /* return buffer pointer */
        return( le_dual[le_swip] );

    }

