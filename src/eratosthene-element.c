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

    # include "eratosthene-element.h"

/*
    source - accessor methods
 */

    le_real_t * le_element_get_pose( le_element_t const * const le_element, le_size_t const le_offset ) {

        /* Return spatial components */
        return( ( le_real_t * ) ( le_element->em_elem + ( le_offset * LE_ELEMENT_ELEN ) ) );

    }

    le_data_t * le_element_get_data( le_element_t const * const le_element, le_size_t const le_offset ) {

        /* Return colorimetric components */
        return( ( le_data_t * ) ( le_element->em_elem + ( le_offset * LE_ELEMENT_ELEN ) + LE_ELEMENT_SLEN ) );

    }

/*
    source - mutator methods
 */

    le_void_t le_element_set_clear( le_element_t * const le_element ) {

        /* Check array state */
        if ( le_element->em_size > 0 ) {

            /* Unallocate array memory */
            free( le_element->em_elem );

            /* Invalidate array pointer */
            le_element->em_elem = NULL;

            /* Reset array size */
            le_element->em_size = 0;           

        }

    }

    le_enum_t le_element_set( le_element_t * le_element, le_real_t const * const le_pose, le_data_t const * const le_data ) {

        /* Allocation swap variables */
        le_byte_t * le_swap = NULL;

        /* Array pointer variables */
        le_byte_t * le_pose_p = NULL;
        le_byte_t * le_data_p = NULL;

        /* Check array capacity */
        if ( ( le_element->em_head + LE_ELEMENT_ELEN ) >= le_element->em_size ) {

            /* Update array size */
            le_element->em_size += LE_ELEMENT_STEP;

            /* Check array state */
            if ( le_element->em_elem == NULL ) {

                /* Array memory allocation */
                if ( ( le_element->em_elem = ( le_byte_t * ) malloc( le_element->em_size ) ) == NULL ) {

                    /* Reset array size */
                    le_element->em_size = 0;

                    /* Send message */
                    return( LE_ERROR_MEMORY );

                }

            } else {

                /* Array memory reallocation */
                if ( ( le_swap = realloc( ( void * ) le_element->em_elem, le_element->em_size ) ) == NULL ) {

                    /* Reset array size */
                    le_element->em_size -= LE_ELEMENT_STEP;

                    /* Send message */
                    return( LE_ERROR_MEMORY );

                }

                /* Assign reallocated memory */
                le_element->em_elem = le_swap;

            }

        }

        /* Compute array pointers */
        le_pose_p = le_element->em_elem + le_element->em_head;
        le_data_p = le_element->em_elem + le_element->em_head + LE_ELEMENT_SLEN;        

        /* Update array head */
        le_element->em_head += LE_ELEMENT_ELEN;

        /* Inject spatial components */
        * ( ( ( le_real_t * ) le_pose_p )     ) = * ( le_pose     );
        * ( ( ( le_real_t * ) le_pose_p ) + 1 ) = * ( le_pose + 1 );
        * ( ( ( le_real_t * ) le_pose_p ) + 2 ) = * ( le_pose + 2 );

        /* Inject colorimetric components */
        * ( ( ( le_data_t * ) le_data_p )     ) = * ( le_data     );
        * ( ( ( le_data_t * ) le_data_p ) + 1 ) = * ( le_data + 1 );
        * ( ( ( le_data_t * ) le_data_p ) + 2 ) = * ( le_data + 2 );

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

