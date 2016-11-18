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

        /* returned structure variables */
        le_array_t le_array = LE_ARRAY_C;

        /* return created structure */
        return( le_array );

    }

    le_void_t le_array_delete( le_array_t * const le_array ) {

        /* returned structure variables */
        le_array_t le_delete = LE_ARRAY_C;

        /* check array state - memory unallocation */
        if ( le_array->ar_byte != NULL ) free( le_array->ar_byte );

        /* delete structure */
        * le_array = le_delete;

    }

/*
    source - accessor methods
 */

    inline le_size_t le_array_get_size( le_array_t const * const le_array ) {

        /* return array size */
        return( le_array->ar_size );

    }

    inline le_byte_t * le_array_get_byte( le_array_t const * const le_array ) {

        /* return array bytes pointer */
        return( ( le_byte_t * ) le_array->ar_byte );

    }

/*
    source - mutator methods
 */

    le_enum_t le_array_set_memory( le_array_t * const le_array, le_size_t const le_length ) {

        /* memory swapping variables */
        static le_byte_t * le_swap = NULL;

        /* check memory reallocation necessities */
        if ( ( le_array->ar_size += le_length ) >= le_array->as_virt ) {

            /* update virtual size */
            le_array->as_virt += LE_ARRAY_STEP;

            /* array memory allocation */
            if ( ( le_swap = ( le_byte_t * ) realloc( ( void * ) le_array->ar_byte, le_array->as_virt ) ) == NULL ) {

                /* cancel array size modification */
                le_array->ar_size -= le_length;

                /* send message */
                return( LE_ERROR_MEMORY );

            }

            /* swap memory pointers */
            le_array->ar_byte = le_swap;

        }

        /* send message */
        return( LE_ERROR_SUCCESS );

    }

    le_void_t le_array_set_pushsf( le_array_t * const le_array, le_real_t const * const le_pose, le_time_t const le_time, le_data_t const * const le_data ) {

        /* array mapping variables */
        static le_array_sf_t le_map = LE_ARRAY_SF_C;

        /* array memory management - abort */
        if ( le_array_set_memory( le_array, LE_ARRAY_SFL ) != LE_ERROR_SUCCESS ) return;

        /* array mapping computation */
        le_array_sf( le_array->ar_byte, le_array->ar_size - LE_ARRAY_SFL, le_map );

        /* assign elements to array */
        le_map.as_pose[0] = le_pose[0];
        le_map.as_pose[1] = le_pose[1];
        le_map.as_pose[2] = le_pose[2];
        le_map.as_time[0] = le_time;
        le_map.as_data[0] = le_data[0];
        le_map.as_data[1] = le_data[1];
        le_map.as_data[2] = le_data[2];

    }

    le_void_t le_array_set_pushrf( le_array_t * const le_array, le_real_t const * const le_pose, le_data_t const * const le_data ) {

        /* array mapping variables */
        static le_array_rf_t le_map = LE_ARRAY_RF_C;

        /* array memory management - abort */
        if ( le_array_set_memory( le_array, LE_ARRAY_RFL ) != LE_ERROR_SUCCESS ) return;

        /* array mapping computation */
        le_array_rf( le_array->ar_byte, le_array->ar_size - LE_ARRAY_RFL, le_map );

        /* assign elements to array */
        le_map.as_pose[0] = le_pose[0];
        le_map.as_pose[1] = le_pose[1];
        le_map.as_pose[2] = le_pose[2];
        le_map.as_data[0] = le_data[0];
        le_map.as_data[1] = le_data[1];
        le_map.as_data[2] = le_data[2];

    }

    le_void_t le_array_set_pushcf( le_array_t * const le_array, le_size_t const le_size, le_time_t const le_time ) {

        /* array mapping variables */
        static le_array_cf_t le_map = LE_ARRAY_CF_C;

        /* array memory management - abort */
        if ( le_array_set_memory( le_array, LE_ARRAY_CFL ) != LE_ERROR_SUCCESS ) return;

        /* array mapping computation */
        le_array_cf( le_array->ar_byte, le_array->ar_size - LE_ARRAY_CFL, le_map );

        /* assign elements to array */
        le_map.as_size[0] = le_size;
        le_map.as_time[0] = le_time;

    }

/*
    source - i/o methods
 */

    le_enum_t le_array_io_write( le_array_t const * const le_array, le_sock_t const le_socket ) {

        /* block size variables */
        le_size_t le_size = _LE_USE_MTU;

        /* block pointer variables */
        le_byte_t * le_lblock = le_array->ar_byte;
        le_byte_t * le_hblock = le_array->ar_byte + le_size;
        le_byte_t * le_sblock = le_array->ar_byte + le_array->ar_size;

        /* sending array over TCP/IP */
        while ( le_lblock < le_sblock ) {

            /* check block size - compute block size */
            if ( le_hblock > le_sblock ) le_size = le_sblock - le_lblock;

            /* send block to socket - Send message */
            if ( write( le_socket, le_lblock, le_size ) != le_size ) return( LE_ERROR_IO_WRITE );

            /* update block pointers */
            le_lblock = le_hblock;
            le_hblock = le_hblock + le_size;

        }
            
        /* send message */
        return( LE_ERROR_SUCCESS );

    }

    le_enum_t le_array_io_read( le_array_t * const le_array, le_sock_t const le_socket ) {

        /* socket i/o variables */
        le_size_t le_size = 0;
        le_size_t le_read = 0;

        /* receiving array over TCP/IP */
        while ( le_read < _LE_USE_RETRY ) {

            /* array memory allocation - Send message */
            if ( le_array_set_memory( le_array, _LE_USE_MTU ) != LE_ERROR_SUCCESS ) return( LE_ERROR_MEMORY );

            /* array size management */
            le_array->ar_size -= _LE_USE_MTU;

            /* read block from socket */
            if ( ( le_size = read( le_socket, le_array->ar_byte + le_array->ar_size, _LE_USE_MTU ) ) > 0 ) {

                /* array size management */
                le_array->ar_size += le_size;

            /* update reading value */
            le_read = 0; } else { le_read ++; }

        }

        /* send message */
        return( LE_ERROR_SUCCESS );

    }

