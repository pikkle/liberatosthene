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

    /*! \file   eratosthene-address.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  liberatosthene - address
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_ADDRESS__
    # define __LE_ADDRESS__

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
    # include "eratosthene-array.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define LE_ADDRESS_C             { 0, 0, 0, { _LE_TIME_NULL, _LE_TIME_NULL, _LE_TIME_NULL }, { 0 } }
    # define LE_ADDRESS_C_SIZE(s)     { s, 0, 0, { _LE_TIME_NULL, _LE_TIME_NULL, _LE_TIME_NULL }, { 0 } }

    /* define address mode */
    # define LE_ADDRESS_OR            ( 3 )
    # define LE_ADDRESS_AND           ( 4 )
    # define LE_ADDRESS_XOR           ( 5 )

    /* asynchronous dimension */
    # define LE_ADDRESS_DEPTH_P       ( 1 )
    # define LE_ADDRESS_DEPTH_A       ( 10 )

    /* define WGS84 ellipsoid parameters */
    # define LE_ADDRESS_WGS_A         ( 6378137.0 )

    /* define ellispoidal coordinates boundaries */
    # define LE_ADDRESS_MIN_L         ( - LE_PI )
    # define LE_ADDRESS_MAX_L         ( + LE_PI )
    # define LE_ADDRESS_MIN_A         ( - LE_P2 )
    # define LE_ADDRESS_MAX_A         ( + LE_P2 )
    # define LE_ADDRESS_MIN_H         ( - ( LE_PI * LE_ADDRESS_WGS_A ) / 1024.0 )
    # define LE_ADDRESS_MAX_H         ( + ( LE_PI * LE_ADDRESS_WGS_A ) / 1024.0 )

    /* define ellipsoidal coordinates ranges */
    # define LE_ADDRESS_RAN_L         ( LE_ADDRESS_MAX_L - LE_ADDRESS_MIN_L )
    # define LE_ADDRESS_RAN_A         ( LE_ADDRESS_MAX_A - LE_ADDRESS_MIN_A )
    # define LE_ADDRESS_RAN_H         ( LE_ADDRESS_MAX_H - LE_ADDRESS_MIN_H )

    /* define ellipsoidal coordinates ranges */
    # define LE_ADDRESS_IRN_L         ( 1.0 / LE_ADDRESS_RAN_L )
    # define LE_ADDRESS_IRN_A         ( 1.0 / LE_ADDRESS_RAN_A )
    # define LE_ADDRESS_IRN_H         ( 1.0 / LE_ADDRESS_RAN_H )

/*
    header - preprocessor macros
 */

    /* base according to scale */
    # define le_address_base(s)       ( ( s ) < LE_ADDRESS_DEPTH_P ) ? ( _LE_USE_BASE >> 2 ) : ( ( ( s ) < LE_ADDRESS_DEPTH_A ) ? ( _LE_USE_BASE >> 1 ) : ( _LE_USE_BASE ) )

    /* method alias */
    # define le_address_get_pose(a,p) ( le_address_get_pose_( ( a ), ( a )->as_size, ( p ) ) )

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct le_address_struct
     *  \brief address structure ( revoked )
     *
     *  This structure holds the address, or the index, of an equivalence class
     *  defined on the geodetic and time parameter spaces. These addresses are
     *  used to store and access data linked to spatio-temporal references.
     *
     *  The structure holds two distinct times in order to allows comparison
     *  methods between different times to take place. Then, the mode parameter
     *  is used to store the information on comparison method :
     *
     *      mode = 1 : first time only
     *      mode = 2 : second time only
     *      mode = 3 : first time (logical or) second time
     *      mode = 4 : first time (logical and) second time
     *      mode = 5 : first time (logical xor) second time
     *
     *  The proper spatial address of the pointed equivalence class is stored
     *  in an array holding the digits of the address. The amount of digits is
     *  provided by the size field.
     *
     *  In the last place the structure holds an depth parameter that gives,
     *  usually in case of data query, the additional scale depth where the
     *  desired data have to be gathered as an equivalence class contains only
     *  a single element.
     *
     *  Times are understood under the UTC standard in the way they give the
     *  amount of seconds elapsed since EPOCH (UTC) with consideration of leap
     *  seconds.
     *
     *  As an example, the following address, written in its text form :
     *
     *      /3/950486422,-7258032000/122010001340232/7
     *
     *  represents the data located in Venice in 2000 and in 1740. The first
     *  number correspond to the times comparison mode. The two times are given
     *  next followed by the digits of the spatial address. The last number
     *  gives the additional depth, from the scale point of view, where the
     *  desired data have to be searched.
     *
     *  \var le_address_struct::as_size
     *  Address number of digits
     *  \var le_address_struct::as_mode
     *  Times comparison mode
     *  \var le_address_struct::as_span
     *  Address additionnal depth
     *  \var le_address_struct::as_times
     *  Array containing the address times
     *  \var le_address_struct::as_digit
     *  Array containing the address digit
     */

    typedef struct le_address_struct {

        le_byte_t as_size;
        le_byte_t as_mode;
        le_byte_t as_span;

        le_time_t as_times[_LE_USE_TIMES];
        le_byte_t as_digit[_LE_USE_DEPTH];

    } le_address_t;

