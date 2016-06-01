/*
 *  liberatosthene - geodetic system
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
     *  Classes module
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

    /* Define pseudo-constructor */
    # define LE_CLASS_C { { _LE_SIZE_NULL }, { 0 }, 0 }

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
        le_data_t cs_data[3];
        le_size_t cs_mean;

    } le_class_t;

/*
    header - function prototypes
 */

    /*! \brief accessor methods
     */

    le_size_t le_class_get_offset( le_class_t const * const le_class, le_size_t const le_addr );

    /*! \brief accessor methods
     */

    le_size_t * le_class_get_addr( le_class_t const * const le_class );

    /*! \brief accessor methods
     */

    le_size_t le_class_get_mean( le_class_t const * const le_class );

    /*! \brief accessor methods
     */

    le_data_t * le_class_get_data( le_class_t const * const le_class );

    /*! \breif mutator methods
     */

    le_void_t le_class_set_init( le_class_t * const le_class, le_data_t const * const le_data );

    /*! \breif mutator methods
     */

    le_enum_t le_class_set_offset( le_class_t * const le_class, le_size_t const le_addr, le_size_t const le_offset );

    /*! \breif mutator methods
     */

    le_void_t le_class_set_push( le_class_t * const le_class, le_data_t const * const le_data );

    /*! \breif i/o methods
     */

    le_enum_t le_class_io_read( le_class_t * const le_class, le_size_t const le_offset, FILE * const le_stream );

    /*! \breif i/o methods
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

