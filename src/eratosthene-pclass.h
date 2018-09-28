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

    /*! \file   eratosthene-pclass.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  liberatosthene - pclass (poly-vertex class)
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_PCLASS__
    # define __LE_PCLASS__

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
    # define LE_PCLASS_C      { { 0 } }

    /* define class limit */
    # define LE_PCLASS_SIZE   ( 255 )

    /* define class structure */
    # define LE_PCLASS_DATA   ( sizeof( le_byte_t ) + sizeof( le_size_t ) * LE_PCLASS_SIZE )

    /* define class structure */
    # define LE_PCLASS_OFFSET ( _LE_USE_OFFSET * _LE_USE_BASE )

    /* define class memory */
    # define LE_PCLASS_ARRAY  ( LE_PCLASS_DATA + LE_PCLASS_OFFSET )

    /* define class memory */
    # define LE_PCLASS_MEMORY ( LE_PCLASS_ARRAY + sizeof( le_size_t ) - _LE_USE_OFFSET )

/*
    header - preprocessor macros
 */

    /* define offset access */
    # define le_pclass_mac_offset(c,i) ( ( le_size_t * ) ( ( c )->pc_data + LE_PCLASS_DATA + ( _LE_USE_OFFSET * ( i ) ) ) )

/*
    header - type definition
 */

/*
    header - structures
 */

    /* *** */

    typedef struct le_pclass_struct {

        le_byte_t pc_data[LE_PCLASS_MEMORY];

    } le_pclass_t;

/*
    header - function prototypes
 */

    /* *** */

    le_pclass_t le_pclass_create( le_void_t );

    /* *** */

    le_void_t le_pclass_delete( le_pclass_t * const le_pclass );

    /* *** */

    le_size_t le_pclass_get_offset( le_pclass_t const * const le_pclass, le_size_t const le_index );

    /* *** */

    le_void_t le_pclass_set_offset( le_pclass_t * const le_pclass, le_size_t const le_index, le_size_t const le_offset );

    /* *** */

    le_void_t le_pclass_set_push( le_pclass_t * const le_pclass, le_size_t const le_link );

    /* *** */

    le_enum_t le_pclass_io_read( le_pclass_t * const le_pclass, le_size_t const le_offset, le_file_t const le_stream );

    /* *** */

    le_enum_t le_pclass_io_write( le_pclass_t * const le_pclass, le_size_t const le_offset, le_file_t const le_stream );

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

