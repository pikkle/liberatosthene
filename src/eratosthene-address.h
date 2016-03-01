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

    /* Define digital boundaries */
    # define LE_ADDRESS_BASE 8

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct le_address_struct
        \brief address structure
     */

    typedef struct le_address_struct {

        le_size_t as_size;
        le_byte_t as_addr[_LE_USE_DEPTH];

    } le_address_t;

/*
    header - function prototypes
 */

    /*! \brief accessor methods
     */

    le_size_t le_address_get_size( le_address_t const * const le_address );

    /*! \brief accessor methods
     */

    le_byte_t le_address_get_digit( le_address_t const * const le_address, le_size_t const le_offset );

    /*! \brief mutator methods
     */

    le_enum_t le_address_set_size( le_address_t * const le_address, le_size_t const le_size );

    /*! \brief mutator methods
     */

    le_enum_t le_address_set_digit( le_address_t * const le_address, le_size_t const le_offset, le_byte_t const le_digit );

    /*! \brief conversion methods
     */

    le_void_t le_address_cgd( le_address_t * const le_address, le_real_t * const le_pose );

    /*! \brief conversion methods
     */

    le_void_t le_address_cdg( le_address_t const * const le_address, le_real_t * const le_pose );

    /*! \brief conversion methods
     */

    le_void_t le_address_cas( le_address_t const * const le_address, le_char_t * const le_string );

    /*! \brief conversion methods
     */

    le_void_t le_address_csa( le_address_t * const le_address, le_char_t const * const le_string );

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