/*
    header - function prototypes
 */

    /*! \brief accessor methods
     *
     *  Returns the size, ie. the number of digits, of the class address stored
     *  in the address structure.
     *
     *  \param le_address Address structure
     *
     *  \return Address number of digits
     */

    le_byte_t le_address_get_size( le_address_t const * const le_address );

    /*! \brief accessor methods
     *
     *  Returns the address times comparison mode.
     *
     *  \param le_address Address structure
     *
     *  \return Address times comparison mode
     */

    le_byte_t le_address_get_mode( le_address_t const * const le_address );

    /*! \brief accessor methods ( revoked - used for comb also )
     *
     *  Returns the desired time of the equivalence class stored in the address
     *  structure. The provided offset has to be 0 or 1 to retrieve the first
     *  and second time, respectively.
     *
     *  \param le_address Address structure
     *  \param le_offset  Offset of time - zero based
     *
     *  \return Desired address time
     */

    le_time_t le_address_get_time( le_address_t const * const le_address, le_size_t const le_offset );

    /*! \brief accessor methods
     *
     *  Returns the desired digit of the index stored in the address structure.
     *  The provided offset has to be smaller to the address size.
     *
     *  \param le_address Address structure
     *  \param le_offset  Offset of the digit - zero based
     *
     *  \return Returns address index digit
     */

    le_byte_t le_address_get_digit( le_address_t const * const le_address, le_size_t const le_offset );

    /*! \brief accessor methods
     *
     *  Returns the additional depth parameter of the class stored in the
     *  address structure.
     *
     *  \param le_address Address structure
     *
     *  \return Address additional depth
     */

    le_byte_t le_address_get_span( le_address_t const * const le_address );

    /*! \brief accessor methods ( revoked )
     *
     *  This function compare the two provided address structures and returns a
     *  boolean value indicating their identity.
     *
     *  \param le_addra Address structure
     *  \param le_addrb Address structure
     *
     *  \return Returns _LE_TRUE on addresses identity, _LE_FALSE otherwise
     */

    le_enum_t le_address_get_equal( le_address_t const * const le_addra, le_address_t const * const le_addrb );

    /* *** note : does not check address size equality */

    le_enum_t le_address_get_greater( le_address_t const * const le_address, le_address_t const * const le_applicant );

    /*! \brief accessor methods
     *
     *  This function reads the digits of the index stored in the address
     *  structure and converts it into a geographic coordinates 3-vector.
     *
     *  In other words, this function computes the geographic coordinates of the
     *  equivalence class corner pointed by the index stored in the address
     *  structure.
     *
     *  This function expects the number of digits of the index to be provided
     *  as parameter, discarding the size field of the address structure. This
     *  allows optimisation to take place.
     *
     *  The \b le_address_get_pose() macro provides a way to invoke the function
     *  without having to explicitly pass the address size as parameter.
     *
     *  \param le_address Address structure
     *  \param le_size    Number of digits to consider
     *  \param le_pose    3-vector receiving the coordinates
     */

    le_void_t le_address_get_pose_( le_address_t const * const le_address, le_size_t le_size, le_real_t * const le_pose );

    /*! \brief mutator methods
     *
     *  Sets the number of digits, i.e. the address size of the index stored in
     *  the provided address structure.
     *
     *  \param le_address Address structure
     *  \param le_size    Number of digits
     */

    le_void_t le_address_set_size( le_address_t * const le_address, le_byte_t const le_size );

    /*! \brief mutator methods
     *
     *  Sets the address time comparison mode.
     *
     *  \param le_address Address structure
     *  \param le_mode    Times comparison mode
     */

    le_void_t le_address_set_mode( le_address_t * const le_address, le_byte_t const le_mode );

    /*! \brief mutator methods ( revoked - used for comb also )
     *
     *  Sets the address desired time. The provided offset has to be 0 or 1 to
     *  set the first or second time, respectively.
     *
     *  \param le_address Address structure
     *  \param le_offset  Offset of time - zero based
     *  \param le_time    Time value
     */

    le_void_t le_address_set_time( le_address_t * const le_address, le_size_t const le_offset, le_time_t const le_time );

    /*! \brief mutator methods
     *
     *  Sets the specified digit of the index stored in the address structure.
     *  The provided offset has to be a whole number smaller that the address
     *  size.
     *
     *  \param le_address Address structure
     *  \param le_offset  Offset of the digit - zero based
     *  \param le_digit   Digit value
     */

    le_void_t le_address_set_digit( le_address_t * const le_address, le_size_t const le_offset, le_byte_t const le_digit );

    /*! \brief mutator methods
     *
     *  Sets the address additional depth.
     *
     *  \param le_address Address structure
     *  \param le_span    Additional depth
     */

    le_void_t le_address_set_span( le_address_t * const le_address, le_byte_t const le_span );

    /*! \brief mutator methods
     *
     *  This function sets the digits of the index stored in the address
     *  structure induced by the provided 3-vector expressed in geographic
     *  coordinates.
     *
     *  The number of digits is read in the structure itself and has to be set
     *  before any call to this function.
     *
     *  \param le_address Address structure
     *  \param le_pose    3-vector containing the coordinates
     */

    le_void_t le_address_set_pose( le_address_t * const le_address, le_real_t * const le_pose );

    /*! \brief serialisation method ( revoked )
     *
     *  This functions is used to pack and unpack an address structure in and
     *  from the provided array. In other words, this function ensure the
     *  translation between its structure representation and its packing in a
     *  byte sequence.
     *
     *  The provided \b le_offset parameter indicates at which byte position the
     *  packing or unpacking of the structure has to take place in the array.
     *
     *  The provided array, for both packing and unpacking, as to be an already
     *  allocated array allowing the packing and unpacking to take place.
     *
     *  \param le_address Address structure
     *  \param le_array   Array structure
     *  \param le_offset  Serialisation offset, in bytes
     *  \param le_mode    Serialisation mode : _LE_SET or _LE_GET
     *
     *  \return Offset of the byte following the structure in the array
     */

    le_size_t le_address_serial( le_address_t * const le_address, le_array_t * const le_array, le_size_t const le_offset, le_enum_t const le_mode );

    /*! \brief conversion methods ( revoked )
     *
     *  This function translate the address structure content in human readable
     *  text form.
     *
     *  Considering m the times comparison mode, t1 and t2 the times, d the
     *  digits of the spatial index and q the query additional depth, the
     *  function provides a string structured as follow :
     *
     *      /m/t1,t2/ddd...d/q
     *
     *  \param le_address Address structure
     *  \param le_string  String receiving the converted address structure
     */

    le_void_t le_address_ct_string( le_address_t const * const le_address, le_char_t * const le_string );

    /*! \brief conversion methods ( revoked )
     *
     *  This function inverts the conversion made by \b le_address_ct_string()
     *  function. It expects an address in human readable text string structured
     *  as defined by this last function :
     *
     *      /m/t1,t2/ddd...d/q
     *
     *  with m the time comparison mode, t1 and t2 the address times, d the
     *  digits of the spatial index and q the additional depth.
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

