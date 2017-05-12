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

    le_size_t le_array_get_size( le_array_t const * const le_array ) {

        /* return array size */
        return( le_array->ar_vsize );

    }

    le_byte_t * le_array_get_byte( le_array_t * const le_array ) {

        /* return array bytes pointer */
        return( le_array->ar_vbyte );

    }

/*
    source - mutator methods
 */

    le_enum_t le_array_set( le_array_t * const le_array, le_size_t const le_length ) {

        /* array memory swap variables */
        le_void_t * le_swap = NULL;

        /* check requierments */
        if ( le_array->ar_rsize >= ( ( le_array->ar_vsize += le_length ) + LE_ARRAY_HEADER ) ) {

            /* send message */
            return( _LE_TRUE );

        }

        /* allocate array memory */
        if ( ( le_swap = realloc( ( le_void_t * ) le_array->ar_rbyte, le_array->ar_rsize += LE_ARRAY_STEP ) ) == NULL ) {

            /* restore array size */
            le_array->ar_rsize -= LE_ARRAY_STEP;

            /* restore array size */
            le_array->ar_vsize -= le_length;

            /* send message */
            return( _LE_FALSE );

        }

        /* update structure references */
        le_array->ar_rbyte = ( le_byte_t * ) ( le_swap );

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

            /* update array size */
            le_array->ar_vsize = le_size;

            /* send message */
            return( _LE_TRUE );

        } else {

            /* allocate array memory */
            if ( ( le_swap = realloc( ( le_void_t * ) le_array->ar_rbyte, le_size + LE_ARRAY_HEADER ) ) == NULL ) {

                /* send message */
                return( _LE_FALSE );

            }

            /* update structure references */
            le_array->ar_rbyte = ( le_byte_t * ) ( le_swap );

            /* update structure references */
            le_array->ar_vbyte = ( le_byte_t * ) ( le_array->ar_rbyte + LE_ARRAY_HEADER );

            /* update array size */
            le_array->ar_rsize = le_size + LE_ARRAY_HEADER;

            /* update array size */
            le_array->ar_vsize = le_size;

            /* send message */
            return( _LE_TRUE );

        }

    }

/*
    source - serialisation methods
 */

    le_size_t le_array_serial( le_array_t * const le_array, le_void_t * const le_bytes, le_size_t const le_length, le_size_t const le_offset, le_enum_t const le_mode ) {

        /* check serialisation mode */
        if ( le_mode == _LE_GET ) {

            /* serialisation */
            memcpy( le_bytes, le_array->ar_vbyte + le_offset, le_length );

        } else {

            /* serialisation */
            memcpy( le_array->ar_vbyte + le_offset, le_bytes, le_length );

        }

        /* return updated offset */
        return( le_offset + le_length );

    }

/*
    source - i/o methods
 */

    le_byte_t le_array_io_write( le_array_t * const le_array, le_byte_t le_mode, le_sock_t const le_socket ) {

        /* socket-array size variables */
        le_size_t le_size = le_array->ar_vsize + LE_ARRAY_HEADER;

        /* serialise array size */
        ( * ( ( le_size_t * ) le_array->ar_rbyte ) ) = le_array->ar_vsize;

        /* serialise array mode */
        ( * ( le_array->ar_rbyte + sizeof( le_size_t ) ) ) = le_mode;

        /* write array on socket */
        if ( write( le_socket, le_array->ar_rbyte, le_size ) != le_size ) {

            /* send message */
            return( LE_MODE_NULL );

        }

        /* send message */
        return( le_mode );

    }

    le_byte_t le_array_io_read( le_array_t * const le_array, le_sock_t const le_socket ) {

        /* reading size variables */
        le_size_t le_size = sizeof( le_size_t );

        /* reading head variables */
        le_size_t le_head = 0;

        /* socket i/o variables */
        le_size_t le_read = 0;
        le_size_t le_fail = 0;

        /* socket array reading */
        while ( ( le_head < le_size ) && ( le_fail < _LE_USE_RETRY ) ) {

            /* socket-array size */
            if ( ( le_read = read( le_socket, ( ( le_byte_t * ) & le_array->ar_vsize ) + le_head, le_size - le_head ) ) > 0 ) {

                /* update head */
                le_head += le_read;

            /* update failure */
            } else { le_fail ++; }

        }

        /* socket-array memory */
        if ( le_array_set_size( le_array, le_array->ar_vsize ) == _LE_FALSE ) {

            /* send message */
            return( LE_MODE_NULL );

        }

        /* socket-array size */
        le_size = le_array->ar_vsize + LE_ARRAY_HEADER;

        /* socket-array reading */
        while ( ( le_head < le_size ) && ( le_fail < _LE_USE_RETRY ) ) {

            /* socket-array content */
            if ( ( le_read = read( le_socket, le_array->ar_rbyte + le_head, le_size - le_head ) ) > 0 ) {

                /* update head */
                le_head += le_read;

            /* update failure */
            } else { le_fail ++; }

        }

        /* serialise array mode */
        return( le_array->ar_rbyte[sizeof( le_size_t )] );

    }

