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

        /* Created structure variables */
        le_class_t le_class = LE_CLASS_C_DATA( le_data[0], le_data[1], le_data[2] );

        /* Return constructed structure */
        return( le_class );

    }

    le_void_t le_class_delete( le_class_t * const le_class ) {

        /* Deleted structure variables */
        le_class_t le_delete = LE_CLASS_C;

        /* Delete structure */
        * le_class = le_delete;

    }

/*
    source - accessor methods
 */

    inline le_size_t le_class_get_offset( le_class_t const * const le_class, le_size_t const le_addr ) {

        /* Offset memory pointer variables */
        register le_size_t * le_vptr = ( le_size_t * ) ( ( le_byte_t * ) le_class->cs_data + _LE_USE_DATA + _LE_USE_OFFSET * le_addr );

        /* Return offset */
        return( * le_vptr & _LE_OFFS_NULL );

    }

    inline le_data_t * le_class_get_data( le_class_t const * const le_class ) {

        /* Return class pointer */
        return( ( le_data_t * ) le_class->cs_data );

    }

/*
    source - mutator methods
 */

    inline le_void_t le_class_set_offset( le_class_t * const le_class, le_size_t const le_addr, le_size_t const le_offset ) {

        /* Offset memory pointer variables */
        register le_size_t * le_vptr = ( le_size_t * ) ( ( le_byte_t * ) le_class->cs_data + _LE_USE_DATA + _LE_USE_OFFSET * le_addr );

        /* Assign offset */
        ( * le_vptr ) &= ~ ( ( le_size_t ) _LE_OFFS_NULL ), ( * le_vptr ) |= le_offset;

    }

    inline le_void_t le_class_set_push( le_class_t * const le_class, le_data_t const * const le_data ) {

        /* Corrected heap variables */
        float le_heap = le_class->cs_data[_LE_USE_DATA - 1] + 1;

        /* Data pseudo-accumulation */
        le_class->cs_data[0] = ( ( le_heap * ( float ) le_class->cs_data[0] ) + ( float ) le_data[0] ) / ( le_heap + 1 );
        le_class->cs_data[1] = ( ( le_heap * ( float ) le_class->cs_data[1] ) + ( float ) le_data[1] ) / ( le_heap + 1 );
        le_class->cs_data[2] = ( ( le_heap * ( float ) le_class->cs_data[2] ) + ( float ) le_data[2] ) / ( le_heap + 1 );

        /* Check heap value */
        if ( le_class->cs_data[_LE_USE_DATA - 1] < _LE_BYTE_MAX ) le_class->cs_data[_LE_USE_DATA - 1] ++;

    }

/*
    source - i/o methods
 */

    inline le_enum_t le_class_io_read( le_class_t * const le_class, le_size_t const le_offset, FILE * const le_stream ) {

        /* Move head to class offset - send message */
        if ( fseek( le_stream, le_offset, SEEK_SET ) != 0 ) return( LE_ERROR_IO_SEEK );

        /* Read class buffer - send message */
        if ( fread( ( le_void_t * ) le_class->cs_data, 1, LE_CLASS_BUFFER, le_stream ) != LE_CLASS_BUFFER ) return( LE_ERROR_IO_WRITE );

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

    inline le_enum_t le_class_io_write( le_class_t const * const le_class, le_size_t const le_offset, FILE * const le_stream ) {

        /* Move head to class offset - send message */
        if ( fseek( le_stream, le_offset, SEEK_SET ) != 0 ) return( LE_ERROR_IO_SEEK );

        /* Write class buffer - send message */
        if ( fwrite( ( le_void_t * ) le_class->cs_data, 1, LE_CLASS_BUFFER, le_stream ) != LE_CLASS_BUFFER ) return( LE_ERROR_IO_WRITE );

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

