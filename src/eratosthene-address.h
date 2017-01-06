/*
 *  liberatosthene
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2017 EPFL CDH DHLAB
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
    # define LE_ADDRESS_C             { 0, 0, 0, { _LE_TIME_NULL, _LE_TIME_NULL }, { 0 } }
    # define LE_ADDRESS_C_SIZE( s )   { s, 0, 0, { _LE_TIME_NULL, _LE_TIME_NULL }, { 0 } }

    /* define WGS84 ellipsoid parameters */
    # define LE_ADDRESS_WGSA          ( 6378137.0 )
    # define LE_ADDRESS_WGSF          ( 298.257223563 )

    /* asynchronous dimension edges */
    # define LE_ADDRESS_SYNP          ( 1 )
    # define LE_ADDRESS_SYNA          ( 10 )

    /* define ellispoidal coordinates boundaries */
    # define LE_ADDRESS_MINL          ( - LE_PI )
    # define LE_ADDRESS_MAXL          ( + LE_PI )
    # define LE_ADDRESS_MINA          ( - LE_P2 )
    # define LE_ADDRESS_MAXA          ( + LE_P2 )
    # define LE_ADDRESS_MINH          ( - ( LE_PI * LE_ADDRESS_WGSA ) / 1024.0 )
    # define LE_ADDRESS_MAXH          ( + ( LE_PI * LE_ADDRESS_WGSA ) / 1024.0 )

    /* define ellipsoidal coordinates ranges */
    # define LE_ADDRESS_RNGL          ( LE_ADDRESS_MAXL - LE_ADDRESS_MINL )
    # define LE_ADDRESS_RNGA          ( LE_ADDRESS_MAXA - LE_ADDRESS_MINA )
    # define LE_ADDRESS_RNGH          ( LE_ADDRESS_MAXH - LE_ADDRESS_MINH )