/*
    source - entropic methods
 */

    le_void_t le_array_uf3_encode( le_array_t * const le_array ) {

        /* pointer variables */
        le_byte_t * le_real = le_array->ar_vbyte + LE_ARRAY_UF3;
        le_byte_t * le_trim = le_array->ar_vbyte + LE_ARRAY_UF3;

        /* check necessities */
        if ( le_array->ar_vsize <= LE_ARRAY_UF3 ) {

            /* array unchanged */
            return;

        }

        /* parsing array */
        while ( ( le_real - le_array->ar_vbyte ) < le_array->ar_vsize ) {

            /* encode array */
            ( ( le_trim_t * ) le_trim )[0] = ( ( le_real_t * ) le_real )[0] - ( ( le_real_t * ) le_array->ar_vbyte )[0];
            ( ( le_trim_t * ) le_trim )[1] = ( ( le_real_t * ) le_real )[1] - ( ( le_real_t * ) le_array->ar_vbyte )[1];
            ( ( le_trim_t * ) le_trim )[2] = ( ( le_real_t * ) le_real )[2] - ( ( le_real_t * ) le_array->ar_vbyte )[2];

            /* update pointer */
            le_real += LE_ARRAY_UF3_POSE;
            le_trim += LE_ARRAY_CU3_POSE;

            /* encode array */
            ( ( le_data_t * ) le_trim )[0] = ( ( le_data_t * ) le_real )[0];
            ( ( le_data_t * ) le_trim )[1] = ( ( le_data_t * ) le_real )[1];
            ( ( le_data_t * ) le_trim )[2] = ( ( le_data_t * ) le_real )[2];

            /* update pointer */
            le_real += LE_ARRAY_UF3_DATA;
            le_trim += LE_ARRAY_CU3_DATA;

        }

        /* assign array size */
        le_array->ar_vsize = le_trim - le_array->ar_vbyte;

    }

    le_enum_t le_array_uf3_decode( le_array_t * const le_array ) {

        /* pointer variables */
        le_byte_t * le_real = NULL;

        /* pointer variables */
        le_byte_t * le_trim = le_array->ar_vbyte + LE_ARRAY_UF3;

        /* mirror array variables */
        le_array_t le_mirror = LE_ARRAY_C;

        /* check necessities */
        if ( le_array->ar_vsize <= LE_ARRAY_UF3 ) {

            /* send message */
            return( LE_ERROR_SUCCESS );

        }

        /* create mirror array */
        if ( le_array_set_size( & le_mirror, le_array_uf3_size( le_array ) ) != _LE_TRUE ) {

            /* send message */
            return( LE_ERROR_MEMORY );

        }

        /* copy reference */
        memcpy( le_mirror.ar_vbyte, le_array->ar_vbyte, LE_ARRAY_UF3 );

        /* mirror pointer */
        le_real = le_mirror.ar_vbyte + LE_ARRAY_UF3;

        /* parsing array */
        while ( ( le_real - le_mirror.ar_vbyte ) < le_mirror.ar_vsize ) {

            /* decode array */
            ( ( le_real_t * ) le_real )[0] = ( ( le_trim_t * ) le_trim )[0] + ( ( le_real_t * ) le_array->ar_vbyte )[0];
            ( ( le_real_t * ) le_real )[1] = ( ( le_trim_t * ) le_trim )[1] + ( ( le_real_t * ) le_array->ar_vbyte )[1];
            ( ( le_real_t * ) le_real )[2] = ( ( le_trim_t * ) le_trim )[2] + ( ( le_real_t * ) le_array->ar_vbyte )[2];

            /* update pointer */
            le_real += LE_ARRAY_UF3_POSE;
            le_trim += LE_ARRAY_CU3_POSE;

            /* decode array */
            ( ( le_data_t * ) le_real )[0] = ( ( le_data_t * ) le_trim )[0];
            ( ( le_data_t * ) le_real )[1] = ( ( le_data_t * ) le_trim )[1];
            ( ( le_data_t * ) le_real )[2] = ( ( le_data_t * ) le_trim )[2];

            /* update pointer */
            le_real += LE_ARRAY_UF3_DATA;
            le_trim += LE_ARRAY_CU3_DATA;

        }

        /* delete array */
        le_array_delete( le_array );

        /* assign mirror array */
        ( * le_array ) = le_mirror;

        /* send message */
        return( LE_ERROR_SUCCESS );

    }

