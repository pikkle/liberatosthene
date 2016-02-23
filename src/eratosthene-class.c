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

    # include "eratosthene-class.h"

/*
    source - accessor methods
 */

    le_byte_t le_class_get_digit( le_class_t const * const le_class ) {

        /* Returns class digit */
        return( le_class->cs_node & 0x0f );

    }

    le_byte_t le_class_get_dcc( le_class_t const * const le_class ) {

        /* Return class daughter class count */
        return( le_class->cs_node >> 0x04 );

    }

    le_mean_t le_class_get_mean( le_class_t const * const le_class ) {

        /* Returns class accumulator */
        return( le_class->cs_mean );

    }

/*
    source - mutator methods
 */

    le_void_t le_class_set_clear( le_class_t * const le_class, le_byte_t const le_digit ) {

        /* Assign class digit */
        le_class->cs_node = ( le_digit & 0x0f );

        /* Clear colorimetry */
        le_class->cs_data[0] = 0.0;
        le_class->cs_data[1] = 0.0;
        le_class->cs_data[2] = 0.0;

        /* Clear class accumulator */
        le_class->cs_mean = 0;

    }

    le_void_t le_class_set_digit( le_class_t * const le_class, le_byte_t const le_digit ) {

        /* Assign class digit */
        le_class->cs_node = ( le_class->cs_node & 0xf0 ) | ( le_digit & 0x0f );

    }

    le_void_t le_class_set_dcc( le_class_t * const le_class, le_byte_t const le_dcc ) {

        /* Assign class daughter class count */
        le_class->cs_node = ( le_class->cs_node & 0x0f ) | ( ( le_dcc & 0x0f ) << 0x04 );

    }

    le_void_t le_class_set_node( le_class_t * const le_class, le_byte_t const le_digit, le_byte_t const le_dcc ) {

        /* Assign class digit and daughter class count */
        le_class->cs_node = ( le_digit & 0x0f ) | ( ( le_dcc & 0x0f ) << 0x04 );

    }

    le_void_t le_class_set_inject( le_class_t * const le_class, le_real_t const le_red, le_real_t const le_green, le_real_t const le_blue ) {

        /* Inject colorimetric information */
        le_class->cs_data[0] = ( ( le_class->cs_data[0] * le_class->cs_mean ) + le_red   ) / ( le_class->cs_mean + 1 );
        le_class->cs_data[1] = ( ( le_class->cs_data[1] * le_class->cs_mean ) + le_green ) / ( le_class->cs_mean + 1 );
        le_class->cs_data[2] = ( ( le_class->cs_data[2] * le_class->cs_mean ) + le_blue  ) / ( le_class->cs_mean + 1 );

        /* Update class accumulator */
        le_class->cs_mean ++;

    }

/*
    source - i/o methods
 */

    le_enum_t le_class_io_read( le_class_t * const le_class, FILE * const le_stream ) {

        /* Check stream */
        # if ( _LE_USE_STDIO_EXT == _LE_TRUE )
        if ( __freadable( le_stream ) == 0 ) {

            /* Send message */
            return( LE_ERROR_IO_STREAM );

        }
        # endif

        /* Reading class node */
        if ( fread( ( le_void_t * ) ( & ( le_class->cs_node ) ), sizeof( le_byte_t ), 1, le_stream ) != 1 ) {

            /* Send message */
            return( LE_ERROR_IO_READ );

        }

        /* Writing class addresses */
        if ( fread( ( le_void_t * ) le_class->cs_addr, sizeof( le_size_t ), 8, le_stream ) != 8 ) {

            /* Send message */
            return( LE_ERROR_IO_READ );

        }

        /* Writing class colorimetry */
        if ( fread( ( le_void_t * ) le_class->cs_data, sizeof( le_real_t ), 3, le_stream ) != 3 ) {

            /* Send message */
            return( LE_ERROR_IO_READ );

        }

        /* Writing class accumulator */
        if ( fread( ( le_void_t * ) ( & le_class->cs_mean ), sizeof( le_mean_t ), 1, le_stream ) != 1 ) {

            /* Send message */
            return( LE_ERROR_IO_READ );

        } else {

            /* Send message */
            return( LE_ERROR_SUCCESS );

        }

    }

    le_enum_t le_class_io_write( le_class_t const * const le_class, FILE * const le_stream ) {

        /* Check stream */
        # if ( _LE_USE_STDIO_EXT == _LE_TRUE )
        if ( __fwritable( le_stream ) == 0 ) {

            /* Send message */
            return( LE_ERROR_IO_STREAM );

        }
        # endif

        /* Writing class node */
        if ( fwrite( ( le_void_t * ) ( & ( le_class->cs_node ) ), sizeof( le_byte_t ), 1, le_stream ) != 1 ) {

            /* Send message */
            return( LE_ERROR_IO_WRITE );

        }

        /* Writing class addresses */
        if ( fwrite( ( le_void_t * ) le_class->cs_addr, sizeof( le_size_t ), 8, le_stream ) != 8 ) {

            /* Send message */
            return( LE_ERROR_IO_WRITE );

        }

        /* Writing class colorimetry */
        if ( fwrite( ( le_void_t * ) le_class->cs_data, sizeof( le_real_t ), 3, le_stream ) != 3 ) {

            /* Send message */
            return( LE_ERROR_IO_WRITE );

        }

        /* Writing class accumulator */
        if ( fwrite( ( le_void_t * ) ( & le_class->cs_mean ), sizeof( le_mean_t ), 1, le_stream ) != 1 ) {

            /* Send message */
            return( LE_ERROR_IO_WRITE );

        } else {

            /* Send message */
            return( LE_ERROR_SUCCESS );

        }

    }

