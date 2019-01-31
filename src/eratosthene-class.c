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

    # include "eratosthene-class.h"

/*
    source - private static variables
 */

    /* class size array */
    static const le_byte_t le_class_size[LE_CLASS_COUNT] = LE_CLASS_SIZE;

    /* direct access array */
    static const le_byte_t le_class_direct[LE_CLASS_COUNT][_LE_USE_BASE] = LE_CLASS_DIRECT;

    /* invert access array */
    static const le_byte_t le_class_invert[LE_CLASS_COUNT][_LE_USE_BASE] = LE_CLASS_INVERT;

/*
    source - constructor/destructor methods
 */

    le_void_t le_class_create( le_byte_t * const le_class ) {

        /* initialise class descriptor */
        ( * le_class ) = 0x00;

    }

/*
    source - accessor methods
 */

    le_size_t le_class_get_count( le_byte_t const * const le_class ) { // necessity to check //

        /* return class size */
        return( le_class_size[ ( * le_class ) ] );

    }

    le_size_t le_class_get_offset( le_byte_t const * le_class, le_size_t const le_index ) {

        /* check offset availability */
        if ( le_class_direct[ ( * le_class ) ][le_index] == 0xff ) {

            /* return null offset */
            return( _LE_OFFS_NULL );

        } else {

            /* move pointer to offset position */
            le_class += le_class_direct[ ( * le_class ) ][le_index] * _LE_USE_OFFSET + sizeof( le_byte_t );

            /* extract and return offset */
            return( le_class_mac_cast( le_class ) & _LE_OFFS_NULL );

        }

    }

/*
    source - mutator methods
 */

    le_void_t le_class_set_offset( le_byte_t * le_class, le_size_t const le_index, le_size_t const le_offset ) {

        /* detection mask variable */
        le_byte_t le_pattern = ( 1 << le_index );

        /* swapping variable */
        le_byte_t le_move = 0;

        /* swapping variable */
        le_byte_t * le_base = le_class + sizeof( le_byte_t );

        /* offset presence detection */
        if ( ( ( * le_class ) & le_pattern ) == 0 ) {

            /* check for initialy empty class */
            if ( ( * le_class ) != 0 ) {

                /* distinguish append from insertion */
                if ( ( * le_class ) > le_pattern ) {

                    /* push class descriptor with addition of offset marker */
                    le_pattern |= ( * le_class );

                    /* parsing previous class offsets */
                    for ( le_size_t le_parse = 0; le_parse < le_class_size[ * le_class ]; le_parse ++ ) {

                        /* compute and check offset swapping */
                        if ( ( le_move = le_class_direct[ le_pattern ][ le_class_invert[ * le_class ][ le_parse ] ] ) != le_parse ) {

                            /* move offset */
                            memcpy( le_base + ( le_parse * _LE_USE_OFFSET ), le_base + ( le_move * _LE_USE_OFFSET ), _LE_USE_OFFSET );

                        }

                    }

                    /* assign pushed class descriptor */
                    ( * le_class ) = le_pattern;

                } else {

                    /* add offset marker */
                    ( * le_class ) |= le_pattern;

                }

            } else {

                /* add offset marker */
                ( * le_class ) |= le_pattern;

            }

        }

        /* move pointer to offset position */
        le_class += ( le_class_direct[ ( * le_class ) ][le_index] ) * _LE_USE_OFFSET + sizeof( le_byte_t );

        /* assign offset value */
        le_class_mac_cast( le_class ) = ( le_class_mac_cast( le_class ) & ( ~ _LE_OFFS_NULL ) ) | ( le_offset & _LE_OFFS_NULL );

    }

/*
    source - i/o methods
 */

    /* assumes class header has been read by calling function */

    le_enum_t le_class_io_read( le_byte_t * le_class, le_file_t const le_stream ) {

        /* class size variable */
        le_size_t le_read = le_class_size[ ( * ( le_class ++ ) ) ] * _LE_USE_OFFSET;

        /* check empty class */
        if ( le_read == 0 ) {

            /* send message */
            return( LE_ERROR_SUCCESS );

        } else {

            /* read class offsets */
            if ( fread( le_class, sizeof( le_byte_t ), le_read, le_stream ) != le_read ) {

                /* send message */
                return( LE_ERROR_IO_READ );

            } else {

                /* send message */
                return( LE_ERROR_SUCCESS );

            }

        }

    }

    le_enum_t le_class_io_write( le_byte_t * le_offset, le_file_t const le_stream ) {

        /* */
        static le_byte_t le_size[LE_CLASS_COUNT] = LE_CLASS_SIZE;

        /* */
        le_size_t le_write = le_size[ ( * ( le_offset ++ ) ) ] * _LE_USE_OFFSET;

        /* */
        if ( le_write == 0 ) {

            /* */
            return( LE_ERROR_SUCCESS );

        }

        /* */
        if ( fwrite( le_offset, sizeof( le_byte_t ), le_write, le_stream ) != le_write ) {

            /* */
            return( LE_ERROR_IO_WRITE );

        } else {

            /* */
            return( LE_ERROR_SUCCESS );

        }

    }

    le_size_t le_class_io_offset( le_size_t const le_index, le_file_t const le_stream ) {

        /* */
        static le_byte_t le_direct[LE_CLASS_COUNT][_LE_USE_BASE] = LE_CLASS_DIRECT;

        /* */
        le_byte_t le_pattern = 0x00;

        /* */
        le_size_t le_return;

        /* */
        if ( fread( & le_pattern, sizeof( le_byte_t ), 1, le_stream ) != 1 ) {

            /* */
            return( _LE_OFFS_NULL );

        } else {

            /* */
            if ( le_direct[ le_pattern ][ le_index ] == 0xff ) {

                /* */
                return( _LE_OFFS_NULL );

            } else {

                /* */
                fseek( le_stream, le_direct[le_pattern][le_index] * _LE_USE_OFFSET, SEEK_CUR );

                /* */
                if ( fread( & le_return, sizeof( le_byte_t ), _LE_USE_OFFSET, le_stream ) != _LE_USE_OFFSET ) {

                    /* */
                    return( _LE_OFFS_NULL );

                } else {

                    /* */
                    return( le_return );

                }

            }

        }

    }

