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

    le_size_t le_class_get_addr( le_class_t const * const le_class, le_size_t const le_addr ) {

        /* Return daughter class offset */
        return( le_class->cs_addr[ ( le_addr & 0x7 ) ] );

    }

    le_mean_t le_class_get_mean( le_class_t const * const le_class ) {

        /* Returns class accumulator */
        return( le_class->cs_mean );

    }

/*
    source - mutator methods
 */

    le_void_t le_class_set_clear( le_class_t * const le_class, le_byte_t const le_digit ) {

        /* Clear address */
        le_class->cs_addr[0] = LE_CLASS_NULL;
        le_class->cs_addr[1] = LE_CLASS_NULL;
        le_class->cs_addr[2] = LE_CLASS_NULL;
        le_class->cs_addr[3] = LE_CLASS_NULL;
        le_class->cs_addr[4] = LE_CLASS_NULL;
        le_class->cs_addr[5] = LE_CLASS_NULL;
        le_class->cs_addr[6] = LE_CLASS_NULL;
        le_class->cs_addr[7] = LE_CLASS_NULL;

        /* Clear colorimetry */
        le_class->cs_data[0] = 0.0;
        le_class->cs_data[1] = 0.0;
        le_class->cs_data[2] = 0.0;

        /* Clear class accumulator */
        le_class->cs_mean = 0;

    }

    le_void_t le_class_set_addr( le_class_t * const le_class, le_size_t const le_addr, le_size_t const le_offset ) {

        /* Update daughter class offset */
        le_class->cs_addr[ ( le_addr & 0x7 ) ] = le_offset;

    }

    le_void_t le_class_set_inject( le_class_t * const le_class, le_real_t const * const le_data ) {

        /* Inject colorimetric information */
        le_class->cs_data[0] = ( ( le_class->cs_data[0] * le_class->cs_mean ) + le_data[0] ) / ( le_class->cs_mean + 1 );
        le_class->cs_data[1] = ( ( le_class->cs_data[1] * le_class->cs_mean ) + le_data[1] ) / ( le_class->cs_mean + 1 );
        le_class->cs_data[2] = ( ( le_class->cs_data[2] * le_class->cs_mean ) + le_data[2] ) / ( le_class->cs_mean + 1 );

        /* Update class accumulator */
        le_class->cs_mean ++;

    }

/*
    source - i/o methods
 */

    le_enum_t le_class_io_read( le_class_t * const le_class, le_size_t const le_offset, FILE * const le_stream ) {

        /* Check stream */
        # if ( _LE_USE_STDIO_EXT == _LE_TRUE )
        if ( __freadable( le_stream ) == 0 ) {

            /* Send message */
            return( LE_ERROR_IO_STREAM );

        }
        # endif

        /* Move head to class offset */
        if ( fseek( le_stream, le_offset, SEEK_SET ) != 0 ) {

            /* Send message */
            return( LE_ERROR_IO_SEEK );

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

    le_enum_t le_class_io_write( le_class_t const * const le_class, le_size_t const le_offset, FILE * const le_stream ) {

        /* Check stream */
        # if ( _LE_USE_STDIO_EXT == _LE_TRUE )
        if ( __fwritable( le_stream ) == 0 ) {

            /* Send message */
            return( LE_ERROR_IO_STREAM );

        }
        # endif

        /* Move head to class offset */
        if ( fseek( le_stream, le_offset, SEEK_SET ) != 0 ) {

            /* Send message */
            return( LE_ERROR_IO_SEEK );

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

