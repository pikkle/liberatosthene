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
    # define LE_ARRAY_C                 { 0, NULL, 0, 0, NULL }

    /* define array step */
    # define LE_ARRAY_STEP              ( 65536 )

    /* define array header size */
    # define LE_ARRAY_HEADER_SIZE       ( sizeof( le_size_t ) * 2 )
    # define LE_ARRAY_HEADER_MODE       ( sizeof( le_byte_t ) )
    # define LE_ARRAY_HEADER            ( LE_ARRAY_HEADER_SIZE + LE_ARRAY_HEADER_MODE )

    /* define array-size - authorise */
    # define LE_ARRAY_AUTH              ( sizeof( le_size_t ) + sizeof( le_time_t ) )

    /* define array-size - address */
    # define LE_ARRAY_ADDR_TIME         ( sizeof( le_time_t ) * 2 )
    # define LE_ARRAY_ADDR_DESC         ( sizeof( le_byte_t ) * 3 )
    # define LE_ARRAY_ADDR              ( LE_ARRAY_ADDR_TIME + LE_ARRAY_ADDR_DESC + _LE_USE_DEPTH )

    /* define array-size - uf3/cu3 */
    # define LE_ARRAY_UF3_POSE          ( 3 * sizeof( le_real_t ) )
    # define LE_ARRAY_UF3_DATA          ( 3 * sizeof( le_data_t ) )
    # define LE_ARRAY_UF3               ( LE_ARRAY_UF3_POSE + LE_ARRAY_UF3_DATA )

    /* define array-size - inject */
    # define LE_ARRAY_INJE_HEAD         ( sizeof( le_time_t ) )

