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

    # include "eratosthene-array.h"

/*
    source - constructor/destructor methods
 */

    le_array_t le_array_create( le_void_t ) {

        /* Returned structure variables */
        le_array_t le_array = LE_ARRAY_C;

        /* Return created structure */
        return( le_array );

    }

    le_void_t le_array_delete( le_array_t * const le_array ) {

        /* Returned structure variables */
        le_array_t le_delete = LE_ARRAY_C;

        /* Check array state - memory unallocation */
        if ( le_array->ar_byte != NULL ) free( le_array->ar_byte );

        /* Delete structure */
        * le_array = le_delete;

    }

/*
    source - accessor methods
 */

    le_size_t le_array_get_size( le_array_t const * const le_array ) {

        /* Return array size */
        return( le_array->ar_size );

    }

    le_byte_t * le_array_get_byte( le_array_t const * const le_array ) {

        /* Return array bytes pointer */
        return( ( le_byte_t * ) le_array->ar_byte );

    }

/*
    source - mutator methods
 */

    le_enum_t le_array_set_memory( le_array_t * const le_array, le_size_t const le_length ) {

        /* Memory swapping variables */
        le_byte_t * le_swap = NULL;

        /* Check memory reallocation necessities */
        if ( ( le_array->ar_size += le_length ) >= le_array->as_virt ) {

            /* Update virtual size */
            le_array->as_virt += LE_ARRAY_STEP;

            /* Array memory allocation */
            if ( ( le_swap = ( le_byte_t * ) realloc( ( void * ) le_array->ar_byte, le_array->as_virt ) ) == NULL ) {

                /* Cancel array size modification */
                le_array->ar_size -= le_length;

                /* Send message */
                return( LE_ERROR_MEMORY );

            }

            /* Swap memory pointers */
            le_array->ar_byte = le_swap;

        }

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

    le_void_t le_array_set_pushsf( le_array_t * const le_array, le_real_t const * const le_pose, le_time_t const le_time, le_data_t const * const le_data ) {

        /* Array mapping variables */
        le_array_sf_t le_map = LE_ARRAY_SF_C;

        /* Array memory management - abort */
        if ( le_array_set_memory( le_array, LE_ARRAY_SFL ) != LE_ERROR_SUCCESS ) return;

        /* Array mapping computation */
        le_array_sf( le_array->ar_byte, le_array->ar_size - LE_ARRAY_SFL, le_map );

        /* Assign elements to array */
        le_map.as_pose[0] = le_pose[0];
        le_map.as_pose[1] = le_pose[1];
        le_map.as_pose[2] = le_pose[2];
        le_map.as_time[0] = le_time;
        le_map.as_data[0] = le_data[0];
        le_map.as_data[1] = le_data[1];
        le_map.as_data[2] = le_data[2];

    }

    le_void_t le_array_set_pushrf( le_array_t * const le_array, le_real_t const * const le_pose, le_data_t const * const le_data ) {

        /* Array mapping variables */
        le_array_rf_t le_map = LE_ARRAY_RF_C;

        /* Array memory management - abort */
        if ( le_array_set_memory( le_array, LE_ARRAY_RFL ) != LE_ERROR_SUCCESS ) return;

        /* Array mapping computation */
        le_array_rf( le_array->ar_byte, le_array->ar_size - LE_ARRAY_RFL, le_map );

        /* Assign elements to array */
        le_map.as_pose[0] = le_pose[0];
        le_map.as_pose[1] = le_pose[1];
        le_map.as_pose[2] = le_pose[2];
        le_map.as_data[0] = le_data[0];
        le_map.as_data[1] = le_data[1];
        le_map.as_data[2] = le_data[2];

    }

    le_void_t le_array_set_pushtf( le_array_t * const le_array, le_time_t const le_time ) {

        /* Array mapping variables */
        le_array_tf_t le_map = LE_ARRAY_TF_C;

        /* Array memory management - abort */
        if ( le_array_set_memory( le_array, LE_ARRAY_TFL ) != LE_ERROR_SUCCESS ) return;

        /* Array mapping computation */
        le_array_tf( le_array->ar_byte, le_array->ar_size - LE_ARRAY_TFL, le_map );

        /* Assign elements to array */
        le_map.as_time[0] = le_time;

    }

    le_void_t le_array_set_pushcf( le_array_t * const le_array, le_size_t const le_size, le_time_t const le_time ) {

        /* Array mapping variables */
        le_array_cf_t le_map = LE_ARRAY_CF_C;

        /* Array memory management - abort */
        if ( le_array_set_memory( le_array, LE_ARRAY_CFL ) != LE_ERROR_SUCCESS ) return;

        /* Array mapping computation */
        le_array_cf( le_array->ar_byte, le_array->ar_size - LE_ARRAY_CFL, le_map );

        /* Assign elements to array */
        le_map.as_size[0] = le_size;
        le_map.as_time[0] = le_time;

    }

/*
    source - i/o methods
 */

    le_enum_t le_array_io_write( le_array_t const * const le_array, le_sock_t const le_socket ) {

        /* Array pointer variables */
        le_byte_t * le_lbloc = le_array->ar_byte;
        le_byte_t * le_hbloc = le_array->ar_byte + _LE_USE_MTU;
        le_byte_t * le_sbloc = le_array->ar_byte + le_array->ar_size;

        /* Segment size variables */
        le_size_t le_size = 0;

        /* Check consistency - send message */
        if ( le_socket == _LE_SOCK_NULL ) return( LE_ERROR_IO_SOCKET );

        /* Sending array over TCP/IP */
        while ( le_lbloc < le_sbloc ) {

            /* Compute bloc size */
            le_size = ( le_size_t ) ( le_hbloc > le_sbloc ? le_sbloc - le_lbloc : le_hbloc - le_lbloc );

            /* Send bloc to socket - Send message */
            if ( write( le_socket, le_lbloc, le_size ) != le_size ) return( LE_ERROR_IO_WRITE );

            /* Update pointers */
            le_lbloc += _LE_USE_MTU;
            le_hbloc += _LE_USE_MTU;

        }
            
        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

    le_enum_t le_array_io_stream( le_array_t * const le_array, le_sock_t const le_socket, le_enum_t const le_mode ) {

        /* Array base variables */
        le_byte_t * le_base = le_array->ar_byte;

        /* Check mode */
        if ( le_mode == 0 ) {

            /* Streaming loop */
            while ( ( le_size_t ) ( le_base - le_array->ar_byte ) + _LE_USE_MTU < le_array->ar_size ) {

                /* Write block to socket */
                if ( write( le_socket, le_base, _LE_USE_MTU ) != _LE_USE_MTU ) return( LE_ERROR_IO_WRITE );

                /* Update base pointer */
                le_base += _LE_USE_MTU;

            }

            /* Compute remaining size */
            le_array->ar_size = le_array->ar_size - ( le_size_t ) ( le_base - le_array->ar_byte );

            /* Rebase array memory */
            memmove( le_array->ar_byte, le_base, le_array->ar_size );

        /* Send array remaining segment */
        } else { return( le_array_io_write( le_array, le_socket ) ); }

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

    le_enum_t le_array_io_read( le_array_t * const le_array, le_sock_t const le_socket ) {

        /* Returned value variables */
        le_enum_t le_return = LE_ERROR_SUCCESS;

        /* Socket i/o variables */
        le_size_t le_size = 0;
        le_size_t le_read = 0;

        /* Check consistency - send message */
        if ( le_socket == _LE_SOCK_NULL ) return( LE_ERROR_IO_SOCKET );

        /* Receiving array over TCP/IP */
        while ( le_read < _LE_USE_RETRY ) {

            /* Array memory allocation - Send message */
            if ( ( le_return = le_array_set_memory( le_array, _LE_USE_MTU ) ) != LE_ERROR_SUCCESS ) return( le_return );

            /* Array size management */
            le_array->ar_size -= _LE_USE_MTU;

            /* Read bloc on socket */
            if ( ( le_size = read( le_socket, le_array->ar_byte + le_array->ar_size, _LE_USE_MTU ) ) > 0 ) {

                /* Array size management */
                le_array->ar_size += le_size;

            /* Update reading value */
            le_read = 0; } else { le_read ++; }

        }

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

