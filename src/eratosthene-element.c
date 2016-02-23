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

    # include "eratosthene-element.h"

/*
    source - i/o methods
 */

    le_enum_t le_element_scan( le_element_t * const le_element, FILE * const le_stream ) {

        /* Check stream */
        # if ( _LE_USE_STDIO_EXT == _LE_TRUE )
        if ( __freadable( le_stream ) == 0 ) {

            /* Send message */
            return( LE_ERROR_IO_STREAM );

        }
        # endif

        /* Scan element components */
        if ( fscanf( le_stream, "%f %f %f %hhu %hhu %hhu %li",

            & le_element->em_pose[0],
            & le_element->em_pose[1],
            & le_element->em_pose[2],
            & le_element->em_data[0],
            & le_element->em_data[1],
            & le_element->em_data[2],
            & le_element->em_time

        ) != 7 ) {

            /* Send message */
            return( LE_ERROR_IO_READ );

        } else {

            /* Send message */
            return( LE_ERROR_SUCCESS );

        }

    }

    le_enum_t le_element_read( le_element_t * const le_element, FILE * const le_stream ) {

        /* Check stream */
        # if ( _LE_USE_STDIO_EXT == _LE_TRUE )
        if ( __freadable( le_stream ) == 0 ) {

            /* Send message */
            return( LE_ERROR_IO_STREAM );

        }
        # endif

        /* Write spatial components */
        if ( fread( ( le_void_t * ) le_element->em_pose, sizeof( le_real_t ), 3, le_stream ) != 3 ) {

            /* Send message */
            return( LE_ERROR_IO_READ );

        }

        /* Write colorimetric components */
        if ( fread( ( le_void_t * ) le_element->em_data, sizeof( le_byte_t ), 3, le_stream ) != 3 ) {

            /* Send message */
            return( LE_ERROR_IO_READ );

        }

        /* Write temporal components */
        if ( fread( ( le_void_t * ) ( & le_element->em_time ), sizeof( le_time_t ), 1, le_stream ) != 1 ) {

            /* Send message */
            return( LE_ERROR_IO_READ );

        } else {

            /* Send message */
            return( LE_ERROR_SUCCESS );

        }

    }

    le_enum_t le_element_print( le_element_t const * const le_element, FILE * const le_stream ) {

        /* Check stream */
        # if ( _LE_USE_STDIO_EXT == _LE_TRUE )
        if ( __fwritable( le_stream ) == 0 ) {

            /* Send message */
            return( LE_ERROR_IO_STREAM );

        }
        # endif

        /* Scan element components */
        if ( fprintf( le_stream, "%.14e %.14e %.14e %hhu %hhu %hhu %li",

            le_element->em_pose[0],
            le_element->em_pose[1],
            le_element->em_pose[2],
            le_element->em_data[0],
            le_element->em_data[1],
            le_element->em_data[2],
            le_element->em_time

        ) != 7 ) {

            /* Send message */
            return( LE_ERROR_IO_READ );

        } else {

            /* Send message */
            return( LE_ERROR_SUCCESS );

        }

    }

    le_enum_t le_element_write( le_element_t const * const le_element, FILE * const le_stream ) {

        /* Check stream */
        # if ( _LE_USE_STDIO_EXT == _LE_TRUE )
        if ( __fwritable( le_stream ) == 0 ) {

            /* Send message */
            return( LE_ERROR_IO_STREAM );

        }
        # endif

        /* Write spatial components */
        if ( fwrite( ( le_void_t * ) le_element->em_pose, sizeof( le_real_t ), 3, le_stream ) != 3 ) {

            /* Send message */
            return( LE_ERROR_IO_READ );

        }

        /* Write colorimetric components */
        if ( fwrite( ( le_void_t * ) le_element->em_data, sizeof( le_byte_t ), 3, le_stream ) != 3 ) {

            /* Send message */
            return( LE_ERROR_IO_READ );

        }

        /* Write temporal components */
        if ( fwrite( ( le_void_t * ) ( & le_element->em_time ), sizeof( le_time_t ), 1, le_stream ) != 1 ) {

            /* Send message */
            return( LE_ERROR_IO_READ );

        } else {

            /* Send message */
            return( LE_ERROR_SUCCESS );

        }

    }

