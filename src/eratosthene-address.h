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
    # include "eratosthene-network.h"

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define LE_ADDRESS_C           { 0, 0, { 0 }, 0 }
    # define LE_ADDRESS_C_SIZE( s ) { s, 0, { 0 }, 0 }

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
        le_time_t as_time;
        le_byte_t as_addr[_LE_USE_DEPTH];
        le_size_t as_dept;

    } le_address_t;

/*
    header - function prototypes
 */

    /*! \brief accessor methods
     */

    le_time_t le_address_get_time( le_address_t const * const le_address );

    /*! \brief accessor methods
     */

    le_size_t le_address_get_size( le_address_t const * const le_address );

    /*! \brief accessor methods
     */

    le_byte_t le_address_get_digit( le_address_t const * const le_address, le_size_t const le_offset );

    /*! \brief accessor methods
     */

    le_byte_t * le_address_get_digits( le_address_t const * const le_address );

    /*! \brief accessor methods
     */

    le_size_t le_address_get_depth( le_address_t const * const le_address );

    /*! \brief accessor methods
     */

    le_enum_t le_address_get_valid( le_address_t const * const le_address );

    /*! \brief accessor methods
     */

    le_void_t le_address_get_pose( le_address_t const * const le_address, le_real_t * const le_pose );

    /*! \brief mutator methods
     */

    le_void_t le_address_set_time( le_address_t * const le_address, le_time_t const le_time );

    /*! \brief mutator methods
     */

    le_enum_t le_address_set_size( le_address_t * const le_address, le_size_t const le_size );

    /*! \brief mutator methods
     */

    le_enum_t le_address_set_digit( le_address_t * const le_address, le_size_t const le_offset, le_byte_t const le_digit );

    /*! \brief mutator methods
     */

    le_void_t le_address_set_depth( le_address_t * const le_address, le_size_t const le_depth );

    /*! \brief mutator methods
     */

    le_void_t le_address_set_pose( le_address_t * const le_address, le_real_t * const le_pose );

    /*! \brief mutator methods
     */

    le_void_t le_address_set_shift( le_address_t * const le_address, le_size_t const le_scale, le_diff_t le_xcarry, le_diff_t le_ycarry, le_diff_t le_zcarry );

    /*! \brief conversion methods
     */

    le_void_t le_address_cvas( le_address_t const * const le_address, le_char_t * const le_string );

    /*! \brief conversion methods
     */

    le_void_t le_address_cvsa( le_address_t * const le_address, le_char_t const * const le_string );

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

