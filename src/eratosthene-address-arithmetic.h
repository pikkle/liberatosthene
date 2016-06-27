/*
 *  liberatosthene - eratosthene indexation server
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

    /*! \file   eratosthene-address-arithmetic.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Address module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_SYSTEM_ADDRESS_ARITHMETIC__
    # define __LE_SYSTEM_ADDRESS_ARITHMETIC__

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
    # include "eratosthene-address.h"

/*
    header - preprocessor definitions
 */

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

/*
    header - function prototypes
 */

    /*! \brief accessor methods
     *
     *  This function implements a specific mathematical distance on addresses.
     *  Given two addresses, it returns the highest offset (scale) at which the 
     *  addresses digits disagree. Considering the following addresses :
     *
     *      100020221346271521225
     *      100020222346271521225
     *
     *  the function returns 8.
     *
     *  When two addresses are identical and of the same size, the function
     *  always returns the addresses size plus one. In case of addresses with
     *  different sizes, the function starts the comparison considering the
     *  smallest addresses size. Providing the following addresses :
     *
     *      100020221346271521225
     *      1000202213
     *
     *  makes the function to answer 10.
     *
     *  \param le_addr1 Address structure
     *  \param le_addr2 Address structure
     *
     *  \return Returns computed distance
     */

    le_size_t le_address_get_dist( le_address_t const * const le_addr1, le_address_t const * const le_addr2 );

    /*! \brief mutator methods
     *
     *  This function expects a whole number and converts it into an address
     *  index. In other words, the function interprets the value as an address
     *  index following the constraint on address digits bases.
     *
     *  Considering the value 123456789123456789, a conversion offset of 20 and
     *  an address index of size 32, the function fills the index as follows :
     *
     *      03103122116546405742500000000000
     *
     *  This function is mostly dedicated to initialise index used in addresses
     *  arithmetic.
     *
     *  \param le_address Address structure
     *  \param le_offset  Address conversion offset
     *  \param le_value   Address conversion value
     *
     *  \return Returns LE_ERROR_SUCCESS on success, LE_ERROR_DEPTH otherwise
     */

    le_enum_t le_address_set_value( le_address_t * const le_address, le_size_t le_offset, le_size_t le_value );

    /*! \brief mutator methods
     *
     *  This function computes the addition of the addresses index provided as
     *  parameters and stores the result in the address structure also given
     *  as parameter. Both operand addresses have to be of the same size.
     *
     *  The addition is performed as for usual whole numbers expect that the
     *  base of the digits depends on their offsets. Considering the following
     *  addresses index :
     *
     *      03103122116546405742500000000000
     *      00023223004417762422444000000000
     *
     *  the function returns the following index :
     *
     *      03133011123166370365144000000000
     *
     *  through the provided structure. As for traditionnal addition, the index
     *  addition is commutative.
     *
     *  Overflow are not handled by the function. A non zero remainder is simply
     *  ignored.
     *
     *  \param le_addr  Address structure of result
     *  \param le_addr1 Address structure of first operand
     *  \param le_addr2 Address structure of second operand
     *
     *  \return Returns LE_ERROR_SUCCESS on success, LE_ERROR_DEPTH otherwise
     */

    le_enum_t le_address_set_add( le_address_t * const le_addr, le_address_t const * const le_addr1, le_address_t const * const le_addr2 );

    /*! \brief mutator methods
     *
     *  This function computes the substraction of the addresses index provided
     *  as parameters and returns the result through the address structure also
     *  provided as parameter. Both operand addresses have to be of the same
     *  size.
     *
     *  Considering the example given in the documentation of the addition
     *  function le_address_set_add(), substracting the following addresses
     *  index :
     *
     *      03133011123166370365144000000000
     *      00023223004417762422444000000000
     *
     *  gives back the following index :
     *
     *      03103122116546405742500000000000
     *
     *  as expected.
     *
     *  Overflow are not handled by the function. A non zero remainder is simply
     *  ignored.
     *
     *  \param le_addr  Address structure of result
     *  \param le_addr1 Address structure of first operand
     *  \param le_addr2 Address structure of second operand
     *
     *  \return Returns LE_ERROR_SUCCESS on success, LE_ERROR_DEPTH otherwise
     */

    le_enum_t le_address_set_sub( le_address_t * const le_addr, le_address_t const * const le_addr1, le_address_t const * const le_addr2 );

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

