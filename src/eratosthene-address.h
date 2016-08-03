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

    /* Base according to scale */
    # define LE_ADDRESS_BASE( s )   ( ( ( s ) < LE_GEODESY_ASYA ) ? ( _LE_USE_BASE >> 1 ) : ( ( ( s ) < LE_GEODESY_ASYP ) ? ( _LE_USE_BASE >> 2 ) : _LE_USE_BASE ) )

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct le_address_struct
     *  \brief address structure
     *
     *  This structure holds the address of an equivalence class defined on the
     *  geodetic parameters space. It consists in a sequence of digits going
     *  from zero to seven stored in the structure array. The number of digit
     *  engeged in the class addressing is stored in the size field of the
     *  structure. Digits are understood as the parent-daughter classes 
     *  relationship explaining which daughter to consider when considering a 
     *  given parent class.
     *
     *  In addition to the class address, the structure also contains the time
     *  at which the class as to be searched. In this way, this structure is
     *  able to address four dimensional equivalence classes.
     *
     *  Finally, the structure offers the possibility to store an additional
     *  depth considered when performing queries on the daughters of the class
     *  pointed by the address.
     *
     *  The geodetic parameters space is understood as a three dimensional
     *  sub-space of R3 for longitude, latitude and normalised altitude, in 
     *  this order and expressed in radians and metres. The first dimension 
     *  range is then ]-pi,pi], ]-pi/2,pi/2[ for the second and [hmin, hmax] 
     *  for the thrid dimension.
     *
     *  Times are given by signed whole numbers giving the number of seconds
     *  since EPOCH (UTC). Time are assumed to follow TAI more than UTC.
     *
     *  See eratosthene-geodesy header for more information on parametric
     *  ranges.
     *
     *  \var le_address_struct::as_size
     *  Size of the address - number of digits of the address
     *  \var le_address_struct::as_time
     *  Time of the class pointed by the address
     *  \var le_address_struct::as_digit
     *  Address digits array - contains whole number in [0,8[
     *  \var le_address_struct::as_depth
     *  Address query depth
     */

    typedef struct le_address_struct {

        le_size_t as_size; /* Size, not last 0-based index */
        le_time_t as_time;
        le_byte_t as_digit[_LE_USE_DEPTH];
        le_size_t as_depth;

    } le_address_t;

/*
    header - function prototypes
 */

    /*! \brief accessor methods
     *
     *  Returns time of the class stored in the address structure.
     *
     *  See \b le_address_struct documentation for more information on frames.
     *
     *  \param le_address Address structure
     *
     *  \return Address time
     */

    le_time_t le_address_get_time( le_address_t const * const le_address );

    /*! \brief accessor methods
     *
     *  Returns the size, i. e. the number of digits, of the class address 
     *  stored in the address structure.
     *
     *  \param le_address Address structure
     *
     *  \return Address size
     */

    le_size_t le_address_get_size( le_address_t const * const le_address );

    /*! \brief accessor methods
     *
     *  Returns the digit at queried offset of the class address stored in the
     *  address structure.
     *
     *  \param le_address Address structure
     *  \param le_offset  Offset of the digit - zero based
     *
     *  \return Returns digit found at queried offset, _LE_BYTE_NULL otherwise
     */

    le_byte_t le_address_get_digit( le_address_t const * const le_address, le_size_t const le_offset );

    /*! \brief accessor methods
     *
     *  Returns query depth of the class stored in the address structure.
     *
     *  \param le_address Address structure
     *
     *  \return Query depth
     */

    le_size_t le_address_get_depth( le_address_t const * const le_address );

    /*! \brief accessor methods
     *
     *  This function converts the indexation address of the class stored in
     *  the address structure into a geodetic coordinates vector. The position
     *  is expressed in the spatial ranges on which indexation is defined.
     *
     *  See \b le_address_struct documentation for more information on frames.
     *
     *  \param le_address Address structure
     *  \param le_pose    Array receiving the position coordinates
     */

    le_void_t le_address_get_pose( le_address_t const * const le_address, le_real_t * const le_pose );

    /*! \brief mutator methods
     *
     *  Set the time of the class stored in the address structure.
     *
     *  See \b le_address_struct documentation for more information on frames.
     *
     *  \param le_address Address structure
     *  \param le_time    Address time
     */

    le_void_t le_address_set_time( le_address_t * const le_address, le_time_t const le_time );

    /*! \brief mutator methods
     *
     *  Set the size of the class address stored in the address structure.
     *
     *  \param le_address Address structure
     *  \param le_size    Address sizs
     *
     *  \return Returns LE_ERROR_SUCCESS on success, LE_ERROR_DEPTH otherwise
     */

    le_enum_t le_address_set_size( le_address_t * const le_address, le_size_t const le_size );

    /*! \brief mutator methods
     *
     *  Set address digit at provided offset of the class address stored in the
     *  address structure.
     *
     *  \param le_address Address structure
     *  \param le_offset  Offset of the digit to set
     *  \param le_digit   Digit value
     *
     *  \return Returns LE_ERROR_SUCCESS on success, LE_ERROR_DEPTH if offset is
     *  inconsistent and LE_ERROR_BASE if digit is inconsistent
     */

    le_enum_t le_address_set_digit( le_address_t * const le_address, le_size_t const le_offset, le_byte_t const le_digit );

    /*! \brief mutator methods
     *
     *  Set the query depth of the class stored in the address structure.
     *
     *  \param le_address Address structure
     *  \param le_depth   Query depth
     *
     *  \return Returns LE_ERROR_SUCCESS on success, LE_ERROR_DEPTH otherwise
     */

    le_enum_t le_address_set_depth( le_address_t * const le_address, le_size_t const le_depth );

    /*! \brief mutator methods
     *
     *  This function convert a geodetic coordinate vector into a class address
     *  following the defined spatial indexation. The size of the computed
     *  address is read in the structure size field that as to contain the
     *  desired value before calling this function.
     *
     *  The position vector has to be expressed in the geodetic coordinate
     *  ranges on which spatial indexation is defined.
     *
     *  See \b le_address_struct documentation for more information on frames.
     *
     *  \param le_address Address structure
     *  \param le_pose    Array containing the position coordinates
     */

    le_void_t le_address_set_pose( le_address_t * const le_address, le_real_t * const le_pose );

    le_enum_t le_address_cmp( le_address_t const * const le_addr1, le_address_t const * const le_addr2 );

    /*! \brief i/o methods
     *
     */

    le_enum_t le_address_io_read( le_address_t * const le_address, le_sock_t const le_socket );

    /*! \brief i/o methods
     *
     */

    le_enum_t le_address_io_write( le_address_t const * const le_address, le_sock_t const le_socket );

    /*! \brief conversion methods
     *
     *  This function translate the address structure into a query string used
     *  for client/server communication. Considering the class time t, address
     *  digits dd...d and query depth q, the string is composed as follows : 
     *
     *      /t/dd..d/q
     *
     *  \param le_address Address structure
     *  \param le_string  String receiving the converted address structure
     */

    le_void_t le_address_ct_string( le_address_t const * const le_address, le_char_t * const le_string );

    /*! \brief conversion methods
     *
     *  This function performs the invert operation done by \b le_address_cvas
     *  function. The string has to follow the expected standard in order to
     *  compute a correct class address structure.
     *
     *  \param le_address Address structure
     *  \param le_string  String containing the converted address structure
     */

    le_void_t le_address_cf_string( le_address_t * const le_address, le_char_t const * const le_string );

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

