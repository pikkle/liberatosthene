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

    # include "eratosthene-pclass.h"

/*
    source - constructor/destructor methods
 */

    le_pclass_t le_pclass_create( le_void_t ) {

        /* create structure variable */
        le_pclass_t le_pclass = LE_PCLASS_C;

        /* initialise class offsets */
        memset( le_pclass.pc_data + LE_PCLASS_DATA, 0xff, LE_PCLASS_OFFSET );

        /* return created structure */
        return( le_pclass );

    }

    le_void_t le_pclass_delete( le_pclass_t * const le_pclass ) {

        /* deleted structure variable */
        le_pclass_t le_delete = LE_PCLASS_C;

        /* delete structure */
        ( * le_pclass ) = le_delete;

    }

/*
    source - accessor methods
 */

    le_size_t le_pclass_get_offset( le_pclass_t const * const le_pclass, le_size_t const le_index ) {

        /* extract and return offset */
        return( ( * le_pclass_mac_offset( le_pclass, le_index ) ) & _LE_OFFS_NULL );

    }

/*
    source - mutator methods
 */

    le_void_t le_pclass_set_offset( le_pclass_t * const le_pclass, le_size_t const le_index, le_size_t const le_offset ) {

        /* class pointer variable */
        le_size_t * le_base = le_pclass_mac_offset( le_pclass, le_index );

        /* assign offset */
        ( * le_base ) = ( ( * le_base ) & ( ~ _LE_OFFS_NULL ) ) | le_offset;

    }

    le_void_t le_pclass_set_push( le_pclass_t * const le_pclass, le_size_t const le_link ) {

        /* class pointer variable */
        le_size_t * le_base = ( le_size_t * ) ( le_pclass->pc_data + sizeof( le_byte_t ) );

        /* check class state */
        if ( le_pclass->pc_data[0] < LE_PCLASS_SIZE ) {

            /* push link */
            le_base[le_pclass->pc_data[0]] = le_link;

            /* update stack state */
            le_pclass->pc_data[0] ++;

        }

    }

/*
    source - i/o methods
 */

    le_enum_t le_pclass_io_read( le_pclass_t * const le_pclass, le_size_t const le_offset, le_file_t const le_stream ) {

        /* size variable */
        le_size_t le_size = sizeof( le_size_t );

        /* stream offset */
        fseek( le_stream, le_offset, SEEK_SET );

        /* import class data - header */
        if ( fread( le_pclass->pc_data, sizeof( le_byte_t ), 1, le_stream ) != 1 ) {

            /* send message */
            return( LE_ERROR_IO_READ );

        } else {

            /* compute data size */
            le_size *= le_pclass->pc_data[0];

            /* import class data */
            if ( fread( le_pclass->pc_data + 1, sizeof ( le_byte_t ), le_size, le_stream ) != le_size ) {

                /* send message */
                return( LE_ERROR_IO_READ );

            } else {

                /* import class offset */
                if ( fread( le_pclass->pc_data + LE_PCLASS_DATA, sizeof( le_byte_t ), LE_PCLASS_OFFSET, le_stream ) != LE_PCLASS_OFFSET ) {

                    /* send message */
                    return( LE_ERROR_IO_READ );

                } else {

                    /* send message */
                    return( LE_ERROR_SUCCESS );

                }

            }

        }

    }

    le_enum_t le_pclass_io_write( le_pclass_t * const le_pclass, le_size_t const le_offset, le_file_t const le_stream ) {

        /* size variable */
        le_size_t le_size = le_pclass->pc_data[0] * sizeof( le_size_t ) + sizeof( le_byte_t );

        /* check offset */
        if ( le_offset != _LE_OFFS_NULL ) {

            /* stream offset */
            fseek( le_stream, le_offset, SEEK_SET );

        }

        /* export class data */
        if ( fwrite( le_pclass->pc_data, sizeof( le_byte_t ), le_size, le_stream ) != le_size ) {

            /* send message */
            return( LE_ERROR_IO_WRITE );

        } else {

            /* export class offset */
            if ( fwrite( le_pclass->pc_data + LE_PCLASS_DATA, sizeof( le_byte_t ), LE_PCLASS_OFFSET, le_stream ) != LE_PCLASS_OFFSET ) {

                /* send message */
                return( LE_ERROR_IO_WRITE );

            } else {

                /* send message */
                return( LE_ERROR_SUCCESS );

            }

        }

    }

