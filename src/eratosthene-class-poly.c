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

    # include "eratosthene-class-poly.h"

/*
    source - constructor/destructor methods
 */

    le_poly_t le_poly_create( le_void_t ) {

        /* created structure variable */
        le_poly_t le_pclass = LE_POLY_C;

        /* initialise offsets */
        memset( le_pclass.pc_data + LE_POLY_HEAD, 0xff, LE_POLY_OFFSET );

        /* return created structure */
        return( le_pclass );

    }

    le_void_t le_poly_reset( le_poly_t * const le_pclass ) {

        /* reset class size */
        memset( le_pclass->pc_data, 0x00, sizeof( le_pidx_t ) );

        /* reset class offsets */
        memset( le_pclass->pc_data + LE_POLY_HEAD, 0xff, LE_POLY_OFFSET );

    }

    le_void_t le_poly_delete( le_poly_t * const le_pclass ) {

        /* deleted structure variable */
        le_poly_t le_delete = LE_POLY_C;

        /* check memory */
        if ( le_pclass->pc_size > 0 ) {

            /* release memory */
            free( le_pclass->pc_link );

        }

        /* delete structure */
        ( * le_pclass ) = le_delete;

    }

/*
    source - accessor methods
 */

    le_size_t le_poly_get_size( le_poly_t const * const le_pclass ) {

        /* size variable */
        le_pidx_t * le_size = ( le_pidx_t * ) le_pclass->pc_data;

        /* return class size */
        return( ( le_size_t ) ( * le_size ) );

    }

    le_size_t le_poly_get_link( le_poly_t const * const le_pclass, le_size_t const le_index ) {

        /* return class link */
        return( ( le_pclass->pc_link )[le_index] );

    }

    le_size_t le_poly_get_offset( le_poly_t const * const le_pclass, le_size_t const le_index ) {

        /* extract and return offset */
        return( ( * le_poly_mac_offset( le_pclass, le_index ) ) & _LE_OFFS_NULL );

    }

/*
    source - mutator methods
 */

    le_enum_t le_poly_set_memory( le_poly_t * const le_pclass, le_size_t const le_size ) {

        /* swap variable */
        le_void_t * le_swap = NULL;

        /* check memory */
        if ( le_size > le_pclass->pc_size ) {

            /* re-allocate memory */
            if ( ( le_swap = realloc( le_pclass->pc_link, le_size * sizeof( le_size_t ) ) ) == NULL ) {

                /* send message */
                return( LE_ERROR_MEMORY );

            }

            /* update size */
            le_pclass->pc_size = le_size;

            /* validate memory */
            le_pclass->pc_link = ( le_size_t * ) le_swap;

        }

        /* send message */
        return( LE_ERROR_SUCCESS );

    }

    le_void_t le_poly_set_offset( le_poly_t * const le_pclass, le_size_t const le_index, le_size_t const le_offset ) {

        /* class pointer variable */
        le_size_t * le_base = le_poly_mac_offset( le_pclass, le_index );

        /* assign offset */
        ( * le_base ) = ( ( * le_base ) & ( ~ _LE_OFFS_NULL ) ) | ( le_offset & _LE_OFFS_NULL );

    }

    le_enum_t le_poly_set_push( le_poly_t * const le_pclass, le_size_t const le_link ) {

        /* size variable */
        le_pidx_t * le_size = ( le_pidx_t * ) le_pclass->pc_data;

        /* check class limit */
        if ( ( * le_size ) == LE_POLY_LIMIT ) {

            /* send message */
            return( LE_ERROR_OVERFLOW );

        }

        /* check class memory */
        if ( le_pclass->pc_size == ( * le_size ) ) {

            /* update class memory */
            if ( le_poly_set_memory( le_pclass, ( * le_size ) + LE_POLY_STEP ) != LE_ERROR_SUCCESS ) {

                /* send message */
                return( LE_ERROR_MEMORY );

            }

        }

        /* push link and update class */
        le_pclass->pc_link[ ( * le_size ) ++ ] = le_link;

        /* send message */
        return( LE_ERROR_SUCCESS );

    }

