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

    /*! \file   eratosthene-address.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Address module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_SYSTEM_ADDRESS__
    # define __LE_SYSTEM_ADDRESS__

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
    # include "eratosthene-geodesy.h"

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define LE_ADDRESS_C           { 0, { 0 } }
    # define LE_ADDRESS_C_SIZE( s ) { s, { 0 } }

    /* Define null digit */
    # define LE_ADDRESS_NULL        ( 0xFF )

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    typedef struct le_address_struct {

        le_size_t as_size;
        le_byte_t as_addr[LE_DEPTH_MAX];

    } le_address_t;

/*
    header - function prototypes
 */

    le_size_t le_address_get_size( le_address_t const * const le_address );
    le_byte_t le_address_get_digit( le_address_t const * const le_address, le_size_t const le_offset );
    le_enum_t le_address_set_size( le_address_t * const le_address, le_size_t const le_size );
    le_enum_t le_address_set_digit( le_address_t * const le_address, le_size_t const le_offset, le_byte_t const le_digit );
    le_void_t le_address_sys( le_address_t * const le_address, le_real_t * const le_pose );
    le_void_t le_address_geo( le_address_t const * const le_address, le_real_t * const le_pose );
    le_void_t le_address_binary_string( le_address_t const * const le_address, le_char_t * const le_string );
    le_void_t le_address_string_binary( le_address_t * const le_address, le_char_t const * const le_string );

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

