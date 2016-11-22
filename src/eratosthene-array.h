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

    /*! \file   eratosthene-array.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Array module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_SYSTEM_ARRAY__
    # define __LE_SYSTEM_ARRAY__

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
    # define LE_ARRAY_C      { 0, 0, NULL }
    # define LE_ARRAY_SF_C   { NULL, NULL, NULL }
    # define LE_ARRAY_RF_C   { NULL, NULL }
    # define LE_ARRAY_TF_C   { NULL }
    # define LE_ARRAY_CF_C   { NULL, NULL }

    /* define array step (bytes) */
    # define LE_ARRAY_STEP   ( 65356 )

    /* Define array streaming mode */
    # define LE_ARRAY_PURGE  ( 0 )
    # define LE_ARRAY_STREAM ( 1 )

    /* define array formats */
    # define LE_ARRAY_SFD    ( 0x01 )
    # define LE_ARRAY_RFD    ( 0x02 )
    # define LE_ARRAY_CFD    ( 0x04 )

    /* define array formats records length */
    # define LE_ARRAY_SFL    ( sizeof( le_real_t ) * 3 + sizeof( le_byte_t ) * 3 + sizeof( le_time_t ) )
    # define LE_ARRAY_RFL    ( sizeof( le_real_t ) * 3 + sizeof( le_byte_t ) * 3 )
    # define LE_ARRAY_CFL    ( sizeof( le_size_t ) + sizeof( le_time_t ) )

/*
    header - preprocessor macros
 */

    # define le_array_sf( a, o, s ) { \
        ( s ).as_pose = ( le_real_t * ) ( ( a ) + ( o ) ); \
        ( s ).as_time = ( le_time_t * ) ( ( s ).as_pose + 3 ); \
        ( s ).as_data = ( le_data_t * ) ( ( s ).as_time + 1 ); \
    }

    # define le_array_rf( a, o, s ) { \
        ( s ).as_pose = ( le_real_t * ) ( ( a ) + ( o ) ); \
        ( s ).as_data = ( le_data_t * ) ( ( s ).as_pose + 3 ); \
    }

    # define le_array_cf( a, o, s ) { \
        ( s ).as_size = ( le_size_t * ) ( ( a ) + ( o ) ); \
        ( s ).as_time = ( le_time_t * ) ( ( s ).as_size + 1 ); \
    }

/*
    header - type definition
 */

    typedef struct {

        le_real_t * as_pose;
        le_time_t * as_time;
        le_data_t * as_data;

    } le_array_sf_t;

    typedef struct {

        le_real_t * as_pose;
        le_data_t * as_data;

    } le_array_rf_t;

    typedef struct {

        le_size_t * as_size;
        le_time_t * as_time;

    } le_array_cf_t;