/*
    source - i/o methods
 */

    le_enum_t le_poly_io_read( le_poly_t * const le_pclass, le_size_t const le_offset, le_file_t const le_stream ) {

        /* size variable */
        le_pidx_t * le_size = ( le_pidx_t * ) le_pclass->pc_data;

        /* stream offset */
        fseek( le_stream, le_offset, SEEK_SET );

        /* class importation */
        if ( fread( le_pclass->pc_data, sizeof( le_byte_t ), LE_POLY_FIXED, le_stream ) != LE_POLY_FIXED ) {

            /* send message */
            return( LE_ERROR_IO_READ );

        } else {

            /* memory management */
            if ( le_poly_set_memory( le_pclass, ( * le_size ) ) != LE_ERROR_SUCCESS ) {

                /* send message */
                return( LE_ERROR_MEMORY );

            } else {

                /* class importation */
                if ( fread( le_pclass->pc_link, sizeof( le_size_t ), ( * le_size ), le_stream ) != ( * le_size ) ) {

                    /* send message */
                    return( LE_ERROR_IO_READ );

                } else {

                    /* send message */
                    return( LE_ERROR_SUCCESS );

                }

            }

        }

    }

    le_enum_t le_poly_io_read_fast( le_poly_t * const le_pclass, le_size_t const le_offset, le_file_t const le_stream ) {

        /* stream offset */
        fseek( le_stream, le_offset, SEEK_SET );

        /* class importation */
        if ( fread( le_pclass->pc_data, sizeof( le_byte_t ), LE_POLY_FIXED, le_stream ) != LE_POLY_FIXED ) {

            /* send message */
            return( LE_ERROR_IO_READ );

        } else {

            /* send message */
            return( LE_ERROR_SUCCESS );

        }

    }

    le_enum_t le_poly_io_read_next( le_poly_t * const le_pclass, le_file_t const le_stream ) {

        /* size variable */
        le_pidx_t * le_size = ( le_pidx_t * ) le_pclass->pc_data;

        /* memory management */
        if ( le_poly_set_memory( le_pclass, ( * le_size ) ) != LE_ERROR_SUCCESS ) {

            /* send message */
            return( LE_ERROR_MEMORY );

        } else {

            /* class importation */
            if ( fread( le_pclass->pc_link, sizeof( le_size_t ), ( * le_size ), le_stream ) != ( * le_size ) ) {

                /* send message */
                return( LE_ERROR_IO_READ );

            } else {

                /* send message */
                return( LE_ERROR_SUCCESS );

            }

        }

    }

    le_enum_t le_poly_io_write( le_poly_t const * const le_pclass, le_size_t const le_offset, le_file_t const le_stream ) {

        /* size variable */
        le_pidx_t * le_size = ( le_pidx_t * ) le_pclass->pc_data;

        /* check offset */
        if ( le_offset != _LE_OFFS_NULL ) {

            /* stream offset */
            fseek( le_stream, le_offset, SEEK_SET );

        }

        /* class exportation */
        if ( fwrite( le_pclass->pc_data, sizeof( le_byte_t ), LE_POLY_FIXED, le_stream ) != LE_POLY_FIXED ) {

            /* send message */
            return( LE_ERROR_IO_WRITE );

        } else {

            /* class exportation */
            if ( fwrite( le_pclass->pc_link, sizeof( le_size_t ), ( * le_size ), le_stream ) != ( * le_size ) ) {

                /* send message */
                return( LE_ERROR_IO_WRITE );

            } else {

                /* send message */
                return( LE_ERROR_SUCCESS );

            }

        }

    }

    le_size_t le_poly_io_offset( le_size_t const le_offset, le_size_t const le_index, le_file_t const le_stream ) {

        /* returned value variable */
        le_size_t le_return = _LE_OFFS_NULL;

        /* stream position */
        fseek( le_stream, le_offset + LE_POLY_HEAD + ( _LE_USE_OFFSET * le_index ), SEEK_SET );

        /* import offset */
        if ( fread( ( le_void_t * ) & le_return, sizeof( le_byte_t ), _LE_USE_OFFSET, le_stream ) != _LE_USE_OFFSET ) {

            /* send message */
            return( _LE_OFFS_NULL );

        } else {

            /* return offset */
            return( le_return );

        }

    }

