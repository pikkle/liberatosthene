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

    /* Define initialiser */
    # define LE_CLASS_I_OFFSET {\
    _LE_OFFS_NULL, \
    _LE_OFFS_NULL, \
    _LE_OFFS_NULL, \
    _LE_OFFS_NULL, \
    _LE_OFFS_NULL, \
    _LE_OFFS_NULL, \
    _LE_OFFS_NULL, \
    _LE_OFFS_NULL, \
    }

    /* Define pseudo-constructor */
    # define LE_CLASS_C             { LE_CLASS_I_OFFSET, { 0 } }
    # define LE_CLASS_C_DATA(r,g,b) { LE_CLASS_I_OFFSET, { r, g, b, 0 } }

    /* Define class buffer size */
    # define LE_CLASS_BUFFER_SIZE   ( _LE_USE_OFFSET * 8 + 4 )

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct le_class_struct
     *  \brief class structure
     *
     *  This structure holds the representative of an equivalence class defined
     *  on three dimensional colorimetric points.
     *
     *  The representative stored by the structure has a color stored in the
     *  \b cs_data field. The \b cs_mean field gives the number of points
     *  considered to compute the representative color.
     *
     *  The spatial information of the equivalence class is stored through the
     *  \b cs_addr array giving the file storage offset of the daughter
     *  structures in the next scale. In other words, the proper spatial
     *  information of the equivalence class can only be accessed following
     *  the offsets chains along the storage structure scales.
     *
     *  \var le_class_struct::cs_addr
     *  Offset, in bytes, of the class daughter in the next scale storage file
     *  \var le_class_struct::cs_data
     *  Class representative color
     *  \var le_class_struct::cs_mean
     *  Number of points considered to compute the representative color
     */

    typedef struct le_class_struct {

        le_size_t cs_addr[8];
        le_data_t cs_data[4];

    } le_class_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     */

    le_class_t le_class_create( le_data_t const * const le_data );

    /*! \brief constructor/destructor methods
     *
     */

    le_class_t le_class_delete( le_void_t );

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
     *  Returns pointer to daughter class storage offset array.
     *
     *  \param le_class Class structure
     *
     *  \return Daughter class storage offset array
     */

    le_size_t * le_class_get_addr( le_class_t const * const le_class );

    /*! \brief accessor methods
     *
     *  Returns pointer to class representative color array.
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
     *
     *  \return Returns LE_ERROR_SUCCESS on success and LE_ERROR_BASE on
     *  inconsistent index
     */

    le_enum_t le_class_set_offset( le_class_t * const le_class, le_size_t const le_addr, le_size_t const le_offset );

    /*! \brief mutator methods
     *
     *  This function injects an new element in the class. It then recomputes
     *  the class representative color taking into account the injected point
     *  color.
     *
     *  \param le_class Class structure
     *  \param le_data  Injected element color array
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

