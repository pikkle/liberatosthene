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
    # define LE_ARRAY_C    { 0, 0, NULL }

    /* define array step */
    # define LE_ARRAY_STEP ( 1073741824 )

    /* define array mapping sizes */
    # define LE_ARRAY_SD   ( sizeof( le_real_t ) * 3 + sizeof( le_data_t ) * 3 )
    # define LE_ARRAY_DT   ( sizeof( le_size_t ) + sizeof( le_time_t ) )

/*
    header - preprocessor macros
 */

    /* access macro for sd-records - buffer */
    # define le_array_sd_pose_b( a, o ) ( ( le_real_t * ) ( a + o ) )
    # define le_array_sd_data_b( a, o ) ( ( le_data_t * ) ( a + o + sizeof( le_real_t ) * 3 ) )

    /* access macro for sd-records - array last */
    # define le_array_sd_pose_al( a )   ( ( a )->ar_byte + ( a )->ar_size - LE_ARRAY_SD )
    # define le_array_sd_data_al( a )   ( ( a )->ar_byte + ( a )->ar_size - LE_ARRAY_SD + sizeof( le_real_t ) * 3 )

    /* access macro for dt-records - array */
    # define le_array_dt_size_a( a, o )  ( ( le_size_t * ) ( ( a )->ar_byte + o ) )
    # define le_array_dt_time_a( a, o )  ( ( le_time_t * ) ( ( a )->ar_byte + o + sizeof( le_size_t ) ) )

/*
    header - type definition
 */

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
     *  \var le_array_struct::ar_virt
     *  Memory size of the bytes array, in bytes
     *  \var le_array_struct::ar_size
     *  Data size of the bytes array, in bytes
     *  \var le_array_struct::ar_byte
     *  Bytes array memory base pointer
     */

    typedef struct le_array_struct {

        le_size_t   ar_virt;
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

    le_enum_t le_array_set( le_array_t * const le_array, le_size_t const le_length );

    le_enum_t le_array_set_size( le_array_t * const le_array, le_size_t const le_size );

    le_void_t le_array_map_std( le_array_t * const le_array, le_real_t const * const le_pose, le_time_t const le_time, le_data_t const * const le_data );

    le_void_t le_array_map_sd( le_array_t * const le_array, le_real_t const * const le_pose, le_data_t const * const le_data );

    le_void_t le_array_map_dt( le_array_t * const le_array, le_size_t const le_size, le_time_t const le_time );

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