/*
    header - structures
 */

    /*! \struct le_array_struct
     *  \brief array structure
     *
     *  This structure holds the definition and content of a bytes array. These
     *  arrays are commonly used to transmit data through TCP/IP sockets.
     *
     *  Being a simple bytes succession, arrays are interpreted as more complexe
     *  arrays of structures using memory mapper depending on the actual data
     *  stored in the array.
     *
     *  The structures comes with methods allowing array size management and
     *  socket i/o along with memory mapper structure initiated using macros.
     *
     *  \var le_array_struct::as_virt
     *  Memory size of the bytes array, in bytes
     *  \var le_array_struct::ar_size
     *  Data size of the bytes array, in bytes
     *  \var le_array_struct::ar_byte
     *  Bytes array memory base pointer
     */

    typedef struct le_array_struct {

        le_size_t   as_virt;
        le_size_t   ar_size;
        le_byte_t * ar_byte;

    } le_array_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function simply returns an array structure created using default
     *  values.
     *
     *  \return Returns created structure
     */

    le_array_t le_array_create( le_void_t );

    /*! \brief constructor/destructor methods
     *
     *  This function delete the provided array structure. It frees array
     *  memory when required and clears the structure fields.
     *
     *  \param le_array Array structure
     */

    le_void_t le_array_delete( le_array_t * const le_array );

    /*! \brief accessor methods
     *
     *  Returns the data size, in bytes, of the array contained in the provided
     *  structure.
     *
     *  \param le_array Array structure
     *
     *  \return Array data size, in bytes
     */

    le_size_t le_array_get_size( le_array_t const * const le_array );

    /*! \brief accessor methods
     *
     *  Returns the memory base pointer of the array contained in the provided
     *  structure.
     *
     *  \param le_array Array structure
     *
     *  \return Returns array memory base pointer
     */

    le_byte_t * le_array_get_byte( le_array_t const * const le_array );

    /*! \brief mutator methods
     *
     *  This function checks if an element of length \b le_length is addable to
     *  the array memory allocation. When the array memory is not enough, it
     *  reallocate the array memory allowing the addition of the element.
     *
     *  \param le_array  Array structure
     *  \param le_length Length, in bytes, of added element
     *
     *  \return Return _LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_array_set_memory( le_array_t * const le_array, le_size_t const le_length );

    /*! \brief mutator methods
     *
     *  This function is used as a complexe record insertion method in the bytes
     *  array hold by the provided structure.
     *
     *  It expects a position 3-vector, a time value and a data 3-vector as
     *  parameters and pack them in a bytes sequence that is inserted at the end
     *  of the provided array.
     *
     *  \param le_array Array structure
     *  \param le_pose  Position 3-vector
     *  \param le_time  Time value
     *  \param le_data  Data 3-vector
     */

    le_void_t le_array_set_pushsf( le_array_t * const le_array, le_real_t const * const le_pose, le_time_t const le_time, le_data_t const * const le_data );

    /*! \brief mutator methods
     *
     *  This function is used as a complexe record insertion method in the bytes
     *  array hold by the provided structure.
     *
     *  It expects a position 3-vector and a data 3-vector as parameters and
     *  pack them in a bytes sequence that is inserted at the end of the
     *  provided array.
     *
     *  \param le_array Array structure
     *  \param le_pose  Position 3-vector
     *  \param le_data  Data 3-vector
     */

    le_void_t le_array_set_pushrf( le_array_t * const le_array, le_real_t const * const le_pose, le_data_t const * const le_data );

    /*! \brief mutator methods
     *
     *  This function is used as a complexe record insertion method in the bytes
     *  array hold by the provided structure.
     *
     *  It expects a time value as parameter and pack them in a bytes sequence
     *  that is inserted at the end of the provided array.
     *
     *  \param le_array Array structure
     *  \param le_time  Time value
     */

    le_void_t le_array_set_pushtf( le_array_t * const le_array, le_time_t const le_time );

    /*! \brief mutator methods
     *
     *  This function is used as a complexe record insertion method in the bytes
     *  array hold by the provided structure.
     *
     *  It expects a configuration size and a configuration time as parameters
     *  and pack them in a bytes sequence that is inserted at the end of the
        provided array.
     *
     *  \param le_array Array structure
     *  \param le_size  Configuration size
     *  \param le_time  Configuration time
     */

    le_void_t le_array_set_pushcf( le_array_t * const le_array, le_size_t const le_size, le_time_t const le_time );

    /*! \brief i/o methods
     *
     *  This function writes the data content from the provided array on the
     *  provided opened socket.
     *
     *  \param le_array  Array structure
     *  \param le_socket Opened socket
     *
     *  \return Return _LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_array_io_write( le_array_t const * const le_array, le_sock_t const le_socket );

    /*! \brief i/o methods
     *
     *  This function reads the data content of the provided array from the
     *  provided opened socket.
     *
     *  It reads bytes blocks as their are coming until connection is closed
     *  or socket become silent.
     *
     *  \return Returns _LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_array_io_read( le_array_t * const le_array, le_sock_t const le_socket );

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

