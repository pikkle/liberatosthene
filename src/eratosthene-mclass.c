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

    # include "eratosthene-mclass.h"

/*
    source - constructor/destructor methods
 */

    le_mclass_t le_mclass_create( le_void_t ) {

        /* created structure variable */
        le_mclass_t le_mclass = LE_MCLASS_C;

        /* initialise offsets */
        memset( le_mclass.mc_data + LE_MCLASS_HEAD, 0xff, LE_MCLASS_OFFSET );

        /* return created structure */
        return( le_mclass );

    }

    le_void_t le_mclass_reset( le_mclass_t * const le_mclass ) {

        /* reset offsets */
        memset( le_mclass->mc_data + LE_MCLASS_HEAD, 0xff, LE_MCLASS_OFFSET );

        /* reset counter */
        le_mclass->mc_size = 0;

        /* reset accumumlation */
        le_mclass->mc_push[0] = 0.0;
        le_mclass->mc_push[1] = 0.0;
        le_mclass->mc_push[2] = 0.0;

    }

    le_void_t le_mclass_delete( le_mclass_t * const le_mclass ) {

        /* deleted structure variable */
        le_mclass_t le_delete = LE_MCLASS_C;

        /* delete structure */
        ( * le_mclass ) = le_delete;

    }

/*
    source - accessor methods
 */

    le_size_t le_mclass_get_offset( le_mclass_t const * const le_mclass, le_size_t const le_index ) {

        /* extract and return offset */
        return( ( * le_mclass_mac_offset( le_mclass, le_index ) ) & _LE_OFFS_NULL );

    }

/*
    source - mutator methods
 */

    le_void_t le_mclass_set_offset( le_mclass_t * const le_mclass, le_size_t const le_index, le_size_t const le_offset ) {

        /* class pointer variable */
        le_size_t * le_base = le_mclass_mac_offset( le_mclass, le_index );

        /* assign offset */
        ( * le_base ) = ( ( * le_base ) & ( ~ _LE_OFFS_NULL ) ) | ( le_offset & _LE_OFFS_NULL );

    }

/*
    source - i/o methods
 */

    le_enum_t le_mclass_read( le_mclass_t * const le_mclass, le_size_t const le_offset, le_file_t const le_stream ) {

        /* stream offset */
        fseek( le_stream, le_offset, SEEK_SET );

        /* class importation */
        if ( fread( le_mclass->mc_data, sizeof( le_byte_t ), LE_MCLASS_FIXED, le_stream ) != LE_MCLASS_FIXED ) {

            /* send message */
            return( LE_ERROR_IO_READ );

        } else {

            /* send message */
            return( LE_ERROR_SUCCESS );

        }

    }

    le_enum_t le_mclass_read_fast( le_mclass_t * const le_mclass, le_size_t const le_offset, le_file_t const le_stream ) {

        /* stream offset */
        fseek( le_stream, le_offset + LE_MCLASS_HEAD, SEEK_SET );

        /* class importation */
        if ( fread( le_mclass->mc_data, sizeof( le_byte_t ), LE_MCLASS_OFFSET, le_stream ) != LE_MCLASS_OFFSET ) {

            /* send message */
            return( LE_ERROR_IO_READ );

        } else {

            /* send message */
            return( LE_ERROR_SUCCESS );

        }

    }

    le_enum_t le_mclass_write( le_mclass_t * const le_mclass, le_size_t const le_offset, le_file_t const le_stream ) {

        /* check offset */
        if ( le_offset != _LE_OFFS_NULL ) {

            /* stream offset */
            fseek( le_stream, le_offset, SEEK_SET );

        }

        /* compose class data */
        le_mclass->mc_data[0] = le_mclass->mc_push[0] / le_mclass->mc_size;
        le_mclass->mc_data[1] = le_mclass->mc_push[1] / le_mclass->mc_size;
        le_mclass->mc_data[2] = le_mclass->mc_push[2] / le_mclass->mc_size;

        /* class exportation */
        if ( fwrite( le_mclass->mc_data, sizeof( le_byte_t ), LE_MCLASS_FIXED, le_stream ) != LE_MCLASS_FIXED ) {

            /* send message */
            return( LE_ERROR_IO_WRITE );

        } else {

            /* send message */
            return( LE_ERROR_SUCCESS );

        }

    }

