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

    # include "eratosthene-array.h"

/*
    source - constructor/destructor methods
 */

    le_array_t le_array_create( le_void_t ) {

        /* Array variables */
        le_array_t le_array = LE_ARRAY_C;

        /* Return initialised array */
        return( le_array );

    }

    le_void_t le_array_delete( le_array_t * const le_array ) {

        /* Check array state */
        if ( le_array->ar_vsze > 0 ) {

            /* Unallocate array memory */
            free( le_array->ar_byte );

            /* Reset array size */
            le_array->ar_vsze = 0;
            le_array->ar_size = 0;

        }

    }

/*
    source - accessor methods
 */

    le_size_t le_array_get_size( le_array_t * le_array ) {

        /* Return array size */
        return( le_array->ar_size );

    }

    le_byte_t * le_array_get_byte( le_array_t * le_array ) {

        /* Return array bytes pointer */
        return( le_array->ar_byte );

    }

/*
    source - mutator methods
 */

    le_enum_t le_array_set_push( le_array_t * le_array, le_real_t const * const le_pose, le_time_t const le_time, le_data_t const * const le_data ) {

        /* Memory swap variables */
        le_byte_t * le_swap = NULL;

        /* Array pointer variables */
        le_real_t * le_ptrp = NULL;
        le_time_t * le_ptrt = NULL;
        le_data_t * le_ptrd = NULL;

        /* Check necessities */
        if ( ( le_array->ar_size + LE_ARRAY_ELEM_LEN ) >= le_array->ar_vsze ) {

            /* Update virtual size */
            le_array->ar_vsze += ( LE_ARRAY_STEP * LE_ARRAY_ELEM_LEN );

            /* Check array state */
            if ( le_array->ar_vsze == ( LE_ARRAY_STEP * LE_ARRAY_ELEM_LEN ) ) {

                /* Array memory allocation */
                if ( ( le_array->ar_byte = ( le_byte_t * ) malloc( le_array->ar_vsze ) ) == NULL ) {

                    /* Reset virtual size */
                    le_array->ar_vsze = 0;

                    /* Send message */
                    return( LE_ERROR_MEMORY );

                }

            } else {

                /* Array memory reallocation */
                if ( ( le_swap = realloc( ( void * ) le_array->ar_byte, le_array->ar_vsze ) ) == NULL ) {

                    /* Reset virtual size */
                    le_array->ar_vsze -= ( LE_ARRAY_STEP * LE_ARRAY_ELEM_LEN );

                    /* Send message */
                    return( LE_ERROR_MEMORY );

                }

                /* Swap memory pointers */
                le_array->ar_byte = le_swap;

            }

        }

        /* Compute pointers */
        le_ptrp = ( le_real_t * ) ( le_array->ar_byte + le_array->ar_size );
        le_ptrt = ( le_time_t * ) ( le_ptrp + 3 );
        le_ptrd = ( le_data_t * ) ( le_ptrt + 1 );

        /* Push element coordinates */
        le_ptrp[0] = le_pose[0];
        le_ptrp[1] = le_pose[1];
        le_ptrp[2] = le_pose[2];
        le_ptrt[0] = le_time;
        le_ptrd[0] = le_data[0];
        le_ptrd[1] = le_data[1];
        le_ptrd[2] = le_data[2];

        /* Update array head */
        le_array->ar_size += LE_ARRAY_ELEM_LEN;

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