/*
    header - preprocessor macros
 */

    /* define encoded size */
    # define le_array_entropy( a )      ( ( ( a )->ar_vsize / LE_ARRAY_UF3 ) * ( LE_ARRAY_UF3 + 3 ) )

    /* access macro for sd-records - array */
    # define le_array_sd_pose_a( a, o ) ( ( le_real_t * ) ( ( a )->ar_vbyte + o ) )
    # define le_array_sd_data_a( a, o ) ( ( le_data_t * ) ( ( a )->ar_vbyte + o + sizeof( le_real_t ) * 3 ) )
    # define le_array_sd_pose_al( a )   ( ( a )->ar_vbyte + ( a )->ar_vsize - LE_ARRAY_UF3 )
    # define le_array_sd_data_al( a )   ( ( a )->ar_vbyte + ( a )->ar_vsize - LE_ARRAY_UF3 + sizeof( le_real_t ) * 3 )

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
     *  arrays of bytes are commonly interpreted through serialisation and
     *  access macro and functions.
     *
     *  More specifically, the structure holds an array in an array. The first
     *  array, corresponding to the allocated memory has a virtual size,
     *  usually greater than the actual data array contain in it. This size
     *  correspond to the available memory.
     *
     *  A second array is defined through its base pointer (\b ar_vbyte) and its
     *  size (\b ar_vsize). This pseudo array contains the proper data. From
     *  a memory point of view, one can represent the data array as follows :
     *
     *      R...RD...DR...R
     *
     *  where R designate first array bytes and D byte of the first array being
     *  part of the data array.
     *
     *  This is done so for two main reasons : the first one is the possibility
     *  to have a header in which the sizes and array mode can be packed and
     *  send over TCP/IP without additional memory allocation and copy. The
     *  second reason is the possibility to only reallocate memory when the
     *  real array is not sufficient to holds the data array, again saving
     *  memory allocation and copy.
     *
     *  A last field is available in the structure that is the compressed size
     *  \b ar_csize. This fields indicates, if not zero, that the data array
     *  is compressed. The value of this fields gives the size of the array
     *  in its uncompressed form, the compressed size being stored by the
     *  \b ar_vsize fields.
     *
     *  When the array is written on TCP/IP socket, the header is filled as
     *  follows :
     *
     *      [ar_vsize][ar_csize][mode] ...
     *
     *  As the array is received in the remote computer, the header contains
     *  all the information needed to rebuild the array.
     *
     *  \var le_array_struct::ar_rsize
     *  Array real memory size, in bytes
     *  \var le_array_struct::ar_rbyte
     *  Array real memory base pointer
     *  \var le_array_struct::ar_vsize
     *  Size, in bytes, of the data stored in the array
     *  \var le_array_struct::ar_vbyte
     *  Base pointer of the data stored in the array
     *  \var le_array_struct::ar_csize
     *  Size of the data in uncompressed form or zero
     */

    typedef struct le_array_struct {

        le_size_t   ar_rsize;
        le_byte_t * ar_rbyte;

        le_size_t   ar_vsize;
        le_size_t   ar_csize;
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
     *  This function returns the array data memory base pointer.
     *
     *  \param le_array Array structure
     *
     *  \return Returns array memory base pointer
     */

    le_byte_t * le_array_get_byte( le_array_t * const le_array );

    /* *** */

    le_void_t le_array_set_header( le_array_t * const le_array, le_byte_t const le_mode );

    /* *** */

    le_byte_t le_array_set_array( le_array_t * const le_array );

    /*! \brief mutator methods
     *
     *  This function checks if the memory size of the provided array structure
     *  allows the insertion of an element of size \b le_length.
     *
     *  If the memory size allows the insertion, the function update the array
     *  data size. Otherwise, it updates the size of the array and also the data
     *  size. In this case, the array memory is reallocated by the function
     *  according to the updated memory size.
     *
     *  \param le_array  Array structure
     *  \param le_length Length, in bytes, of inserted element
     *
     *  \return Return _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t le_array_set( le_array_t * const le_array, le_size_t const le_length );

    /*! \brief mutator methods
     *
     *  This function is used to force the data size of the provided array to a
     *  specific value. The function checks if the memory size remains greater
     *  than the specified size. In this case, the array memory allocation is
     *  not changed. Otherwise, the memory size of the array is aligned on the
     *  provided size and the function reallocates the array memory.
     *
     *  \param le_array  Array structure
     *  \param le_size   Size of the array
     *
     *  \return Return _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t le_array_set_size( le_array_t * const le_array, le_size_t const le_size );

    /*! \brief serialisation methods
     *
     *  This function is used to pack or unpack variables in the data array
     *  stored in the provided array structure. It receives a variable and it
     *  size and pack or unpack it in the array at the specified offset
     *  according to the provided mode.
     *
     *  \param le_array  Array structure
     *  \param le_bytes  Pointer to the variable to pack/unpack
     *  \param le_length Size, in bytes, of the variable to pack/unpack
     *  \param le_offset Pack/unpack offset in the data array, in bytes
     *  \param le_mode   Serialisation mode : _LE_SET or _LE_GET
     *
     *  \return Returns the variable length added to the provided offset
     */

    le_size_t le_array_serial( le_array_t * const le_array, le_void_t * const le_bytes, le_size_t const le_length, le_size_t const le_offset, le_enum_t const le_mode );

    /*! \brief i/o methods
     *
     *  This function writes the provided array bytes in the socket pointed by
     *  the provided socket descriptor. This function also sets the array
     *  structure header before to send the data over TCP/IP.
     *
     *  In addition to the data array size, the header also contains a mode
     *  value to indicate the type of data carried by the array. This mode
     *  value is set according to the provided \b le_mode parameter.
     *
     *  \param le_array  Array structure
     *  \param le_mode   Socket array mode
     *  \param le_socket Socket descriptor
     *
     *  \return Return _LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_byte_t le_array_io_write( le_array_t * const le_array, le_byte_t le_mode, le_sock_t const le_socket );

    /*! \brief i/o methods
     *
     *  This function reads the provided array bytes from the socket pointed by
     *  the provided socket descriptor. The reading is based on the analysis
     *  of the read array header.
     *
     *  \param le_array  Array structure
     *  \param le_socket Socket descriptor
     *
     *  \return Returns _LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_byte_t le_array_io_read( le_array_t * const le_array, le_sock_t const le_socket );

    /*! \brief uf3-specific methods
     *
     *  This function implements an entropic compression algorithm that can be
     *  applied on array containing uf3 content, that is succession of records
     *  of three doubles and three bytes.
     *
     *  The compressed size is set by the function according to the compression
     *  results. If the compression succeed, the compressed size is set to the
     *  original data array size. It is set to zero otherwise.
     *
     *  \param le_src Original array structure
     *  \param le_dst Encoded array structure
     *
     *  \return Returns LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_array_uf3_encode( le_array_t const * const le_src, le_array_t * const le_dst );

    /*! \brief uf3-specific methods
     *
     *  This function is used to decompress data contained in the provided
     *  array structure that are encoded using the \b le_array_uf3_encode()
     *  algorithm.
     *
     *  The function checks the structure compressed size to determine if the
     *  data array needs to be decoded. A simple swap is performed otherwise.
     *
     *  \param le_src Encoded array structure
     *  \param le_dst Decoded array structure
     *
     * \return Returns LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_array_uf3_decode( le_array_t const * const le_src, le_array_t * const le_dst );

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

