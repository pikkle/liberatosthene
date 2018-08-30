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
    source -
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
        le_size_t le_pedge = 0;
        le_size_t le_qhead = 0;
        le_size_t le_qedge = 0;

        /* address variable */
        le_address_t le_paddr = LE_ADDRESS_C_SIZE( le_scfg );
        le_address_t le_qaddr = LE_ADDRESS_C_SIZE( le_scfg );

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
                le_pedge = le_phead + le_step;

                /* compute merge range */
                le_qhead = le_pedge;
                le_qedge = le_qhead + le_step;

                /* check edge - correction */
                if ( le_pedge > le_size ) le_pedge = le_size;

                /* check edge - correction */
                if ( le_qedge > le_size ) le_qedge = le_size;

                /* range merging algorithm */
                while ( le_index < le_qedge ) {

                    /* exhaust condition */
                    if ( le_phead >= le_pedge ) {

                        /* merge remaining range */
                        memcpy( le_dual[le_swis] + le_index, le_dual[le_swip] + le_qhead, le_qedge - le_qhead );

                        /* update index */
                        le_index = le_qedge;

                    } else if ( le_qhead >= le_qedge ) {

                        /* merge remaining range */
                        memcpy( le_dual[le_swis] + le_index, le_dual[le_swip] + le_phead, le_pedge - le_phead );

                        /* update index */
                        le_index = le_qedge;

                    } else {

                        le_real_t le_pvec[3];

                        memcpy( le_pvec, le_dual[le_swip] + le_phead, sizeof( le_real_t ) * 3 );

                        le_real_t le_qvec[3];

                        memcpy( le_qvec, le_dual[le_swip] + le_qhead, sizeof( le_real_t ) * 3 );

                        /* compute address digit */
                        le_address_set_pose( & le_paddr, le_pvec );

                        /* compute address digit */
                        le_address_set_pose( & le_qaddr, le_qvec );

                        /* compare address index */
                        if ( le_address_get_greater( & le_paddr, & le_qaddr ) == _LE_TRUE ) {

                            /* merge range */
                            memcpy( le_dual[le_swis] + le_index, le_dual[le_swip] + le_qhead, LE_ARRAY_DATA );

                            /* update head */
                            le_qhead += LE_ARRAY_DATA;

                        } else {

                            /* merge range */
                            memcpy( le_dual[le_swis] + le_index, le_dual[le_swip] + le_phead, LE_ARRAY_DATA );

                            /* update head */
                            le_phead += LE_ARRAY_DATA;

                        }

                        /* update index */
                        le_index += LE_ARRAY_DATA;

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

