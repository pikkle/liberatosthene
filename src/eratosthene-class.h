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

    /*! \file   eratosthene-class.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Class module
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
    # include "eratosthene-address.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define LE_CLASS_C             { { 0, 0, 0, 0 } }
    # define LE_CLASS_C_DATA(r,g,b) { { r, g, b, 0 } }

/*
    header - preprocessor macros
 */

    # define le_class_offset( c )   ( ( ( le_byte_t * ) ( c )->cs_data ) + _LE_USE_DATA )

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
     *  is considered in the server storage structure.
     *
     *  Formally, it simply consist in a bytes array storing the colorimetric
     *  information of the class representative and the offsets of the daughter
     *  classes that are considered in the storage structure.
     *
     *  Depending on the \b _LE_USE_OFFSET value, the structure of the bytes
     *  array of the class can be summarised as follow :
     *
     *      [m][r][g][b][  off0  ][  off1  ]...[  off7  ]
     *
     *  where m, r, g and b are stored using one byte and where the offsets are
     *  whole numbers coded on _LE_USE_OFFSET bytes long integers.
     *
     *  The color components gives the class representative color, the m value
     *  being used as a pseudo-mean variable. The offsets i gives the position,
     *  in the next scale storage file, of the daughter class i. Usually,
     *  address digits are used to access the offsets 8-array.
     *
     *  \var le_class_struct::cs_data
     *  Bytes array containing the equivalence class storage data
     */

    typedef struct le_class_struct {

        le_data_t cs_data[LE_BUFFER_CLASS + ( sizeof( le_size_t ) - _LE_USE_OFFSET )];

    } le_class_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates and returns and class structure using the default
     *  first color provided as parameter. In addition, it initialises the
     *  daughter class offset array.
     *
     *  \param le_data Colors 3-vector of the first point
     *
     *  \return Returns created class structure
     */

    le_class_t le_class_create( le_data_t const * const le_data );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the class hold by the provided class structure.
     *
     *  \param le_class Class structure
     */

    le_void_t le_class_delete( le_class_t * const le_class );

    /*! \brief accessor methods
     *
     *  Returns storage offset of the daughter class pointed by \b le_addr
     *  index.
     *
     *  \param le_class Class structure
     *  \param le_addr  Daughter class index
     *
     *  \return Daughter class storage offset
     */

    le_size_t le_class_get_offset( le_class_t const * const le_class, le_size_t const le_addr );

    /*! \brief accessor methods
     *
     *  Returns pointer to the class representative color 3-vector.
     *
     *  \param le_class Class structure
     *
     *  \return Class representative color array
     */

    le_data_t * le_class_get_data( le_class_t const * const le_class );

    /*! \brief mutator methods
     *
     *  Set the storage offset of the daughter class pointed by \b le_addr.
     *
     *  \param le_class  Class structure
     *  \param le_addr   Daughter class index
     *  \param le_offset Daughter class storage offset
     */

    le_void_t le_class_set_offset( le_class_t * const le_class, le_size_t const le_addr, le_size_t const le_offset );

    /*! \brief mutator methods
     *
     *  This function injects an new element in the class. It then recomputes
     *  the class representative color taking into account the injected point
     *  color.
     *
     *  \param le_class Class structure
     *  \param le_data  Injected element color 3-vector
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

    le_enum_t le_class_io_read( le_class_t * const le_class, le_size_t const le_offset, FILE * const le_stream );

    /*! \brief i/o methods
     *
     *  This function performs the invert operation of \b le_class_io_write
     *  function. It creates the storage representation of the provided class
     *  in the stream that decribe an already opened file in binary mode.
     *
     *  \param le_class  Class structure
     *  \param le_offset Class storage offset
     *  \param le_stream Stream to an opened file in binary write mode
     *
     *  \return Returns LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_class_io_write( le_class_t const * const le_class, le_size_t const le_offset, FILE * const le_stream );

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

