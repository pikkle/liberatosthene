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
 *
 */

    # include "eratosthene-array.h"

/*
    source - accessor methods
 */

    le_size_t le_array_get_size( le_array_t * le_array ) {

        /* Return array size */
        return( le_array->ar_head );

    }

    le_byte_t * le_array_get_byte( le_array_t * le_array ) {

        /* Return array bytes pointer */
        return( le_array->ar_data );

    }

/*
    source - mutator methods
 */

    le_enum_t le_array_set_push( le_array_t * le_array, le_real_t const * const le_pose, le_time_t const le_time, le_data_t const * const le_data ) {

        /* Allocation swap variables */
        le_byte_t * le_swap = NULL;

        /* Array pointer variables */
        le_real_t * le_pose_p = NULL;
        le_time_t * le_time_p = NULL;
        le_data_t * le_data_p = NULL;

        /* Check reallocation necessities */
        if ( ( le_array->ar_head + LE_ARRAY_ELEM_LEN ) >= le_array->ar_size ) {

            /* Update array size */
            le_array->ar_size += LE_ARRAY_STEP * LE_ARRAY_ELEM_LEN;

            /* Check array state */
            if ( le_array->ar_data == NULL ) {

                /* Array memory allocation */
                if ( ( le_array->ar_data = ( le_byte_t * ) malloc( le_array->ar_size ) ) == NULL ) {

                    /* Reset array size */
                    le_array->ar_size = 0;

                    /* Send message */
                    return( LE_ERROR_MEMORY );

                }

            } else {

                /* Array memory reallocation */
                if ( ( le_swap = realloc( ( void * ) le_array->ar_data, le_array->ar_size ) ) == NULL ) {

                    /* Reset array size */
                    le_array->ar_size -= LE_ARRAY_STEP * LE_ARRAY_ELEM_LEN;

                    /* Send message */
                    return( LE_ERROR_MEMORY );

                }

                /* Memory reallocation pointer allocation */
                le_array->ar_data = le_swap;

            }

        }

        /* Compute pointers */
        le_pose_p = ( le_real_t * ) ( le_array->ar_data + le_array->ar_head );
        le_time_p = ( le_time_t * ) ( le_pose_p + 3 );
        le_data_p = ( le_data_t * ) ( le_time_p + 1 );

        /* Update array head */
        le_array->ar_head += LE_ARRAY_ELEM_LEN;

        /* Inject spatial components */
        * ( le_pose_p     ) = * ( le_pose     );
        * ( le_pose_p + 1 ) = * ( le_pose + 1 );
        * ( le_pose_p + 2 ) = * ( le_pose + 2 );

        /* Inject temporal components */
        * ( le_time_p ) = le_time;

        /* Inject colorimetric components */
        * ( le_data_p     ) = * ( le_data     );
        * ( le_data_p + 1 ) = * ( le_data + 1 );
        * ( le_data_p + 2 ) = * ( le_data + 2 );

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

