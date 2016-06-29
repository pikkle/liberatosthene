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

    # include "eratosthene-class.h"

/*
    source - constructor/destructor methods
 */

    le_class_t le_class_create( le_data_t const * const le_data ) {

        /* Returned structure variables */
        le_class_t le_class = LE_CLASS_C_DATA( le_data[0], le_data[1], le_data[2] );

        /* Return constructed structure */
        return( le_class );

    }

    le_class_t le_class_delete( le_void_t ) {

        /* Returned structure variables */
        le_class_t le_class = LE_CLASS_C;

        /* Return deleted structure */
        return( le_class );

    }

/*
    source - accessor methods
 */

    le_size_t le_class_get_offset( le_class_t const * const le_class, le_size_t const le_addr ) {

        /* Check consistency */
        if ( le_addr >= _LE_USE_BASE ) {

            /* Return invalid offset */
            return( _LE_SIZE_NULL );

        } else {

            /* Return daughter offset */
            return( le_class->cs_addr[le_addr] );

        }

    }

    le_size_t * le_class_get_addr( le_class_t const * const le_class ) {

        /* Return class pointer */
        return( ( le_size_t * ) le_class->cs_addr );

    }

    le_data_t * le_class_get_data( le_class_t const * const le_class ) {

        /* Return class pointer */
        return( ( le_data_t * ) le_class->cs_data );

    }

/*
    source - mutator methods
 */

    le_enum_t le_class_set_offset( le_class_t * const le_class, le_size_t const le_addr, le_size_t const le_offset ) {

        /* Check consistency */
        if ( le_addr >= _LE_USE_BASE ) {

            /* Send message */
            return( LE_ERROR_BASE );

        } else {

            /* Update daughter offset */
            le_class->cs_addr[le_addr] = le_offset;

            /* Send message */
            return( LE_ERROR_SUCCESS );

        }

    }

    le_void_t le_class_set_push( le_class_t * const le_class, le_data_t const * const le_data ) {

        /* Corrected heap variables */
        le_real_t le_heap = le_class->cs_heap + 1;

        /* Class data injection */
        le_class->cs_data[0] = ( ( le_heap * ( le_real_t ) le_class->cs_data[0] ) + ( le_real_t ) le_data[0] ) / ( le_heap + 1 );
        le_class->cs_data[1] = ( ( le_heap * ( le_real_t ) le_class->cs_data[1] ) + ( le_real_t ) le_data[1] ) / ( le_heap + 1 );
        le_class->cs_data[2] = ( ( le_heap * ( le_real_t ) le_class->cs_data[2] ) + ( le_real_t ) le_data[2] ) / ( le_heap + 1 );

        /* Check heap value */
        if ( le_class->cs_heap < _LE_BYTE_MAX ) le_class->cs_heap ++;

    }

