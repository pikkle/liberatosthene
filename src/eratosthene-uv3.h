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

    /*! \file   eratosthene-uv3.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  liberatosthene - uv3
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_UV3__
    # define __LE_UV3__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - internal includes
 */

    # include "eratosthene.h"
    # include "eratosthene-address.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define uv3 primitive */
    # define LE_UV3_POINT         ( 1 )
    # define LE_UV3_LINE          ( 2 )
    # define LE_UV3_TRIANGLE      ( 3 )

    /* define standard chunk size */
    # define LE_UV3_CHUNK         ( 2396745 )

/*
    header - preprocessor macros
 */

    /* uv3 record access */
    # define le_uv3_get_type( r ) ( * ( ( le_byte_t * ) ( r + LE_ARRAY_DATA_POSE ) ) )

/*
    header - type definition
 */

/*
    header - structures
 */

/*
    header - function prototypes
 */

    /* *** note : the function assume a complete uv3 stream chunk (no primitive truncation) */

    le_byte_t * le_uv3_set_sort( le_byte_t * const le_buffer, le_size_t const le_size, le_size_t const le_scfg );

    /* *** */

    le_enum_t le_uv3_set_merge( le_char_t const * const le_pfile, le_char_t const * const le_qfile, le_char_t const * const le_file, le_size_t const le_scfg );

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    }
    # endif

/*
    header - inclusion guard
 */

    # endif

