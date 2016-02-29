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

    /*! \file   eratosthene-element.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Array module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_SYSTEM_ARRAY__
    # define __LE_SYSTEM_ARRAY__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - includes
 */

    # include "eratosthene.h"

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor/destructor */
    # define LE_ARRAY_C    { 0, 0, NULL }
    # define LE_ARRAY_D(a) { if ( ( a )->ar_size > 0 ) { free( ( a )->ar_data ); ( a )->ar_size = 0; } }

    /* Define element sizes */
    # define LE_ARRAY_SLEN ( sizeof( le_real_t ) * 3 )
    # define LE_ARRAY_DLEN ( sizeof( le_data_t ) * 3 )
    # define LE_ARRAY_ELEN ( LE_ARRAY_SLEN + LE_ARRAY_DLEN )

    /* Define array allocation step */
    # define LE_ARRAY_STEP ( LE_ARRAY_ELEN * 4096 )

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    typedef struct le_array_struct {

        le_size_t   ar_size;
        le_size_t   ar_head;
        le_byte_t * ar_data;

    } le_array_t;

/*
    header - function prototypes
 */

    le_real_t * le_array_get_pose( le_array_t const * const le_array, le_size_t const le_offset );
    le_data_t * le_array_get_data( le_array_t const * const le_array, le_size_t const le_offset );
    le_enum_t le_array_set( le_array_t * le_array, le_real_t const * const le_pose, le_data_t const * const le_data );

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