/*
    source - i/o methods
 */

    le_enum_t le_class_io_read2( le_class_t * const le_class, le_size_t const le_offset, FILE * const le_stream, le_size_t const le_format ) {

        /* Buffer size variables */
        le_size_t le_size = le_format * 8 + 4;

        /* Class i/o buffer variables */
        le_byte_t le_buffer[LE_CLASS_BUFFER_SIZE] = LE_CLASS_BUFFER_C;

        /* Check stream */
        if ( ( le_stream->_flags & _IO_NO_READS ) != 0 ) {

            /* Send message */
            return( LE_ERROR_IO_STREAM );

        }

        /* Move head to class offset */
        if ( fseek( le_stream, le_offset, SEEK_SET ) != 0 ) {

            /* Send message */
            return( LE_ERROR_IO_SEEK );

        }

        /* Read class buffer */
        if ( fread( ( le_void_t * ) le_buffer, 1, le_size, le_stream ) != le_size ) {

            /* Send message */
            return( LE_ERROR_IO_WRITE );

        }

        /* Class buffer - heap segment */
        le_class->cs_heap = * ( le_buffer );

        /* Class buffer - data segment */
        * ( le_class->cs_data     ) = * ( le_buffer + 1 );
        * ( le_class->cs_data + 1 ) = * ( le_buffer + 2 );
        * ( le_class->cs_data + 2 ) = * ( le_buffer + 3 );

        /* Class buffer - offset segment */
        * ( le_class->cs_addr     ) = * ( ( le_size_t * ) ( le_buffer + 4                 ) );
        * ( le_class->cs_addr + 1 ) = * ( ( le_size_t * ) ( le_buffer + 4 + le_format     ) );
        * ( le_class->cs_addr + 2 ) = * ( ( le_size_t * ) ( le_buffer + 4 + le_format * 2 ) );
        * ( le_class->cs_addr + 3 ) = * ( ( le_size_t * ) ( le_buffer + 4 + le_format * 3 ) );
        * ( le_class->cs_addr + 4 ) = * ( ( le_size_t * ) ( le_buffer + 4 + le_format * 4 ) );
        * ( le_class->cs_addr + 5 ) = * ( ( le_size_t * ) ( le_buffer + 4 + le_format * 5 ) );
        * ( le_class->cs_addr + 6 ) = * ( ( le_size_t * ) ( le_buffer + 4 + le_format * 6 ) );
        * ( le_class->cs_addr + 7 ) = * ( ( le_size_t * ) ( le_buffer + 4 + le_format * 7 ) );

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

    le_enum_t le_class_io_read( le_class_t * const le_class, le_size_t const le_offset, FILE * const le_stream ) {

        /* Check stream */
        if ( ( le_stream->_flags & _IO_NO_READS ) != 0 ) {

            /* Send message */
            return( LE_ERROR_IO_STREAM );

        }

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
        if ( fread( ( le_void_t * ) le_class->cs_data, sizeof( le_data_t ), 3, le_stream ) != 3 ) {

            /* Send message */
            return( LE_ERROR_IO_READ );

        }

        /* Writing class accumulator */
        if ( fread( ( le_void_t * ) ( & le_class->cs_heap ), sizeof( le_byte_t ), 1, le_stream ) != 1 ) {

            /* Send message */
            return( LE_ERROR_IO_READ );

        } else {

            /* Send message */
            return( LE_ERROR_SUCCESS );

        }

    }

    le_enum_t le_class_io_write2( le_class_t const * const le_class, le_size_t const le_offset, FILE * const le_stream, le_size_t const le_format ) {

        /* Buffer size variables */
        le_size_t le_size = le_format * 8 + 4;

        /* Class i/o buffer variables */
        le_byte_t le_buffer[LE_CLASS_BUFFER_SIZE] = LE_CLASS_BUFFER_C;

        /* Check stream */
        if ( ( le_stream->_flags & _IO_NO_WRITES ) != 0 ) {

            /* Send message */
            return( LE_ERROR_IO_STREAM );

        }

        /* Move head to class offset */
        if ( fseek( le_stream, le_offset, SEEK_SET ) != 0 ) {

            /* Send message */
            return( LE_ERROR_IO_SEEK );

        }

        /* Class buffer - heap segment */
        * ( le_buffer ) = le_class->cs_heap;

        /* Class buffer - data segment */
        * ( le_buffer + 1 ) = * ( le_class->cs_data     );
        * ( le_buffer + 2 ) = * ( le_class->cs_data + 1 );
        * ( le_buffer + 3 ) = * ( le_class->cs_data + 2 );

        /* Class buffer - offset segment */
        * ( ( le_size_t * ) ( le_buffer + 4                 ) ) = * ( le_class->cs_addr     );
        * ( ( le_size_t * ) ( le_buffer + 4 + le_format     ) ) = * ( le_class->cs_addr + 1 );
        * ( ( le_size_t * ) ( le_buffer + 4 + le_format * 2 ) ) = * ( le_class->cs_addr + 2 );
        * ( ( le_size_t * ) ( le_buffer + 4 + le_format * 3 ) ) = * ( le_class->cs_addr + 3 );
        * ( ( le_size_t * ) ( le_buffer + 4 + le_format * 4 ) ) = * ( le_class->cs_addr + 4 );
        * ( ( le_size_t * ) ( le_buffer + 4 + le_format * 5 ) ) = * ( le_class->cs_addr + 5 );
        * ( ( le_size_t * ) ( le_buffer + 4 + le_format * 6 ) ) = * ( le_class->cs_addr + 6 );
        * ( ( le_size_t * ) ( le_buffer + 4 + le_format * 7 ) ) = * ( le_class->cs_addr + 7 );

        /* Write class buffer */
        if ( fwrite( ( le_void_t * ) le_buffer, 1, le_size, le_stream ) != le_size ) {

            /* Send message */
            return( LE_ERROR_IO_WRITE );

        }

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }


    le_enum_t le_class_io_write( le_class_t const * const le_class, le_size_t const le_offset, FILE * const le_stream ) {

        /* Check stream */
        if ( ( le_stream->_flags & _IO_NO_WRITES ) != 0 ) {

            /* Send message */
            return( LE_ERROR_IO_STREAM );

        }

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
        if ( fwrite( ( le_void_t * ) le_class->cs_data, sizeof( le_data_t ), 3, le_stream ) != 3 ) {

            /* Send message */
            return( LE_ERROR_IO_WRITE );

        }

        /* Writing class accumulator */
        if ( fwrite( ( le_void_t * ) ( & le_class->cs_heap ), sizeof( le_byte_t ), 1, le_stream ) != 1 ) {

            /* Send message */
            return( LE_ERROR_IO_WRITE );

        } else {

            /* Send message */
            return( LE_ERROR_SUCCESS );

        }

    }

