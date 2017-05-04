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

    /*! \file   eratosthene-array.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  liberatosthene - array module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_ARRAY__
    # define __LE_ARRAY__

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
    # define LE_ARRAY_C                 { 0, NULL, 0, NULL }

    /* define array step */
    # define LE_ARRAY_STEP              ( 1048576 )

    /* define array header size */
    # define LE_ARRAY_HEADER            ( sizeof( le_size_t ) + sizeof( le_byte_t ) )

    /* define arrays size */
    # define LE_ARRAY_AUTH              ( sizeof( le_size_t ) * 2 + sizeof( le_time_t ) )
    # define LE_ARRAY_ADDR_TIME         ( sizeof( le_time_t ) * 2 )
    # define LE_ARRAY_ADDR_DESC         ( sizeof( le_time_t ) * 3 )
    # define LE_ARRAY_ADDR              ( LE_ARRAY_ADDR_TIME + LE_ARRAY_ADDR_DESC + _LE_USE_DEPTH )

    /* define array mapping sizes - sd */
    # define LE_ARRAY_SD_1              ( sizeof( le_real_t ) * 3 )
    # define LE_ARRAY_SD                ( sizeof( le_data_t ) * 3 + LE_ARRAY_SD_1 )

/*
    header - preprocessor macros
 */

    /* access macro for sd-records - array */
    # define le_array_sd_pose_a( a, o ) ( ( le_real_t * ) ( ( a )->ar_vbyte + o ) )
    # define le_array_sd_data_a( a, o ) ( ( le_data_t * ) ( ( a )->ar_vbyte + o + sizeof( le_real_t ) * 3 ) )

    /* access macro for sd-records - array last */
    # define le_array_sd_pose_al( a )   ( ( a )->ar_vbyte + ( a )->ar_vsize - LE_ARRAY_SD )
    # define le_array_sd_data_al( a )   ( ( a )->ar_vbyte + ( a )->ar_vsize - LE_ARRAY_SD + sizeof( le_real_t ) * 3 )

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
     *  arrays are commonly used to transmit data through TCP/IP sockets. These
     *  arrays of bytes are commonly interpreted through more complex types
     *  using specific access marcos and pointers.
     *
     *  The structure holds the byte array base pointer and two size fields. The
     *  proper size field stores the actual size of the array, in bytes. The
     *  virtual size holds the array memory allocation size, in bytes, usually
     *  greater than the array proper size.
     *
     *  The size and virtual size are used to minimise the amount of memory
     *  reallocation during elements appending, the size following the size of
     *  the appended elements while the virtual size, driving the memory
     *  allocation, follows much larger steps.
     *
     *  \var le_array_struct::ar_virt
     *  Memory size of the bytes array, in bytes
     *  \var le_array_struct::ar_size
     *  Data size of the bytes array, in bytes
     *  \var le_array_struct::ar_byte
     *  Bytes array memory base pointer
     */

    //typedef struct le_array_prev {

        //le_size_t   ar_virt;
        //le_size_t   ar_size;
        //le_byte_t * ar_byte;

    //} le_array_t;

    typedef struct le_array_struct {

        le_size_t   ar_rsize;
        le_byte_t * ar_rbyte;

        //le_size_t * ar_vsize;
        le_size_t   ar_vsize;
        le_byte_t * ar_vbyte;

    } le_array_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function initialise and returns an empty array structure.
     *
     *  \return Returns created structure
     */

    le_array_t le_array_create( le_void_t );

    /*! \brief constructor/destructor methods
     *
     *  This function uninitialises the provided array structure. It unallocates
     *  the array memory and reset the structure fields.
     *
     *  \param le_array Array structure
     */

    le_void_t le_array_delete( le_array_t * const le_array );

    /*! \brief accessor methods
     *
     *  This function returns the size, in bytes, of the data stored in the
     *  provided array structure.
     *
     *  \param le_array Array structure
     *
     *  \return Array data size, in bytes
     */

    le_size_t le_array_get_size( le_array_t const * const le_array );

    /*! \brief accessor methods
     *
     *  This function returns the array memory base pointer.
     *
     *  \param le_array Array structure
     *
     *  \return Returns array memory base pointer
     */

    le_byte_t * le_array_get_byte( le_array_t const * const le_array );

    /*! \brief mutator methods
     *
     *  This function checks if the virtual size of the provided array structure
     *  allows the insertion of an element of size \b le_length.
     *
     *  If the virtual size allows the insertion, the function update the array
     *  size. Otherwise, it update the size of the array and also the virtual
     *  size. In this case, the array memory is reallocated by the function
     *  according to the updated virtual size.
     *
     *  \param le_array  Array structure
     *  \param le_length Length, in bytes, of inserted element
     *
     *  \return Return _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t le_array_set( le_array_t * const le_array, le_size_t const le_length );

    /*! \brief mutator methods
     *
     *  This function is used to force the size of the provided array to a
     *  specific value. The function checks if the virtual size remains greater
     *  than the specified size. In this case, the array memory allocation is
     *  not changed. Otherwise, the virtual size of the array is aligned on the
     *  provided size and the function reallocate the array memory.
     *
     *  \param le_array  Array structure
     *  \param le_size   Size of the array
     *
     *  \return Return _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t le_array_set_size( le_array_t * const le_array, le_size_t const le_size );

    /* *** */

    le_size_t le_array_serial( le_array_t * const le_array, le_void_t * const le_bytes, le_size_t const le_length, le_size_t const le_offset, le_enum_t const le_mode );

    /*! \brief i/o methods
     *
     *  This function writes the provided array bytes in the socket pointed by
     *  the provided socket descriptor.
     *
     *  \param le_array  Array structure
     *  \param le_socket Socket descriptor
     *
     *  \return Return _LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_byte_t le_array_io_write( le_array_t * const le_array, le_byte_t le_mode, le_sock_t const le_socket );

    /*! \brief i/o methods
     *
     *  This function reads the provided array bytes from the socket pointed by
     *  the provided socket descriptor. It reads bytes until the connection is
     *  closed or a timeout is reached.
     *
     *  \param le_array  Array structure
     *  \param le_socket Socket descriptor
     *
     *  \return Returns _LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_byte_t le_array_io_read( le_array_t * const le_array, le_sock_t const le_socket );

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

