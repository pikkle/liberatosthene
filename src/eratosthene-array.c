/*
 *  liberatosthene - eratosthene indexation server
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
        if ( le_array->ar_byte != NULL ) free( le_array->ar_byte );

        /* delete structure */
        ( * le_array ) = le_delete;

    }

/*
    source - accessor methods
 */

    le_size_t le_array_get_size( le_array_t const * const le_array ) {

        /* return array size */
        return( le_array->ar_size );

    }

    le_byte_t * le_array_get_byte( le_array_t const * const le_array ) {

        /* return array bytes pointer */
        return( ( le_byte_t * ) le_array->ar_byte );

    }

/*
    source - mutator methods
 */

    le_enum_t le_array_set( le_array_t * const le_array, le_size_t const le_length ) {

        /* persistent memory variables */
        le_byte_t * le_swap = NULL;

        /* check requirements - update array size */
        if ( ( le_array->ar_size += le_length ) >= le_array->ar_virt ) {

            /* update memory size */
            le_array->ar_virt += LE_ARRAY_STEP;

            /* array memory (re)allocation */
            if ( ( le_swap = ( le_byte_t * ) realloc( ( void * ) le_array->ar_byte, le_array->ar_virt ) ) == NULL ) {

                /* cancel array size update */
                le_array->ar_size -= le_length;

                /* send message */
                return( _LE_FALSE );

            }

            /* assign (re)allocated memory */
            le_array->ar_byte = le_swap;

        }

        /* send message */
        return( _LE_TRUE );

    }

    le_enum_t le_array_set_size( le_array_t * const le_array, le_size_t const le_size ) {

        /* persistent memory variables */
        le_byte_t * le_swap = NULL;

        /* check array virtual size */
        if ( le_size > le_array->ar_virt ) {

            /* update virtual size */
            le_array->ar_virt = le_size;

            /* array memory (re)allocation */
            if ( ( le_swap = ( le_byte_t * ) realloc( ( void * ) le_array->ar_byte, le_array->ar_virt ) ) == NULL ) {

                /* send message */
                return( _LE_FALSE );

            }

            /* assign (ra)allocated memory */
            le_array->ar_byte = le_swap;

        }

        /* update array size */
        le_array->ar_size = le_size;

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
        le_byte_t * le_offset = le_array->ar_byte + le_array->ar_size - LE_ARRAY_SD;

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
        le_byte_t * le_offset = le_array->ar_byte + le_array->ar_size - LE_ARRAY_DT;

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

    le_enum_t le_array_io_write( le_array_t const * const le_array, le_sock_t const le_socket ) {

        /* block size variables */
        le_size_t le_size = _LE_USE_MTU;

        /* block pointer variables */
        le_byte_t * le_lblock = le_array->ar_byte;
        le_byte_t * le_hblock = le_array->ar_byte + le_size;
        le_byte_t * le_sblock = le_array->ar_byte + le_array->ar_size;

        /* socket writing */
        while ( le_lblock < le_sblock ) {

            /* check block size - compute block size */
            if ( le_hblock > le_sblock ) le_size = le_sblock - le_lblock;

            /* send block to socket - send message */
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

        /* socket reeading */
        while ( le_read < _LE_USE_RETRY ) {

            /* array memory allocation - Send message */
            if ( le_array_set( le_array, _LE_USE_MTU ) != _LE_TRUE ) return( LE_ERROR_MEMORY );

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

