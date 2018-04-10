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

    /*! \file   eratosthene-tree-service.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  liberatosthene - tree service module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_TREE_SERVICE__
    # define __LE_TREE_SERVICE__

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
    # include "eratosthene-address.h"
    # include "eratosthene-array.h"
    # include "eratosthene-class.h"
    # include "eratosthene-tree.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

/*
    header - function prototypes
 */

    /*! \brief i/o methods
     *
     *  This function writes the content of the incoming array in the server
     *  storage structure.
     *
     *  The provided array is expect to have the UF3 format, that is a series
     *  of records composed of a coordinates 3-vector and a colour 3-vector.
     *
     *  For each array record, the function initiates an address structure using
     *  the record coordinates to compute the spatial index. The index is then
     *  used to drive the progression of the record injection through the scale
     *  files.
     *
     *  For a given scale, the class the record belong to is searched. If the
     *  class is found, the element is added to the class. Otherwise, a new
     *  class is created using the record to initialise it. The parent class
     *  offsets array is updated to take into account the new created class.
     *
     *  \param le_unit   Unit structure
     *  \param le_array  Array structure
     *  \param le_scfg   Server spatial parameter
     */

    le_void_t le_service_io_inject( le_unit_t * const le_unit, le_array_t const * const le_array, le_size_t const le_scfg );

    /*! \brief i/o methods
     *
     *  This function is used to gather spatial classes positions and colours
     *  information to build a array using the UF3 format.
     *
     *  This function assume that the offset of the spatial class pointed by the
     *  address index is known (\b le_tree_io_offset()). Starting from the
     *  offsets array of this class, it starts to gather the position and
     *  colours by enumerating the class daughters and sub-daughters through a
     *  recursive process. It detect the gathering scale using the main class
     *  scale and the address additional depth (span).
     *
     *  If the provided array is not passed empty to the function, its previous
     *  content is left unchanged, the function pushing the element at the end
     *  of it.
     *
     *  \param le_unit   Unit structure
     *  \param le_addr   Address structure
     *  \param le_offset Class storage offset
     *  \param le_parse  Class storage scale
     *  \param le_span   Query additional depth
     *  \param le_array  Data array filled by the function
     */

    le_void_t le_service_io_gather( le_unit_t * const le_unit, le_address_t * const le_addr, le_size_t le_offset, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array );

    /*! \brief i/o methods
     *
     *  This function implements a parallel version of \b le_service_io_gather()
     *  function. It performs the same operations simply considering two times
     *  values.
     *
     *  As two parallel gathering process take place in this function, it allows
     *  it to consider the times comparison mode provided by the address. As
     *  elements for the two times are gathered at the same time, the function
     *  is able to easily implements the logical operators. The array is then
     *  filled with the results of the application of the logical operators.
     *
     *  If the provided array is not passed empty to the function, its previous
     *  content is left unchanged, the function pushing the element at the end
     *  of it.
     *
     *  \param le_unia    Unit structure (time 1)
     *  \param le_unib    Unit structure (time 2)
     *  \param le_addr    Address structure
     *  \param le_mode    Address mode
     *  \param le_offseta Class storage offset (time 1)
     *  \param le_offsetb Class storage offset (time 2)
     *  \param le_parse   Class storage scale
     *  \param le_span    Query additional depth
     *  \param le_array   Data array filled by the function
     */

    le_void_t le_service_io_parallel( le_unit_t * const le_unia, le_unit_t * const le_unib, le_address_t * const le_addr, le_byte_t const le_mode, le_size_t le_offseta, le_size_t le_offsetb, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array );

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

