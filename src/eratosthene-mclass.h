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

    /*! \file   eratosthene-mclass.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  liberatosthene - mclass (mono-vertex class)
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_MCLASS__
    # define __LE_MCLASS__

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

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define LE_MCLASS_C { 0, { 0.0 }, { 0 } }

    /* define class structure */
    # define LE_MCLASS_HEAD   ( sizeof( le_byte_t ) * 3 )

    /* define class structure */
    # define LE_MCLASS_OFFSET ( _LE_USE_OFFSET * _LE_USE_BASE )

    /* define class structure */
    # define LE_MCLASS_FIXED  ( LE_MCLASS_HEAD + LE_MCLASS_OFFSET )

    /* define class memory */
    # define LE_MCLASS_MEMORY ( LE_MCLASS_FIXED + _LE_USE_OFFSET )

/*
    header - preprocessor macros
 */

    /* define offset access */
    # define le_mclass_mac_offset(c,i) ( ( le_size_t * ) ( ( c )->mc_data + LE_MCLASS_HEAD + ( _LE_USE_OFFSET * ( i ) ) ) )

/*
    header - type definition
 */

/*
    header - structures
 */

    /* *** */

    typedef struct le_mclass_struct {

        le_size_t mc_size;
        le_real_t mc_push[3];
        le_byte_t mc_data[LE_MCLASS_MEMORY];

    } le_mclass_t;

/*
    header - function prototypes
 */

    /* *** */

    le_mclass_t le_mclass_create( le_void_t );

    /* *** */

    le_void_t le_mclass_reset( le_mclass_t * const le_mclass );

    /* *** */

    le_void_t le_mclass_delete( le_mclass_t * const le_mclass );

    /* *** */

    le_size_t le_mclass_get_offset( le_mclass_t const * const le_mclass, le_size_t const le_index );

    /* *** */

    le_void_t le_mclass_set_offset( le_mclass_t * const le_mclass, le_size_t const le_index, le_size_t const le_offset );

    /* *** */

    le_enum_t le_mclass_read( le_mclass_t  * const le_mclass, le_size_t const le_offset, le_file_t const le_stream );

    /* *** */

    le_enum_t le_mclass_read_fast( le_mclass_t * const le_mclass, le_size_t const le_offset, le_file_t const le_stream );

    /* *** */

    le_enum_t le_mclass_write( le_mclass_t * const le_mclass, le_size_t const le_offset, le_file_t const le_stream );

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

