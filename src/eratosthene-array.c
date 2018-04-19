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

    # include "eratosthene-array.h"

/*
    source - constructor/destructor methods
 */

    le_array_t le_array_create( le_void_t ) {

        /* created structure variables */
        le_array_t le_array = LE_ARRAY_C;

        /* initialise socket-array */
        le_array_set_size( & le_array, 0 );

        /* return created structure */
        return( le_array );

    }

    le_void_t le_array_delete( le_array_t * const le_array ) {

        /* deleted structure variables */
        le_array_t le_delete = LE_ARRAY_C;

        /* check array pointer */
        if ( le_array->ar_rbyte != NULL ) {

            /* release array memory */
            free( le_array->ar_rbyte );

        }

        /* delete structure */
        ( * le_array ) = le_delete;

    }

/*
    source - accessor methods
 */

    le_size_t le_array_get_size( le_array_t const * const le_array ) {

        /* return size */
        return( le_array->ar_vsize );

    }

    le_byte_t * le_array_get_byte( le_array_t const * const le_array ) {

        /* return bytes pointer */
        return( ( le_byte_t * ) le_array->ar_vbyte );

    }

/*
    source - mutator methods
 */

    le_void_t le_array_set_header( le_array_t * const le_array, le_byte_t const le_mode ) {

        /* serialisation variable */
        le_size_t * le_base = ( le_size_t * ) le_array->ar_rbyte;

        /* serialise size */
        ( * ( le_base ++ ) ) = le_array->ar_vsize;

        /* serialise size */
        ( * ( le_base ++ ) ) = le_array->ar_csize;

        /* serialise mode */
        ( * ( ( le_byte_t * ) le_base ) ) = le_mode;

    }

    le_byte_t le_array_set_array( le_array_t * const le_array ) {

        /* serialisation variable */
        le_size_t * le_base = ( le_size_t * ) le_array->ar_rbyte;

        /* size variable */
        le_size_t le_vsize = * ( le_base ++ );

        /* size variable */
        le_size_t le_csize = * ( le_base ++ );

        /* mode variable */
        le_byte_t le_mode = * ( ( le_byte_t * ) le_base );

        /* socket-array memory allocation */
        if ( le_array_set_size( le_array, le_vsize ) == _LE_FALSE ) {

            /* send message */
            return( LE_MODE_NULL );

        }

        /* socket-array entropic size */
        le_array->ar_csize = le_csize;

        /* return mode */
        return( le_mode );

    }

    le_enum_t le_array_set( le_array_t * const le_array, le_size_t const le_length ) {

        /* swap pointer variables */
        le_byte_t * le_swap = NULL;

        /* check requierments */
        if ( le_array->ar_rsize >= ( ( le_array->ar_vsize += le_length ) + LE_ARRAY_HEADER ) ) {

            /* send message */
            return( _LE_TRUE );

        }

        /* reallocate array memory */
        if ( ( le_swap = ( le_byte_t * ) realloc( ( le_void_t * ) le_array->ar_rbyte, le_array->ar_rsize += LE_ARRAY_STEP ) ) == NULL ) {

            /* restore array size */
            le_array->ar_rsize -= LE_ARRAY_STEP;

            /* restore array size */
            le_array->ar_vsize -= le_length;

            /* send message */
            return( _LE_FALSE );

        }

        /* update structure references */
        le_array->ar_rbyte = le_swap;

        /* update structure references */
        le_array->ar_vbyte = le_array->ar_rbyte + LE_ARRAY_HEADER;

        /* send message */
        return( _LE_TRUE );

    }

    le_enum_t le_array_set_size( le_array_t * const le_array, le_size_t const le_size ) {

        /* swap pointer variables */
        le_byte_t * le_swap = NULL;

        /* memory size variables */
        le_size_t le_real = le_size + LE_ARRAY_HEADER;

        /* check requirements */
        if ( le_array->ar_rsize >= le_real ) {

            /* update array size */
            le_array->ar_vsize = le_size;

            /* send message */
            return( _LE_TRUE );

        }

        /* allocate array memory */
        if ( ( le_swap = ( le_byte_t * ) realloc( ( le_void_t * ) le_array->ar_rbyte, le_real ) ) == NULL ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* update structure references */
        le_array->ar_rbyte = le_swap;

        /* update structure references */
        le_array->ar_vbyte = le_array->ar_rbyte + LE_ARRAY_HEADER;

        /* update array size */
        le_array->ar_rsize = le_real;

        /* update array size */
        le_array->ar_vsize = le_size;

        /* send message */
        return( _LE_TRUE );

    }

    le_enum_t le_array_set_encode( le_array_t const * const le_src, le_array_t * const le_dst ) {

        /* stream variable */
        z_stream le_stream;

        /* check array state */
        if ( le_src->ar_vsize < LE_ARRAY_LIMIT ) {

            /* update array size */
            if ( le_array_set_size( le_dst, le_src->ar_vsize ) == _LE_FALSE ) {

                /* send message */
                return( LE_ERROR_MEMORY );

            }

            /* copy array content */
            memcpy( le_dst->ar_vbyte, le_src->ar_vbyte, le_src->ar_vsize );

            /* update state */
            le_dst->ar_csize = 0;

        } else {

            /* update array size */
            if ( le_array_set_size( le_dst, le_array_entropy( le_src ) ) == _LE_FALSE ) {

                /* send message */
                return( LE_ERROR_MEMORY );

            }

            /* initialise stream */
            le_stream.zalloc = Z_NULL;
            le_stream.zfree  = Z_NULL;
            le_stream.opaque = Z_NULL;

            /* assign stream size */
            le_stream.avail_in  = ( uInt ) le_src->ar_vsize;
            le_stream.avail_out = ( uInt ) le_dst->ar_vsize;

            /* assign stream reference */
            le_stream.next_in  = ( Bytef * ) le_src->ar_vbyte;
            le_stream.next_out = ( Bytef * ) le_dst->ar_vbyte;

            /* initialise encoding */
            deflateInit( & le_stream, Z_DEFAULT_COMPRESSION );

            /* encode array */
            deflate( & le_stream, Z_FINISH );

            /* terminate encoding */
            deflateEnd( & le_stream );

            /* update array size */
            if ( le_array_set_size( le_dst, le_stream.total_out ) == _LE_FALSE ) {

                /* send message */
                return( LE_ERROR_MEMORY );

            }

            /* update state */
            le_dst->ar_csize = le_src->ar_vsize;

        }

        /* send message */
        return( LE_ERROR_SUCCESS );

    }

    le_enum_t le_array_set_decode( le_array_t const * const le_src, le_array_t * const le_dst ) {

        /* stream variable */
        z_stream le_stream;

        /* check array state */
        if ( le_src->ar_csize == 0 ) {
   
            /* update array size */
            if ( le_array_set_size( le_dst, le_src->ar_vsize ) == _LE_FALSE ) {

                /* send message */
                return( LE_ERROR_MEMORY );

            }

            /* copy array content */
            memcpy( le_dst->ar_vbyte, le_src->ar_vbyte, le_src->ar_vsize );

        } else {

            /* update array size */
            if ( le_array_set_size( le_dst, le_src->ar_csize ) == _LE_FALSE ) {

                /* send message */
                return( LE_ERROR_MEMORY );

            }

            /* initialise stream */
            le_stream.zalloc = Z_NULL;
            le_stream.zfree  = Z_NULL;
            le_stream.opaque = Z_NULL;

            /* assign stream size */
            le_stream.avail_in  = ( uInt ) le_src->ar_vsize;
            le_stream.avail_out = ( uInt ) le_dst->ar_vsize;

            /* assign stream reference */
            le_stream.next_in  = ( Bytef * ) le_src->ar_vbyte;
            le_stream.next_out = ( Bytef * ) le_dst->ar_vbyte;

            /* initialise decoding */
            inflateInit( & le_stream );

            /* decode array */
            inflate( & le_stream, Z_NO_FLUSH );

            /* terminate decoding */
            inflateEnd( & le_stream );

        }

        /* update state */
        le_dst->ar_csize = 0;

        /* send message */
        return( LE_ERROR_SUCCESS );

    }

/*
    source - stack methods
 */

    le_void_t le_array_stack_create( le_array_t * const le_array, le_size_t const le_size ) {

        /* parsing socket-array stack */
        for ( le_size_t le_parse = 0; le_parse < le_size; le_parse ++ ) {

            /* create socket-array */
            ( * ( le_array + le_parse ) ) = le_array_create();

        }

    }

    le_void_t le_array_stack_delete( le_array_t * const le_array, le_size_t const le_size ) {

        /* parsing socket-array stack */
        for ( le_size_t le_parse = 0; le_parse < le_size; le_parse ++ ) {

            /* delete socket-array */
            le_array_delete( le_array + le_parse );

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

    le_byte_t le_array_io_put( le_array_t * const le_array, le_array_t * const le_dual, le_byte_t le_mode, le_sock_t const le_socket ) {

        /* check write mode */
        if ( le_dual != NULL ) {

            /* encode array */
            if ( le_array_set_encode( le_array, le_dual ) != LE_ERROR_SUCCESS ) {

                /* send message */
                return( LE_MODE_NULL );

            }

            /* socket-array writing */
            return( le_array_io_write( le_dual, le_mode, le_socket ) );

        } else {

            /* socket-array writing */
            return( le_array_io_write( le_array, le_mode, le_socket ) );

        }

    }

    le_byte_t le_array_io_get( le_array_t * const le_array, le_array_t * const le_dual, le_sock_t const le_socket ) {

        /* array mode variable */
        le_byte_t le_mode = LE_MODE_NULL;

        /* check read mode */
        if ( le_dual != NULL ) {

            /* socket-array reading */
            le_mode = le_array_io_read( le_dual, le_socket );

            /* decode array */
            if ( le_array_set_decode( le_dual, le_array ) != LE_ERROR_SUCCESS ) {

                /* send message */
                return( LE_MODE_NULL );

            }

        } else {

            /* socket-array reading */
            le_mode = le_array_io_read( le_array, le_socket );

        }

        /* return socket-array mode */
        return( le_mode );

    }

    le_byte_t le_array_io_write( le_array_t * const le_array, le_byte_t le_mode, le_sock_t const le_socket ) {

        /* socket-array size variables */
        le_size_t le_size = LE_ARRAY_HEADER + le_array->ar_vsize;

        /* socket i/o variables */
        le_size_t le_head = 0;
        le_size_t le_sent = 0;
        le_size_t le_fail = 0;

        /* create socket-array header */
        le_array_set_header( le_array, le_mode );

        /* socket-array writing */
        while ( le_head < le_size ) {

            /* send socket-array */
            if ( ( le_sent = write( le_socket, le_array->ar_rbyte + le_head, le_size - le_head ) ) > 0 ) {

                /* update head */
                le_head += le_sent;

            } else {

                /* check write state */
                if ( le_sent < 0 ) {

                    /* update and check failure */
                    if ( ( ++ le_fail ) == _LE_USE_RETRY ) {

                        /* send message */
                        return( LE_MODE_NULL );

                    }

                /* return array mode */
                } else { return( LE_MODE_NULL ); }

            }

        }

        /* return socket-array mode */
        return( le_mode );

    }

    le_byte_t le_array_io_read( le_array_t * const le_array, le_sock_t const le_socket ) {

        /* socket-array size variables */
        le_size_t le_size = LE_ARRAY_HEADER;

        /* socket i/o variables */
        le_size_t le_head = 0;
        le_size_t le_read = 0;
        le_size_t le_fail = 0;

        /* array mode variable */
        le_byte_t le_mode = LE_MODE_NULL;

        /* socket-array size */
        le_array_set_size( le_array, 0 );

        /* socket-array reading */
        while ( le_head < le_size ) {

            /* read socket-array */
            if ( ( le_read = read( le_socket, le_array->ar_rbyte + le_head, le_size - le_head ) ) > 0 ) {

                /* update head */
                le_head += le_read;

                /* socket socket-array size */
                if ( ( le_mode == LE_MODE_NULL ) && ( le_head >= LE_ARRAY_HEADER ) ) {

                    /* rebuild socket-array */
                    if ( ( le_mode = le_array_set_array( le_array ) ) != LE_MODE_NULL ) {

                        /* update read size */
                        le_size += le_array->ar_vsize;

                    /* send message */
                    } else { return( LE_MODE_NULL ); }

                }

            } else {

                /* check read state */
                if ( le_read < 0 ) {

                    /* update and check failure */
                    if ( ( ++ le_fail ) == _LE_USE_RETRY ) {

                        /* send message */
                        return( LE_MODE_NULL );

                    }

                /* send message */
                } else { return( LE_MODE_NULL ); }

            }

        }

        /* return socket-array mode */
        return( le_mode );

    }