/*
    header - preprocessor macros
 */

    /* base according to scale */
    # define le_address_base( s )     ( ( ( s ) < LE_ADDRESS_SYNA ) ? ( _LE_USE_BASE >> 1 ) : ( ( ( s ) < LE_ADDRESS_SYNP ) ? ( _LE_USE_BASE >> 2 ) : _LE_USE_BASE ) )

    /* method alias */
    # define le_address_get_pose(a,p) ( le_address_get_pose_( ( a ), ( a )->as_size, ( p ) ) )

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct le_address_struct
     *  \brief address structure
     *
     *  This structure holds the address, or the index, of an equivalence class
     *  defined on the geodetic and time parameter spaces. These addresses are
     *  used to store and access data linked to spatiotemporal references.
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
     *  In the last place, the structure holds an depth parameter that gives,
     *  usually in case of data query, the additional scale depth where the
     *  desired data have to be gathered.
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
     *  gives the additionnal depth, from the scale point of view, where the
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
        le_time_t as_times[LE_BUFFER_TIME];
        le_byte_t as_digit[LE_BUFFER_ADDR];

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

    /*! \brief accessor methods
     *
     *  Returns the desired time of the equivalence class stored in the address
     *  structure.
     *
     *  \param le_address Address structure
     *  \param le_offset  Offset of time - zero based
     *
     *  \return Address time
     */

    le_time_t le_address_get_time( le_address_t const * const le_address, le_size_t const le_offset );

    /*! \brief accessor methods
     *
     *  Returns the desired digit of the index stored in the address structure.
     *
     *  \param le_address Address structure
     *  \param le_offset  Offset of the digit - zero based
     *
     *  \return Returns address index digit
     */

    le_byte_t le_address_get_digit( le_address_t const * const le_address, le_size_t const le_offset );

    /*! \brief accessor methods
     *
     *  Returns query additional depth parameter of the class stored in the
     *  address structure.
     *
     *  \param le_address Address structure
     *
     *  \return Query additional depth
     */

    le_byte_t le_address_get_span( le_address_t const * const le_address );

    /*! \brief accessor methods
     *
     *  This function compare the two provided address structures and returns a
     *  Boolean value indicating their identity.
     *
     *  \param le_addr1 First address structure
     *  \param le_addr2 Second address structure
     *
     *  \return Returns _LE_TRUE on identity, _LE_FALSE otherwise
     */

    le_enum_t le_address_get_equal( le_address_t const * const le_addr1, le_address_t const * const le_addr2 );

    /*! \brief accessor methods
     *
     *  This function reads the digits of the index stored in the address
     *  structure and converts it into a geographic coordinates 3-vector.
     *
     *  In other words, this function compute the geographic coordinates of the
     *  equivalence class corner pointed by the index stored in the address
     *  structure.
     *
     *  This function expects the number of digits of the index to be provided
     *  as parameter, discarding the size field of the address structure.
     *
     *  \param le_address Address structure
     *  \param le_size    Number of digits
     *  \param le_pose    3-vector receiving the coordinates
     */

    le_void_t le_address_get_pose_( le_address_t const * const le_address, le_size_t le_size, le_real_t * const le_pose );

    /*! \brief mutator methods
     *
     *  Set the number of digits of the index stored in the provided address
     *  structure.
     *
     *  \param le_address Address structure
     *  \param le_size    Number of digits
     */

    le_void_t le_address_set_size( le_address_t * const le_address, le_byte_t const le_size );

    /*! \brief mutator methods
     *
     *  Set the address time comparison mode.
     *
     *  \param le_address Address structure
     *  \param le_mode    Times comparison mode
     */

    le_void_t le_address_set_mode( le_address_t * const le_address, le_byte_t const le_mode );

    /*! \brief mutator methods
     *
     *  Set the address time.
     *
     *  \param le_address Address structure
     *  \param le_offset  Offset of time - zero based
     *  \param le_time    Time value
     */

    le_void_t le_address_set_time( le_address_t * const le_address, le_size_t const le_offset, le_time_t const le_time );

    /*! \brief mutator methods
     *
     *  Set the specified digit of the index stored in the address structure.
     *
     *  \param le_address Address structure
     *  \param le_offset  Offset of the digit - zero based
     *  \param le_digit   Digit value
     */

    le_void_t le_address_set_digit( le_address_t * const le_address, le_size_t const le_offset, le_byte_t const le_digit );

    /*! \brief mutator methods
     *
     *  Set the address additional depth (span).
     *
     *  \param le_address Address structure
     *  \param le_span    Additionnal depth
     */

    le_void_t le_address_set_span( le_address_t * const le_address, le_byte_t const le_span );

    /*! \brief mutator methods
     *
     *  This function sets the digits of the index stored in the address
     *  structure induced by the provided 3-vector expressed in geographic
     *  coordinates. The number of digits is read in the structure itself and
     *  has to be set before any call to this function.
     *
     *  \param le_address Address structure
     *  \param le_pose    3-vector containing the coordinates
     */

    le_void_t le_address_set_pose( le_address_t * const le_address, le_real_t * const le_pose );

    /*! \brief i/o methods
     *
     *  This function reads in the provided socket descriptor the compact byte
     *  representation of the address and translates it into a standard address
     *  structure.
     *
     *  The compact byte representation of the address is expected in the format
     *  defined by the \b le_address_io_write() function.
     *
     *  \param le_address Address structure
     *  \param le_socket  Socket descriptor
     *
     *  \return Returns _LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_address_io_read( le_address_t * const le_address, le_sock_t const le_socket );

    /*! \brief i/o methods
     *
     *  This function translates the provided standard address structure in its
     *  compact byte representation. It then write the compacted address in the
     *  socket pointed by the socket descriptor.
     *
     *  \param le_address Address structure
     *  \param le_socket  Socket descriptor
     *
     *  \return Returns _LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_address_io_write( le_address_t * const le_address, le_sock_t const le_socket );

    /*! \brief conversion methods
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

    /*! \brief conversion methods
     *
     *  This function inverts the conversion made by \b le_address_ct_string()
     *  function. It expects address in human readable text string structured
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

