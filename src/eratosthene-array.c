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

    # include "eratosthene-array.h"

/*
    source - constructor/destructor methods
 */

    le_array_t le_array_create( le_void_t ) {

        /* created structure variables */
        le_array_t le_array = LE_ARRAY_C;

        /* return created structure */
        return( le_array );

    }

    le_void_t le_array_delete( le_array_t * const le_array ) {

        /* deleted structure variables */
        le_array_t le_delete = LE_ARRAY_C;

        /* check array state - memory unallocation */
        if ( le_array->ar_rbyte != NULL ) free( le_array->ar_rbyte );

        /* delete structure */
        ( * le_array ) = le_delete;

    }

/*
    source - accessor methods
 */

    le_byte_t le_array_get_type( le_array_t const * const le_array ) {

        /* return header type */
        return( le_array->ar_rbyte[sizeof( le_size_t )] );

    }

    le_size_t le_array_get_size( le_array_t const * const le_array ) {

        /* return array size */
        return( le_array->ar_vsize );

    }

    le_byte_t * le_array_get_byte( le_array_t const * const le_array ) {

        /* return array bytes pointer */
        return( ( le_byte_t * ) le_array->ar_vbyte );

    }

/*
    source - mutator methods
 */

    le_void_t le_array_set_type( le_array_t * const le_array, le_byte_t le_type ) {

        /* update header type */
        le_array->ar_rbyte[sizeof( le_size_t )] = le_type;

    }

    le_enum_t le_array_set( le_array_t * const le_array, le_size_t const le_length ) {

        /* array memory swap variables */
        le_void_t * le_swap = NULL;

        /* check requierments */
        if ( le_array->ar_rsize >= ( ( le_array->ar_vsize += le_length ) + LE_ARRAY_HEADER ) ) {

            /* send message */
            return( _LE_TRUE );

        }

        /* allocate array memory */
        if ( ( le_swap =  realloc( ( le_void_t * ) le_array->ar_rbyte, le_array->ar_rsize += LE_ARRAY_STEP ) ) == NULL ) {

            /* restore array size */
            le_array->ar_rsize -= LE_ARRAY_STEP;

            /* restore array size */
            le_array->ar_vsize -= le_length;

            /* send message */
            return( _LE_FALSE );

        }

        /* update structure references */
        le_array->ar_rbyte = ( le_byte_t * ) le_swap;

        /* update structure references */
        le_array->ar_vbyte = ( le_byte_t * ) ( le_array->ar_rbyte + LE_ARRAY_HEADER );

        /* send message */
        return( _LE_TRUE );

    }

    le_enum_t le_array_set_size( le_array_t * const le_array, le_size_t const le_size ) {

        /* array memory swap variables */
        le_void_t * le_swap = NULL;

        /* check requirements */
        if ( le_array->ar_rsize >= ( le_size + LE_ARRAY_HEADER ) ) {

            /* send message */
            return( _LE_TRUE );

        }

        /* allocate array memory */
        if ( ( le_swap = realloc( ( le_void_t * ) le_array->ar_rbyte, le_size + LE_ARRAY_HEADER ) ) == NULL ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* update structure references */
        le_array->ar_rbyte = ( le_byte_t * ) le_swap;

        /* update structure references */
        le_array->ar_vbyte = ( le_byte_t * ) ( le_array->ar_rbyte + LE_ARRAY_HEADER );

        /* update array size */
        le_array->ar_rsize = le_size + LE_ARRAY_HEADER;

        /* update structure references */
        le_array->ar_vsize = le_size;

        /* send message */
        return( _LE_TRUE );

    }

/*
    source - mapping methods
 */

    le_void_t le_array_map_sd( le_array_t * const le_array, le_real_t const * const le_pose, le_data_t const * const le_data ) {

        /* array memory management - abort mapping */
        if ( le_array_set( le_array, LE_ARRAY_SD ) != _LE_TRUE ) return;

        /* mapping offset variables */
        le_byte_t * le_offset = le_array->ar_vbyte + le_array->ar_vsize - LE_ARRAY_SD;

        /* mapping of spatial components */
        ( ( le_real_t * ) le_offset )[0] = le_pose[0];
        ( ( le_real_t * ) le_offset )[1] = le_pose[1];
        ( ( le_real_t * ) le_offset )[2] = le_pose[2];

        /* update mapping offset */
        le_offset += sizeof( le_real_t ) * 3;

        /* mapping of data components */
        ( ( le_data_t * ) le_offset )[0] = le_data[0];
        ( ( le_data_t * ) le_offset )[1] = le_data[1];
        ( ( le_data_t * ) le_offset )[2] = le_data[2];

    }

    le_void_t le_array_map_dt( le_array_t * const le_array, le_size_t const le_size, le_time_t const le_time ) {

        /* array memory management - abort mapping */
        if ( le_array_set( le_array, LE_ARRAY_DT ) != _LE_TRUE ) return;

        /* mapping offset variables */
        le_byte_t * le_offset = le_array->ar_vbyte + le_array->ar_vsize - LE_ARRAY_DT;

        /* mapping of size parameter */
        ( ( le_size_t * ) le_offset )[0] = le_size;

        /* update mapping offset */
        le_offset += sizeof( le_size_t );

        /* mapping of time parameter */
        ( ( le_time_t * ) le_offset )[0] = le_time;

    }

/*
    source - i/o methods
 */

    le_enum_t le_array_io_write( le_array_t * const le_array, le_sock_t const le_socket ) {

        /* socket-array size variables */
        le_size_t le_size = le_array->ar_vsize + LE_ARRAY_HEADER;

        /* serialise array size */
        ( * ( ( le_size_t * ) le_array->ar_rbyte ) ) = le_array->ar_vsize;

        /* serialise array mode */
        /* ... */

        /* write array on socket */
        if ( write( le_socket, le_array->ar_rbyte, le_size ) != le_size ) {

            /* send message */
            return( LE_ERROR_IO_WRITE );

        }

        /* send message */
        return( LE_ERROR_SUCCESS );

    }

    le_enum_t le_array_io_read( le_array_t * const le_array, le_sock_t const le_socket ) {

        /* socket-array variables */
        le_size_t le_size = sizeof( le_size_t );
        le_size_t le_read = 0;
        le_size_t le_fail = 0;

        /* read socket-array header */
        if ( read( le_socket, & le_array->ar_vsize, sizeof( le_size_t ) ) != sizeof( le_size_t ) ) {

            /* send message */
            return( LE_ERROR_IO_READ );

        }

        /* allocate socket-array memory */
        if ( le_array_set_size( le_array, le_array->ar_vsize ) == _LE_FALSE ) {

            /* send message */
            return( LE_ERROR_MEMORY );

        }

        /* read socket-array */
        while ( ( le_size < le_array->ar_rsize ) && ( le_fail < _LE_USE_RETRY ) ) {

            /* read socket block */
            if ( ( le_read = read( le_socket, le_array->ar_rbyte + le_size, le_array->ar_rsize ) ) > 0 ) {

                /* update read size */
                le_size += le_read;

            /* update read failure */
            } else { le_fail ++; }

        }

        /* unserialise array mode */
        /* ... */

        /* send message */
        return( LE_ERROR_SUCCESS );

    }

