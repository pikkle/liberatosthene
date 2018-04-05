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

    /*! \file   eratosthene-class.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  liberatosthene - class module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_CLASS__
    # define __LE_CLASS__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    "C" {
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
    # define LE_CLASS_C             { { 0, 0, 0, 0 } }
    # define LE_CLASS_C_DATA(r,g,b) { { r, g, b, 0 } }

    /* define class offsets */
    # define LE_CLASS_OFFSET        ( _LE_USE_OFFSET * _LE_USE_BASE )

    /* define class array */
    # define LE_CLASS_ARRAY         ( _LE_USE_DATA + LE_CLASS_OFFSET )

    /* define class memory */
    # define LE_CLASS_MEMORY        ( LE_CLASS_ARRAY + sizeof( le_size_t ) - _LE_USE_OFFSET )

    /* define class offset mask */
    # define LE_CLASS_MASK          ( ~ ( _LE_OFFS_NULL ) )

/*
    header - preprocessor macros
 */

    /* define offset pointer */
    # define le_class_offset(c,i)   ( ( le_size_t * ) ( ( ( le_byte_t * ) ( c )->cs_data ) + _LE_USE_DATA + _LE_USE_OFFSET * ( i ) ) )

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct le_class_struct
     *  \brief class structure
     *
     *  This structure holds the representation of an equivalence class as it
     *  is considered in the server storage structure. Such structure can be
     *  seen as the fundamental microstructure of the server storage structure.
     *
     *  Formally, it simply consists in a bytes array storing the colorimetric
     *  information of the class representative and the offsets of the daughter
     *  classes that are considered in the storage structure.
     *
     *  Depending on the \b _LE_USE_OFFSET value, the structure of the bytes
     *  array of the class can be summarised as follow :
     *
     *      [r][g][b][m][  off0  ][  off1  ]...[  off7  ]
     *
     *  where m, r, g and b are stored using one byte and where the offsets are
     *  whole numbers coded on _LE_USE_OFFSET bytes long integers.
     *
     *  The colour components gives the class representative colour, the m value
     *  being used as a pseudo-mean variable. The offsets i gives the position,
     *  in the next scale storage file, of the daughter class i. Usually,
     *  address digits are used to access the offsets 8-array of this class
     *  structure.
     *
     *  \var le_class_struct::cs_data
     *  Bytes array containing the equivalence class storage data
     */

    typedef struct le_class_struct {

        le_data_t cs_data[LE_CLASS_MEMORY];

    } le_class_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function initialises and returns a class structure. It uses the
     *  provided data as first element that initialise the class content. In
     *  addition it initialises the class offset array using the NULL offsets.
     *
     *  \param le_data Data 3-vector - single byte red, green and blue
     *
     *  \return Returns created class structure
     */

    le_class_t le_class_create( le_data_t const * const le_data );

    /*! \brief constructor/destructor methods
     *
     *  This function uninitialises the provided class structure. It simply
     *  erases the class structure fields using default values.
     *
     *  \param le_class Class structure
     */

    le_void_t le_class_delete( le_class_t * const le_class );

    /*! \brief accessor methods
     *
     *  This function returns the offset of the daughter class pointed by the
     *  provided index.
     *
     *  \param le_class Class structure
     *  \param le_index Offset index
     *
     *  \return Daughter class storage offset
     */

    le_size_t le_class_get_offset( le_class_t const * const le_class, le_size_t const le_index );

    /*! \brief mutator methods
     *
     *  This function sets the offset of the daughter class pointed by the
     *  index using the provided offset.
     *
     *  \param le_class  Class structure
     *  \param le_index  Offset index
     *  \param le_offset Offset value
     */

    le_void_t le_class_set_offset( le_class_t * const le_class, le_size_t const le_index, le_size_t const le_offset );

    /*! \brief mutator methods
     *
     *  This function injects the provided data 3-vector in the provided class
     *  structure. The function then recomputes the class representative colour
     *  taking into account the injected data through a pseudo-mean method.
     *
     *  \param le_class Class structure
     *  \param le_data  Data 3-vector - single byte red, green and blue
     */

    le_void_t le_class_set_push( le_class_t * const le_class, le_data_t const * const le_data );

    /*! \brief i/o methods
     *
     *  This function reads the storage representation of a class structure and
     *  imports it in the provided class structure. The \b le_stream has to
     *  contain the descriptor of an already opened file in binary mode.
     *
     *  \param le_class  Class structure
     *  \param le_offset Class storage offset
     *  \param le_stream Stream to an opened file in binary read mode
     *
     *  \return Returns LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_class_io_read( le_class_t * const le_class, le_size_t const le_offset, le_file_t const le_stream );

    /*! \brief i/o methods
     *
     *  This function is a variation of the \b le_class_io_read(). It also reads
     *  the storage structure of the pointed class but only reads the offset
     *  array parts, discarding the class data. The \b le_stream has to contain
     *  the descriptor of an already opened file in binary mode.
     *
     *  \param le_class  Class structure
     *  \param le_offset Class storage offset
     *  \param le_stream Stream to an opened file in binary read mode
     *
     *  \return Returns LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_class_io_readf( le_class_t * const le_class, le_size_t const le_offset, le_file_t const le_stream );

    /*! \brief i/o methods
     *
     *  This function performs the invert operation of \b le_class_io_write()
     *  function. It creates the storage representation of the provided class
     *  in the stream that describes an already opened file in binary mode.
     *
     *  \param le_class  Class structure
     *  \param le_offset Class storage offset
     *  \param le_stream Stream to an opened file in binary write mode
     *
     *  \return Returns LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_class_io_write( le_class_t const * const le_class, le_size_t const le_offset, le_file_t const le_stream );

    /*! \brief detached methods
     *
     *  Note :
     *
     *  Detached methods are specific function associated to a structure but
     *  without operating directly on it. Such function are used in optimisation
     *  strategies.
     *
     *  This function extract the offset of the class storage structure. The
     *  offset is pointed by the provided class storage offset and its index.
     *
     *  This function allows to query a class offset without having to import
     *  all its content and to extract the desired offset.
     *
     *  \param le_offset Class storage offset
     *  \param le_index  Offset index
     *  \param le_stream Stream to an opened file in binary read mode
     *
     *  \return Returns read offset value on success, _LE_OFFS_NULL otherwise
     */

    le_size_t le_class_det_offset( le_size_t const le_offset, le_size_t const le_index, le_file_t const le_stream );

    /*! \brief detached methods
     *
     *  Note :
     *
     *  Detached methods are specific function associated to a structure but
     *  without operating directly on it. Such function are used in optimisation
     *  strategies.
     *
     *  This function reads the class data in the provided data array. The class
     *  is accessed through its provided storage offset.
     *
     *  This function allows to read a class data content without having to
     *  import all its content and copy its data in an auxiliary array.
     *
     *  \param le_offset Class storage offset
     *  \param le_data   Data array pointer
     *  \param le_stream Stream to an opened file in binary read mode
     *
     *  \return Returns LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_class_det_data( le_size_t const le_offset, le_data_t * const le_data, le_file_t const le_stream );

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

