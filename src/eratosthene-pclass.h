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
    # define LE_PCLASS_C      { 0, NULL, { 0 } }

    /* define class structure */
    # define LE_PCLASS_HEAD   ( sizeof( le_pdata_t ) )

    /* define class structure */
    # define LE_PCLASS_OFFSET ( _LE_USE_OFFSET * _LE_USE_BASE )

    /* define class structure */
    # define LE_PCLASS_FIXED  ( LE_PCLASS_HEAD + LE_PCLASS_OFFSET )

    /* define class memory */
    # define LE_PCLASS_MEMORY ( LE_PCLASS_FIXED + _LE_USE_OFFSET )

    /* define class management */
    # define LE_PCLASS_STEP   ( 32 )

    /* define class limit */
    # define LE_PCLASS_LIMIT  ( UINT16_MAX )

/*
    header - preprocessor macros
 */

    /* define offset access */
    # define le_pclass_mac_offset(c,i) ( ( le_size_t * ) ( ( c )->pc_data + LE_PCLASS_HEAD + ( _LE_USE_OFFSET * ( i ) ) ) )

/*
    header - type definition
 */

    /* class data */
    typedef uint16_t le_pdata_t;

/*
    header - structures
 */

    /* *** */

    typedef struct le_pclass_struct {

        le_size_t   pc_size;
        le_size_t * pc_link;
        le_byte_t   pc_data[LE_PCLASS_MEMORY];

    } le_pclass_t;

/*
    header - function prototypes
 */

    /* *** */

    le_pclass_t le_pclass_create( le_void_t );

    /* *** */

    le_void_t le_pclass_reset( le_pclass_t * const le_pclass );

    /* *** */

    le_void_t le_pclass_delete( le_pclass_t * const le_pclass );

    /* *** */

    le_size_t le_pclass_get_size( le_pclass_t const * const le_pclass );

    /* *** */

    le_size_t le_pclass_get_link( le_pclass_t const * const le_pclass, le_size_t const le_index );

    /* *** */

    le_size_t le_pclass_get_offset( le_pclass_t const * const le_pclass, le_size_t const le_index );

    /* *** */

    le_enum_t le_pclass_set_memory( le_pclass_t * const le_pclass, le_size_t const le_size );

    /* *** */

    le_void_t le_pclass_set_offset( le_pclass_t * const le_pclass, le_size_t const le_index, le_size_t const le_offset );

    /* *** */

    le_enum_t le_pclass_set_push( le_pclass_t * const le_pclass, le_size_t const le_link );

    /* *** */

    le_enum_t le_pclass_io_read( le_pclass_t * const le_pclass, le_size_t const le_offset, le_file_t const le_stream );

    /* *** */

    le_enum_t le_pclass_io_read_fast( le_pclass_t * const le_pclass, le_size_t const le_offset, le_file_t const le_stream );

    /* *** */

    le_enum_t le_pclass_io_read_next( le_pclass_t * const le_pclass, le_file_t const le_stream );

    /* *** */

    le_enum_t le_pclass_io_write( le_pclass_t const * const le_pclass, le_size_t const le_offset, le_file_t const le_stream );

    /*! \brief i/o methods (detached)
     *
     *  Note :
     *
     *  Detached methods are specific function associated to a structure but
     *  without operating directly on it. Such function are used in optimisation
     *  strategies.
     */

    le_size_t le_pclass_io_offset( le_size_t const le_offset, le_size_t const le_index, le_file_t const le_stream );

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

