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

    # include "eratosthene-offset.h"

/*
    source - constructor/destructor methods
 */

    le_void_t le_offset_create( le_byte_t * const le_offset ) {

        /* initialise pattern */
        ( * le_offset ) = 0x00;

    }

/*
    source - accessor methods
 */

    le_size_t le_offset_get_count( le_byte_t const * const le_offset ) { /* un-used */

        /* */
        static le_byte_t le_size[LE_OFFSET_COUNT] = LE_OFFSET_SIZE;

        /* */
        return( le_size[ ( * le_offset ) ] );

    }

    le_size_t le_offset_get_offset( le_byte_t const * le_offset, le_size_t const le_index ) {

        /* */
        static le_byte_t le_direct[LE_OFFSET_COUNT][_LE_USE_BASE] = LE_OFFSET_DIRECT;

        /* */
        if ( le_direct[ ( * le_offset ) ][le_index] != 0xff ) {

            /* */
            le_offset += ( le_direct[ ( * le_offset ) ][le_index] ) * _LE_USE_OFFSET + sizeof( le_byte_t );

            /* */
            return( le_offset_mac_cast( le_offset ) & _LE_OFFS_NULL );

        } else {

            /* */
            return( _LE_OFFS_NULL );

        }

    }

/*
    source - mutator methods
 */

    le_void_t le_offset_set_offset( le_byte_t * le_offset, le_size_t const le_index, le_size_t const le_value ) {

        /* */
        static le_byte_t le_direct[LE_OFFSET_COUNT][_LE_USE_BASE] = LE_OFFSET_DIRECT;

        /* */
        static le_byte_t le_invert[LE_OFFSET_COUNT][_LE_USE_BASE] = LE_OFFSET_INVERT;

        /* */
        le_byte_t le_pattern = ( 1 << le_index );

        /* */
        if ( ( ( * le_offset ) & le_pattern ) == 0 ) {

            /* */
            if ( ( * le_offset ) != 0 ) {

                /* */
                if ( ( * le_offset ) > le_pattern ) {

                    /* */
                    le_pattern |= ( * le_offset );

                    /* */ // Instance fault //
                    le_byte_t * le_base = le_invert[ ( * le_offset ) ];

                    /* */
                    for ( le_size_t le_parse = le_offset_get_count( le_offset ); le_base[le_parse] > le_index; le_parse -- ) {

                        /* */
                        memcpy( le_offset + sizeof( le_byte_t ) + _LE_USE_OFFSET * le_direct[le_pattern][le_base[le_parse]], le_offset + sizeof( le_byte_t ) + _LE_USE_OFFSET * le_direct[ ( * le_offset ) ][le_parse], _LE_USE_OFFSET );

                    }

                    /* */
                    ( * le_offset ) = le_pattern;

                } else {

                    /* */
                    ( * le_offset ) |= le_pattern;

                }

            } else {

                /* */
                ( * le_offset ) |= le_pattern;

            }

        }

        /* */
        le_offset += ( le_direct[ ( * le_offset ) ][le_index] ) * _LE_USE_OFFSET + sizeof( le_byte_t );

        /* */
        le_offset_mac_cast( le_offset ) = ( le_offset_mac_cast( le_offset ) & ( ~ _LE_OFFS_NULL ) ) | ( le_value & _LE_OFFS_NULL );

    }

/*
    source - i/o methods
 */

    le_enum_t le_offset_io_read( le_byte_t * le_offset, le_file_t const le_stream ) {

        /* */
        static le_byte_t le_size[LE_OFFSET_COUNT] = LE_OFFSET_SIZE;

        /* */
        le_size_t le_read = le_size[ ( * ( le_offset ++ ) ) ] * _LE_USE_OFFSET;

        /* */
        if ( fread( le_offset, sizeof( le_byte_t ), le_read, le_stream ) != le_read ) {

            /* */
            return( LE_ERROR_IO_READ );

        } else {

            /* */
            return( LE_ERROR_SUCCESS );

        }

    }

    le_enum_t le_offset_io_write( le_byte_t * le_offset, le_file_t const le_stream ) {

        /* */
        static le_byte_t le_size[LE_OFFSET_COUNT] = LE_OFFSET_SIZE;

        /* */
        le_size_t le_write = le_size[ ( * ( le_offset ++ ) ) ] * _LE_USE_OFFSET;

        /* */
        if ( fwrite( le_offset, sizeof( le_byte_t ), le_write, le_stream ) != le_write ) {

            /* */
            return( LE_ERROR_IO_WRITE );

        } else {

            /* */
            return( LE_ERROR_SUCCESS );

        }

    }

    le_size_t le_offset_io_offset( le_size_t const le_index, le_file_t const le_stream ) {

        /* */
        static le_byte_t le_size[LE_OFFSET_COUNT] = LE_OFFSET_SIZE;

        /* */
        static le_byte_t le_direct[LE_OFFSET_COUNT][_LE_USE_BASE] = LE_OFFSET_DIRECT;

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
            if ( le_size[ le_pattern ] == 0xff ) {

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

